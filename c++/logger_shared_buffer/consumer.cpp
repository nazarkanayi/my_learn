// consumer_daemon.cpp
#include "shared_spsc.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

static std::atomic<bool> stop_flag{false};

// daemonize (simple double-fork)
static bool daemonize() {
    pid_t pid = fork();
    if (pid < 0) return false;
    if (pid > 0) _exit(0); // parent exits

    if (setsid() < 0) return false;

    pid = fork();
    if (pid < 0) return false;
    if (pid > 0) _exit(0);

    umask(0);
    if (chdir("/") < 0) return false;

    // close std fds
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return true;
}

// recv one fd from connected socket
static int recv_fd(int sock) {
    struct msghdr msg{};
    char buf[1];
    struct iovec iov;
    iov.iov_base = buf;
    iov.iov_len = 1;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    char cbuf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = cbuf;
    msg.msg_controllen = sizeof(cbuf);

    ssize_t n = recvmsg(sock, &msg, 0);
    if (n <= 0) return -1;
    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    if (!cmsg) return -1;
    if (cmsg->cmsg_level != SOL_SOCKET || cmsg->cmsg_type != SCM_RIGHTS) return -1;

    int fd;
    memcpy(&fd, CMSG_DATA(cmsg), sizeof(int));
    return fd;
}

static void reader_thread_func(int fd, std::string identity) {
    size_t total = sizeof(SPSCBuffer);
    void* base = mmap(nullptr, total, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (base == MAP_FAILED) {
        // cannot map: close fd and exit thread
        std::cerr << "reader(" << identity << "): mmap failed: " << strerror(errno) << "\n";
        close(fd);
        return;
    }
    SPSCBuffer* buf = reinterpret_cast<SPSCBuffer*>(base);
    std::cout << "reader(" << identity << "): mapped buffer capacity=" << buf->meta.capacity << "\n";

    // Consume: we'll use head local loop but we must update shared head so producer sees progress.
    while (!stop_flag.load(std::memory_order_acquire)) {
        uint64_t ticket = buf->meta.head.load(std::memory_order_relaxed);
        size_t idx = static_cast<size_t>(ticket % buf->meta.capacity);
        SlotHeader* sslot = &buf->slots[idx];

        // wait until seq == ticket + 1
        bool got = false;
        for (int spin = 0; !got && !stop_flag.load(std::memory_order_acquire); ++spin) {
            uint64_t seq = sslot->seq.load(std::memory_order_acquire);
            if (seq == ticket + 1) { got = true; break; }
            // simple backoff
            if ((spin & 0x3F) == 0) std::this_thread::sleep_for(std::chrono::milliseconds(50));
            else sched_yield();
        }
        if (!got) break;

        uint32_t len = sslot->len;
        if (len > buf->meta.slot_bytes) len = static_cast<uint32_t>(buf->meta.slot_bytes);
        std::string out(reinterpret_cast<char*>(sslot->data), len);

        // Print to syslog-like place; since daemon stdout/stderr closed, write to /dev/null safe; here we write to STDERR via a file descriptor 2 reopened to /tmp/consumer.log earlier (or use syslog).
        std::cout << "[" << identity << "] " << out << "\n";

        // release slot: seq = ticket + capacity
        sslot->seq.store(ticket + buf->meta.capacity, std::memory_order_release);

        // advance head
        buf->meta.head.store(ticket + 1, std::memory_order_release);
    }

    munmap(base, total);
    close(fd);
    std::cout << "reader(" << identity << "): exiting\n";
}

// signal handler
static void handle_signal(int signo) {
    stop_flag.store(true);
}

int main(int argc, char** argv) {
    const char* socket_path = (argc > 1) ? argv[1] : "/tmp/spsc_multi.sock";

    // Setup signal handlers for graceful shutdown
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Optionally daemonize (comment/uncomment as needed)
    if (!daemonize()) {
        // If daemonize fails, we will still continue in foreground for debug
        // but print message to stdout which is closed in daemon path
    }

    // Re-open stdout/stderr to a log file for the daemon to write reader outputs
    // (this is optional but useful; attempt to open /tmp/consumer.log)
    int logfd = open("/tmp/consumer.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (logfd >= 0) {
        dup2(logfd, STDOUT_FILENO);
        dup2(logfd, STDERR_FILENO);
        close(logfd);
    }

    // create listening unix socket
    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        std::cerr << "consumer: socket create failed: " << strerror(errno) << "\n";
        return 1;
    }

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "consumer: bind failed: " << strerror(errno) << "\n";
        close(listen_fd);
        return 1;
    }

    if (listen(listen_fd, 32) < 0) {
        std::cerr << "consumer: listen failed: " << strerror(errno) << "\n";
        close(listen_fd);
        unlink(socket_path);
        return 1;
    }

    std::cout << "consumer daemon listening on " << socket_path << "\n";

    std::vector<std::thread> readers;
    readers.reserve(16);

    while (!stop_flag.load(std::memory_order_acquire)) {
        // use accept with timeout: make socket non-blocking or use select; simpler: accept with blocking but break on signal
        int client = accept(listen_fd, nullptr, nullptr);
        if (client < 0) {
            if (errno == EINTR && stop_flag.load()) break;
            // transient error: continue
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // receive FD
        int fd = recv_fd(client);
        if (fd < 0) {
            std::cerr << "consumer: failed to recv fd from client: " << strerror(errno) << "\n";
            close(client);
            continue;
        }

        // make identity string based on pid if possible (not provided), otherwise fd
        std::string identity = "fd:" + std::to_string(fd);

        // spawn thread to read this buffer
        std::thread t([fd, identity]() {
            reader_thread_func(fd, identity);
        });
        t.detach();

        close(client); // close socket end; thread owns fd and mapping
    }

    // shutdown
    close(listen_fd);
    unlink(socket_path);

    // wait briefly for threads to exit
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "consumer daemon exiting\n";
    return 0;
}

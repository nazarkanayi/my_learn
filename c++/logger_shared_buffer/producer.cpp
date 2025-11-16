// producer.cpp
#include "shared_spsc.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#ifndef MFD_CLOEXEC
#define MFD_CLOEXEC 0x0001
#endif

static int memfd_create_wrapper(const char* name, unsigned int flags) {
#if defined(SYS_memfd_create)
    return syscall(SYS_memfd_create, name, flags);
#else
    errno = ENOSYS;
    return -1;
#endif
}

// send one fd via connected unix socket (SCM_RIGHTS)
static bool send_fd(int sock, int fd_to_send) {
    struct msghdr msg{};
    struct iovec iov;
    char dummy = 0;
    iov.iov_base = &dummy;
    iov.iov_len = 1;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    char cbuf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = cbuf;
    msg.msg_controllen = sizeof(cbuf);

    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    memcpy(CMSG_DATA(cmsg), &fd_to_send, sizeof(int));

    ssize_t n = sendmsg(sock, &msg, 0);
    return (n == 1);
}

int main(int argc, char** argv) {
    const char* socket_path = (argc > 1) ? argv[1] : "/tmp/spsc_multi.sock";
    const char* mem_name = "producer_spsc_memfd";
    size_t total = sizeof(SPSCBuffer);

    // 1) create memfd
    int fd = memfd_create_wrapper(mem_name, MFD_CLOEXEC);
    if (fd < 0) { perror("memfd_create"); return 1; }

    if (ftruncate(fd, (off_t)total) == -1) { perror("ftruncate"); close(fd); return 1; }

    void* base = mmap(nullptr, total, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (base == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    SPSCBuffer* buf = reinterpret_cast<SPSCBuffer*>(base);

    // initialize meta and slots
    buf->meta.head.store(0, std::memory_order_relaxed);
    buf->meta.tail.store(0, std::memory_order_relaxed);
    buf->meta.capacity = RING_SIZE;
    buf->meta.slot_bytes = SLOT_DATA;
    for (size_t i = 0; i < RING_SIZE; ++i) {
        buf->slots[i].seq.store(i, std::memory_order_relaxed);
        buf->slots[i].len = 0;
    }

    // connect to consumer daemon
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); munmap(base, total); close(fd); return 1; }

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(s); munmap(base, total); close(fd);
        return 1;
    }

    // send memfd to consumer
    if (!send_fd(s, fd)) {
        std::cerr << "producer: send_fd failed\n";
        close(s); munmap(base, total); close(fd);
        return 1;
    }
    std::cout << "producer: sent memfd to consumer, fd=" << fd << "\n";
    close(s); // socket no longer needed

    // produce logs
    auto enqueue = [&](const char* data, size_t len) -> bool {
        if (len == 0 || len > SLOT_DATA) return false;
        uint64_t ticket = buf->meta.tail.fetch_add(1, std::memory_order_relaxed);
        size_t idx = static_cast<size_t>(ticket % buf->meta.capacity);
        SlotHeader* sslot = &buf->slots[idx];

        // wait until slot free: seq == ticket
        for (;;) {
            uint64_t seq = sslot->seq.load(std::memory_order_acquire);
            if (seq == ticket) break;
#if defined(__x86_64__) || defined(__i386__)
            __asm__ __volatile__("pause");
#else
            std::this_thread::yield();
#endif
        }

        sslot->len = static_cast<uint32_t>(len);
        memcpy(sslot->data, data, len);

        // publish
        sslot->seq.store(ticket + 1, std::memory_order_release);
        return true;
    };

    for (int i = 0; i < 100000; ++i) {
        std::string msg = "pid=" + std::to_string(getpid()) + " log#" + std::to_string(i);
        enqueue(msg.c_str(), msg.size());
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // keep memory alive briefly to allow consumer to finish
    std::cout << "producer: done producing; sleeping 5s then exit\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));

    munmap(base, total);
    close(fd);
    return 0;
}

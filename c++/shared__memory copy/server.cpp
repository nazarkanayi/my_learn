#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <ctime>
#include "shared_memory.h"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(SharedMemory)) == -1) {
        perror("ftruncate");
        return 1;
    }

    SharedMemory *shm = (SharedMemory *)mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    //SharedMemory* shm = new (shm_fresh) SharedMemory();

    memset(shm->data, 0, BUFFER_SIZE);
        shm->read_index.store(0, std::memory_order_seq_cst);
        shm->write_index.store(0, std::memory_order_seq_cst);
        

        pthread_condattr_t cond_attr;
        pthread_condattr_init(&cond_attr);
        pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&shm->cond_var, &cond_attr);
        pthread_condattr_destroy(&cond_attr);

        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&shm->mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);

    shm->server_ready.store(true, std::memory_order_seq_cst);
    std::cout << "Server is waiting for messages...\n";

    while (true) {
        pthread_mutex_lock(&shm->mutex);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 5;  // Timeout after 5 seconds

        while (shm->read_index.load(std::memory_order_relaxed) == shm->write_index.load(std::memory_order_acquire)) {
            if (pthread_cond_timedwait(&shm->cond_var, &shm->mutex, &ts) == ETIMEDOUT) {
                //std::cout << "No messages received in 5 seconds. Waiting...\n";
                pthread_mutex_unlock(&shm->mutex);
                continue;
            }
        }

        int read_pos = shm->read_index.load(std::memory_order_relaxed);
        std::cout << "read start" << read_pos << std::endl;
        char message[BUFFER_SIZE];
        strncpy(message, &shm->data[read_pos], BUFFER_SIZE);
        message[BUFFER_SIZE - 1] = '\0';

        std::cout << "Client: " << message << std::endl;

        if (strcmp(message, "exit") == 0) {
            pthread_mutex_unlock(&shm->mutex);
            break;
        }

        shm->read_index.store((read_pos + strlen(message) + 1) % BUFFER_SIZE, std::memory_order_release);
        //read_pos = shm->read_index.load(std::memory_order_seq_cst);
        ///std::cout << "read end" << read_pos << std::endl;
        pthread_mutex_unlock(&shm->mutex);
        pthread_cond_signal(&shm->cond_var);
    }

    munmap(shm, sizeof(SharedMemory));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    std::cout << "Server shutting down.\n";
    return 0;
}

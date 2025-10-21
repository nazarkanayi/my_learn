#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include "shared_memory.h"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    SharedMemory *shm = (SharedMemory *)mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    while (!shm->server_ready.load(std::memory_order_seq_cst)) {
        usleep(1000);
    }

    std::cout << "Client connected. Type messages (type 'exit' to quit):\n";

    while (true) {
        std::cout << "You: ";
        std::string message;
        std::getline(std::cin, message);

        pthread_mutex_lock(&shm->mutex);

        while (((shm->write_index.load(std::memory_order_seq_cst) + message.size() + 1) % BUFFER_SIZE) ==
                shm->read_index.load(std::memory_order_seq_cst)) {
            pthread_cond_wait(&shm->cond_var, &shm->mutex);
        }

        int write_pos = shm->write_index.load(std::memory_order_seq_cst);
        strncpy(&shm->data[write_pos], message.c_str(), BUFFER_SIZE);
        shm->data[write_pos + message.size()] = '\0';  

        shm->write_index.store((write_pos + message.size() + 1) % BUFFER_SIZE, std::memory_order_seq_cst);

        pthread_mutex_unlock(&shm->mutex);
        pthread_cond_signal(&shm->cond_var);

        if (message == "exit")
            break;
    }

    munmap(shm, sizeof(SharedMemory));
    close(shm_fd);

    std::cout << "Client disconnected.\n";
    return 0;
}

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include "shared_memory.h"
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>

int main() {
    sem_t *sem = sem_open(SEM_NAME,0);
    
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    // Immediately unlink to prevent lingering semaphore
    if (sem_unlink(SEM_NAME) == -1) {
        perror("sem_unlink failed");
        sem_close(sem);
        exit(EXIT_FAILURE);
    }

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    
    void *shm = mmap(0, TOTAL_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    // SharedMemory* shm = new (shm_fresh) SharedMemory();

    if (((std::uint64_t)shm & 0x7) != 0U)
    {
        std::cout << "Memory alignment error \n";
    }

    std::atomic<std::uint32_t> *read_index;
    std::atomic<std::uint32_t> *write_index;
    std::uint8_t *buffer;

    read_index = reinterpret_cast<std::atomic<std::uint32_t> *>(shm);

    write_index = reinterpret_cast<std::atomic<std::uint32_t> *>((uint8_t *)shm + 8U);

    buffer = (uint8_t *)shm + 16U;

    SharedMemory circular_buffer = SharedMemory(buffer, read_index, write_index);
    std::cout << "Client connected. Type messages (type 'exit' to quit):\n";

    while (true) {
        std::cout << "You: ";
        std::string message;
        std::getline(std::cin, message);

        circular_buffer.Write((uint8_t *)message.c_str(), message.length());

        if (sem_post(sem) == -1) {
            perror("sem_post failed");
            exit(EXIT_FAILURE);
        }
      
        if (message == "exit")
            break;
    }

    munmap(shm, TOTAL_MEMORY);
    close(shm_fd);
    sem_close(sem);

    std::cout << "Client disconnected.\n";
    return 0;
}

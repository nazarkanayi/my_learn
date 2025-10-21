#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include "shared_memory.h"


bool Is_full(int read_index , int write_index, int size)
{
    return (((write_index+ size + 1) % BUFFER_SIZE) ==
                read_index);
}

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

     pthread_condattr_t cond_attr;
        pthread_condattr_init(&cond_attr);
        pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&shm->cond_var, &cond_attr);
        //pthread_condattr_destroy(&cond_attr);

        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&shm->mutex, &mutex_attr);

    while (!shm->server_ready) {
        usleep(1000);
    }

    std::cout << "Client connected. Type messages (type 'exit' to quit):\n";

    while (true) {
        std::cout << "You: ";
        std::string message;
        std::getline(std::cin, message);

        pthread_mutex_lock(&shm->mutex);

        while (Is_full(shm->read_index,shm->write_index,message.size())) {
            pthread_cond_wait(&shm->cond_var, &shm->mutex);
        }

        int write_pos = shm->write_index;
        strncpy(&shm->data[write_pos], message.c_str(), BUFFER_SIZE);
        shm->data[write_pos + message.size()] = '\0';  

        shm->write_index = (write_pos + message.size() + 1) % BUFFER_SIZE;
        std::cout << "read start" << shm->read_index << std::endl;
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

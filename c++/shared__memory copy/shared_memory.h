#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <atomic>
#include <pthread.h>
#include <chrono>

#define SHM_NAME "/shm_circular_buffer"
#define BUFFER_SIZE 1024

struct SharedMemory {
    char data[BUFFER_SIZE];  
    std::atomic<int> read_index {0};   
    std::atomic<int> write_index {0};  
    std::atomic<bool> server_ready {false};  

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
};

#endif 

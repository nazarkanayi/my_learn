#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <atomic>
#include <pthread.h>
#include <chrono>

#define SHM_NAME "/shm_circular_buffer"
#define BUFFER_SIZE 1024

struct SharedMemory {
    char data[BUFFER_SIZE];  
    
    volatile int server_ready {0};  

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
    volatile  int read_index {0};   
    volatile int write_index {0};  
}__attribute__ ((aligned(64)));

#endif 

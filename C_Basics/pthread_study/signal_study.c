


#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

// Signal handler that will execute in the target thread
void signal_handler(int sig) {
    printf("Thread %lu received signal %d\n", (unsigned long)pthread_self(), sig);
}

void* worker_thread(void* arg) {
    // Register the handler for SIGUSR1
    signal(SIGTERM, signal_handler);
    
    printf("Worker thread waiting for signal...\n");
    while(1) {
        pause(); // Wait for a signal to arrive
    }
    return NULL;
}

int main() {
    pthread_t tid;
    
    pthread_create(&tid, NULL, worker_thread, NULL);
    sleep(1); // Give thread time to start

    // Send SIGUSR1 to the specific thread
    printf("Main: Sending SIGUSR1 to thread %lu\n", (unsigned long)tid);
    int status = pthread_kill(tid, SIGTERM);

    if (status != 0) {
        fprintf(stderr, "Failed to send signal\n");
    }

    pthread_join(tid, NULL);
    return 0;
}

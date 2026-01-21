#include <stdio.h>
#include <pthread.h>
#include <signal.h>

void* signal_manager(void* arg) {
    sigset_t *set = (sigset_t*)arg;
    int sig;

    printf("Signal manager thread waiting for signals...\n");
    while (1) {
        // Blocks until one of the signals in 'set' arrives
        if (sigwait(set, &sig) == 0) {
            printf("\nCaptured signal: %d\n", sig);
            if (sig == SIGINT) {
                printf("Gracefully shutting down program...\n");
                break; 
            }
        }
    }
    return NULL;
}

int main() {
    pthread_t thread;
    sigset_t set;

    // 1. Define the signals we want to handle
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);

    // 2. IMPORTANT: Block these signals in the main thread
    // All subsequently created threads will inherit this mask
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    // 3. Create a dedicated thread to handle signals
    pthread_create(&thread, NULL, signal_manager, (void*)&set);

    printf("Main thread running. Press Ctrl+C to test sigwait.\n");

    // Main thread remains free to do other work
    pthread_join(thread, NULL);
    
    return 0;
}

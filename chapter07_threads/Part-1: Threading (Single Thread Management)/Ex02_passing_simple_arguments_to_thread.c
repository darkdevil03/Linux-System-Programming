/**
    Scenario:
        How do we pass a single integer argument (like a thread ID) to a newly created thread?

    Details:
        pthread_create accepts a void* argument. We pass the memory address of my_id,
        and inside the thread, we cast the void* back to an int* and dereference it.

    Usage:
        Passing loop indexes, file descriptors, or configuration IDs to worker threads.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_func(void* arg) {
    int thread_num = *((int*) arg);

    for (int i = 0; i < 10; i++) {
        sleep(1); // Simulate work
        printf("[THREAD-%d] I am running in background...\n", thread_num);
    }
    printf("[THREAD-%d] My work done!\n",thread_num);

    return NULL;
}

int main() {

    printf("[MAIN-THREAD] I am started\n");
    pthread_t thread;
    int thread_num = 1952;

    printf("[MAIN-THREAD] Thread Created!!\n");
    // Create the thread and pass the address of my_id
    pthread_create(&thread, nullptr, thread_func, &thread_num);

    // Wait for the background thread to finish
    pthread_join(thread, nullptr);

    printf("[MAIN-THREAD] Background thread done. I am exiting...\n");

    return EXIT_SUCCESS;
}

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
    unsigned long thread_id = *((unsigned long*) arg);

    for (int i = 0; i < 10; i++) {
        sleep(1); // Simulate work
        printf("[THREAD-%lu] I am running in background...\n", thread_id);
    }
    printf("[THREAD-%lu] My work done!\n",thread_id);

    return NULL;
}

int main() {

    pthread_t thread;
    pthread_t thread_id = pthread_self();
    pthread_create(&thread, nullptr, thread_func, &thread_id);

    pthread_join(thread, nullptr);

    return EXIT_SUCCESS;
}

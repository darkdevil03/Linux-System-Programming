/**
    Scenario: (Concepts: Basic Thread Creation and Joining)
        How do you create a background thread to print a message without blocking the main program
        immediately, and then wait for it to finish?

    Details:
        pthread_create spawns a new thread executing print_message.
        pthread_join blocks the main thread until the spawned thread exits,
        ensuring the program doesn't terminate prematurely.

    Usage:
        Used for basic asynchronous tasks like background logging,
        saving a file, or non-blocking UI updates.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* print(void* args) {

    for (int i = 0; i < 10; i++) {
        sleep(1); // Simulate work
        printf("[THREAD-1] I am running...\n");
    }
    printf("[THREAD-1] My work done!\n");

    return NULL;
}

int main() {

    printf("[MAIN-THREAD] I am started\n");

    pthread_t thread1;
    // Create the thread
    printf("[MAIN-THREAD] Thread Created!!\n");
    pthread_create(&thread1,nullptr,print,nullptr);

    // Wait for the background thread to finish
    pthread_join(thread1,nullptr);

    printf("[MAIN-THREAD] Background thread done. I am exiting...\n");

    return EXIT_SUCCESS;
}

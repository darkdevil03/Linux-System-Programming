/**
    Scenario: Thread Signaling (Semaphores)
        A system has a "downloader" thread and a "processor" thread.
        How do you ensure the processor only runs after the downloader
        has fetched a file?

    Details:
        A semaphore is a thread-safe integer.
        sem_wait pauses the thread if the value is 0. sem_post increments it.
        The processor waits because the starting value is 0,
        until the downloader posts.

    Usage:
        Controlling order of execution, limiting access to a pool of resources
        (like a database connection pool limit of 5).
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t ready_sem;

void* downloader(void* arg) {
    printf("Downloading file...\n");
    sleep(3);
    printf("Download completed. Signaling processor.\n");
    sem_post(&ready_sem); // Increment semaphore
    return NULL;
}

void* processor(void* arg) {
    printf("Processor waiting for file...\n");
    sem_wait(&ready_sem); // Decrement semaphore (blocks if 0)
    printf("Processing the file now.\n");
    return NULL;
}


int main() {

    sem_init(&ready_sem, 0, 0);

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, processor, NULL);
    pthread_create(&thread2, NULL, downloader, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&ready_sem);

    return EXIT_SUCCESS;
}
/**
    Scenario: Protecting Shared Data (Mutex)
        Two threads are incrementing a shared counter variable 100,000 times.
        How do you prevent race conditions from corrupting the final count?

    Details:
        global_counter++ is not atomic. The Mutex acts as a lock.
        Thread 1 locks it, modifies the counter, and unlocks it.
        Thread 2 must wait in line, preventing data corruption.

    Usage:
        Updating shared states like global variables, database rows,
        or linked lists from multiple threads
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int global_counter = 0;
pthread_mutex_t lock;

void* counter(void* args) {
    int thread_id = *(int*) args;

    for (int i = 0; i < 100000000; i++) {
        pthread_mutex_lock(&lock);
        // printf("Locked by thread : %d\n", thread_id);
        global_counter++;
        pthread_mutex_unlock(&lock);
        // printf("Unlocked by thread : %d\n", thread_id);
    }

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_t t1_id = 1;
    pthread_t t2_id = 2;

    pthread_mutex_init(&lock, nullptr);

    pthread_create(&thread1, nullptr, counter, &t1_id);
    pthread_create(&thread2, nullptr, counter, &t2_id);

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    printf("Final shared variable value : %d", global_counter);

    pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}

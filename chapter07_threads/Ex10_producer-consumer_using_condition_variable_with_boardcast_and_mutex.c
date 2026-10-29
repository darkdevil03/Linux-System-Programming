#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int keep_running = 1; // 1 means run, 0 means shut down
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* worker(void* arg) {
    int id = *((int*)arg);

    pthread_mutex_lock(&lock);

    // All workers wait here
    while (keep_running == 1) {
        printf("Worker %d going to sleep...\n", id);
        pthread_cond_wait(&cond, &lock);
    }

    // When they break the loop, it means keep_running is 0
    printf("Worker %d received shutdown signal. Exiting.\n", id);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};

    // Start 3 workers
    for(int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    sleep(2); // Let them all go to sleep

    // Initiate shutdown
    printf("\nMain: Initiating global shutdown...\n");
    pthread_mutex_lock(&lock);
    keep_running = 0; // Change the state
    pthread_mutex_unlock(&lock); // Open the door

    // WAKE EVERYONE UP AT ONCE
    pthread_cond_broadcast(&cond);

    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main: All workers shut down cleanly.\n");
    return 0;
}
/**
    Scenario: A web server handling 100 requests simultaneously,
              but the backend Database can only handle 3 connections at a time.
              The semaphore acts as a bouncer.
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t db_connections;

void* handle_request(void* arg) {
    int id = *((int*)arg);

    printf("[THREAD-%d] Request connecting to DB...\n", id);
    sem_wait(&db_connections); // Wait for a free slot

    printf("[THREAD-%d] Got active DB connection\n", id);
    sleep(5); // Simulate DB query
    printf("[THREAD-%d] Request finished.\n", id);

    sem_post(&db_connections); // Release the slot
    return NULL;
}

int main() {
    sem_init(&db_connections, 0, 3); // Max 3 concurrent threads allowed
    pthread_t threads[10] = {0};
    int thread_ids[10] = {0};

    for(int i = 0; i<10; i++) {
        pthread_create(&threads[i], NULL, handle_request, &thread_ids[i]);
    }

    for(int i=0; i<10; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&db_connections);

    return EXIT_SUCCESS;
}

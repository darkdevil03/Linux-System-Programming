#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int buffer = 0; // 0 means empty, 1 means full
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    pthread_mutex_lock(&lock);
    while (buffer == 1) { // Wait if full
        pthread_cond_wait(&cond, &lock);
    }
    buffer = 1; // Produce
    printf("Produced task.\n");
    pthread_cond_signal(&cond); // Wake up consumer
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* consumer(void* arg) {
    pthread_mutex_lock(&lock);
    while (buffer == 0) { // Wait if empty
        pthread_cond_wait(&cond, &lock); // Releases lock and sleeps
    }
    buffer = 0; // Consume
    printf("Consumed task.\n");
    pthread_cond_signal(&cond); // Wake up producer
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t p, c;
    pthread_create(&c, NULL, consumer, NULL); // Starts first, goes to sleep
    sleep(1);
    pthread_create(&p, NULL, producer, NULL); // Wakes the consumer
    
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}
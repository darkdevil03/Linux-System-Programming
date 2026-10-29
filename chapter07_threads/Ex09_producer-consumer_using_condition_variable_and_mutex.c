
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/pthreadtypes.h>


int buffer = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* consumer(void* args) {
    pthread_mutex_lock(&mutex);
    if(buffer == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    buffer = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    printf("[CONSUMER] Given task completed. \n");

    return NULL;
}

void* producer(void* args) {
    pthread_mutex_lock(&mutex);
    if(buffer ==  1) {
        pthread_cond_wait(&cond, &mutex);
    }

    buffer = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    printf("[PRODUCER] Generated a task into buffer.\n");


    return NULL;
}

int main() {

    pthread_t produce, consume;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&produce, NULL, producer, NULL);
    pthread_create(&consume, NULL, consumer, NULL);

    pthread_join(produce, NULL);
    pthread_join(consume, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}

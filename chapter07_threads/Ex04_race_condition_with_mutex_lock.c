
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 0;
int check = 0;


void* counter(void* args) {


    for (int i = 0; i < 10000000; i++) {
        pthread_mutex_lock(args);
        glob++;
        pthread_mutex_unlock(args);
    }

    return NULL;
}

int main() {

    pthread_t thread;
    pthread_t thread2;

    pthread_create(&thread, nullptr, counter, &check);
    pthread_create(&thread2, nullptr, counter, &check);

    pthread_join(thread, nullptr);
    pthread_join(thread2, nullptr);

    printf("glob : %d", glob);

    return EXIT_SUCCESS;
}

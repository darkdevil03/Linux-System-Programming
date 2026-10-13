#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int glob = 0;

void* counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        glob++;
    }
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_t thread2;


    pthread_create(&thread,nullptr,counter,nullptr);
    pthread_create(&thread2,nullptr,counter,nullptr);


    pthread_join(thread,nullptr);
    pthread_join(thread2,nullptr);


    printf("glob : %d", glob);

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global_counter = 0;

void* counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        global_counter++;
    }
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_t thread2;


    pthread_create(&thread,NULL,counter,NULL);
    pthread_create(&thread2,NULL,counter,NULL);


    pthread_join(thread,NULL);
    pthread_join(thread2,NULL);


    printf("glob : %d", global_counter);

    return EXIT_SUCCESS;
}
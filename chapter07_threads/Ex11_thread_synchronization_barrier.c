#include <pthread.h>
#include <stdio.h>

pthread_barrier_t barr;
int gate = 0;

void* func(void* arg) {

    if (gate == 0) {
        printf("[BARRIER-UP] Threads to join for barrier to down\n");
    }

    printf("[THREAD-%d] Waiting for thread to join.\n", *(int*) arg);
    pthread_barrier_wait(&barr);

    printf("[BARRIER-DOWN] ");
    return NULL;
}

int main() {

    pthread_barrier_init(&barr, NULL, 5);

    for (int i = 0; i < 5; i++) {
        pthread_create(,)

    }


    pthread_barrier_destroy(&barr);

    return 0;
}

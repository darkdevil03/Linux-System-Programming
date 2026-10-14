#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* print(void* args) {

    for (int i = 0; i < 10; i++) {
        printf("[THREAD-1] I am running...\n");
    }

    return NULL;
}

int main() {

    printf("[MAIN] I am started\n");
    printf("[MAIN] Thread Created!!\n");

    pthread_t thread1;
    pthread_create(&thread1,nullptr,print,nullptr);
    pthread_join(thread1,nullptr);

    printf("[THREAD-1] My work done!\n");
    printf("[MAIN] I am exiting...\n");

    return EXIT_SUCCESS;
}

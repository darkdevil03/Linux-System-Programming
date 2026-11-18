
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* printer(void* arg) {
    int thread_num = *(int*)arg;

    for (int i = 0; i < 1000000; i++) {
        printf("[THREAD-%d] I had accessed printer\n", thread_num);
    }
    return NULL;
}

int main() {

    printf("[MAIN] I am starting...\n");
    pthread_t thread1, thread2;
    int thread1_token = 1;
    int thread2_token = 2;

    printf("[THREAD-1] I was created\n");
    pthread_create(&thread1,NULL,printer, &thread1_token);
    printf("[THREAD-2] I was created\n");
    pthread_create(&thread2,NULL,printer, &thread2_token);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    printf("[THREAD-1] I was destroyed\n");
    printf("[THREAD-2] I was destroyed\n");
    printf("[MAIN] I am exiting...\n");

    return EXIT_SUCCESS;
}

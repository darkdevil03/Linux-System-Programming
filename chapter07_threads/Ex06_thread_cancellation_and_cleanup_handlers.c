
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>

void cleanup_handler(void* arg) {
    printf("Cleanup: Freeing memory and closing files.\n");
    free(arg);
}

void* long_running_task(void* arg) {
    int* data = malloc(sizeof(int));
    // Push a cleanup function to the stack
    pthread_cleanup_push(cleanup_handler, data);

    while(1) {
        printf("Thread working...\n");
        sleep(1);
        pthread_testcancel(); // Safe point to allow cancellation
    }

    // Pop the cleanup function (0 means don't execute it normally)
    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, long_running_task, NULL);

    sleep(5);
    printf("Main: Canceling thread...\n");
    pthread_cancel(thread); // Send cancellation request
    pthread_join(thread, NULL);

    return 0;
}

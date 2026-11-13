/**
    Scenario:
        How do you forcefully cancel a running thread from the main thread,
        while ensuring the thread cleans up its locked resources
        (like closing files) before dying?

    Details:
        pthread_cancel requests termination.
        pthread_cleanup_push registers a callback that the OS automatically executes
        if the thread is cancelled, preventing memory/file descriptor leaks.

    Usage:
        Daemon threads, background telemetry heartbeat pingers, or
        server client-handling threads where tracking them is unnecessary.

    Note:
        Requires understanding cancellation points,
        asynchronous vs deferred cancellation, and stack-based cleanup macros.
*/
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
    pthread_create(&thread, nullptr, long_running_task, NULL);

    sleep(5);
    printf("Main: Canceling thread...\n");
    pthread_cancel(thread); // Send cancellation request
    pthread_join(thread, nullptr);

    return 0;
}

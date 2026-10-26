/**
    Scenario: (Detached Threads)
        How do you run a "fire-and-forget" thread that cleans up its own resources
        without the main thread ever calling pthread_join?

    Details:
        Normally, a thread's resources are kept alive until join
        is called (like a zombie process).
        pthread_detach tells the OS to automatically reclaim the
        thread's memory the moment it exits.

    Usage:
        Daemon threads, background telemetry heartbeat pingers, or
        server client-handling threads where tracking them is unnecessary.
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* back_ground_task(void* args) {

    int id = *(int *)args;

    printf("[BACKGROUND-%d] Thread started, work in progress... \n",id);
    sleep(15);
    printf("[BACKGROUND-%d] Thread ended, resource reclaim by OS. \n]",id);

    return NULL;
}

int main() {

    printf("[MAIN] I started to trigger the back ground task!! \n");
    pthread_t thread;
    int thread_num = 12;

    pthread_create(&thread, nullptr, back_ground_task, &thread_num);

    pthread_detach(thread);

    printf("[MAIN] Background task trigger has done!!\n");

    return EXIT_SUCCESS;
}


// #define _GNU_SOURCE     /* Required for certain Linux-specific features */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* For syscall() */
#include <sys/syscall.h> /* For SYS_gettid */

void* thread_function(void* arg) {
    int thread_num = *(int*)arg;

    // Get the Pthread ID (User-space handle)
    pthread_t p_id = pthread_self();

    // Get the Kernel Thread ID (System-wide TID)
    // We use syscall(SYS_gettid) because older glibc versions
    // don't expose a direct gettid() function wrapper
    long k_id = syscall(SYS_gettid);

    // Print both. Notice that %lu is used for pthread_t
    // because it's often a long unsigned int/pointer,
    // while pid_t is a standard integer (%d).
    printf("|--> [Thread-%d] Pthread ID: %lu | Kernel TID: %lu \n",
        thread_num, (unsigned long)p_id, k_id);


    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread1_id1 = 1, thread2_id2 = 2;

    printf("Main Process PID: %d\n", getpid());
    printf("----------------------------------------------------------------------\n");

    // Create two threads
    pthread_create(&thread1, nullptr, thread_function, &thread1_id1);
    pthread_create(&thread2, nullptr, thread_function, &thread2_id2);

    // Wait for both threads to finish
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    printf("----------------------------------------------------------------------\n");
    printf("Different thread ids (User-Space and Kernel-Space) info printing finished!!\n");
    printf("Main process exiting....\n");

    return 0;
}
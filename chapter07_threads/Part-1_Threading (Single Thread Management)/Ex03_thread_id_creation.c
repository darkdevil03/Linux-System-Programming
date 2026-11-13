/**
    Scenario:
       To print thread id from user-space and kernel-space case

    Details:
        To use both pthread_self() and gettid() together,
        we need to include the correct headers.
        Because gettid() is a Linux-specific system call,
        we use the standard <sys/syscall.h> and
        syscall(SYS_gettid) wrapper to ensure it compiles smoothly.

    Usage: (main purpose)
        pthread_self():
            Used to manage threads using library functions
            (e.g., passing it to pthread_join or pthread_detach).
        gettid():
            Used for system-level tasks like debugging (e.g., htop, gdb),
            setting CPU affinity, or system logging.

 */

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

/**
                    Difference between pthread_self() and gettid()?

    Feature	                     pthread_self()	                           Kernel gettid() (Linux)

Who creates it?	        The Pthread library (User-space code).	    The Operating System Kernel (System-level).

What is it?	            A pointer to a data structure or            A unique integer representing the thread
                        an arbitrary number representing the        globally across the entire operating system.
                        thread inside your program.

Scope of Uniqueness	    Only unique inside the current process.     Globally unique across the whole computer.
                        Two threads in different programs could     No two threads on the system share the same TID.
                        have the same value.

Portability	            Highly Portable.                            Linux-Specific.
                        Works on Linux, macOS, FreeBSD, and         It is a system call specific to the Linux kernel.
                        any POSIX-compliant system.

 */
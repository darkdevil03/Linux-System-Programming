/**
    CRITICAL FIX 1: Feature Test Macro
         vfork() is a non-ISO standard function. To make the GNU C Library (glibc)
         declare vfork(), we must define _GNU_SOURCE or _DEFAULT_SOURCE BEFORE
         including any header files.
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> // CRITICAL FIX 2: Required for pid_t, vfork(), etc.

int main() {
    int x = 10;
    printf("Parent process running...\n");
    printf("At initial parent process, x = %d\n", x);

    /*
     * vfork() creates a child process.
     * NOTE: Modifying variables like 'x' inside a vfork child before
     * calling exec() or _exit() results in undefined behavior per POSIX,
     * because the child shares the parent's memory space directly.
     */
    const pid_t pid = vfork();

    if (pid < 0) {
        perror("vfork failed");
        exit(1);
    }

    if (pid == 0) {
        printf("Child process created!\n");
        x = 30;
        printf("Child process changed x to 30\n");

        /*
            CRITICAL RULE FOR VFORK:
            The child must terminate using _exit(), not exit().
            Using exit() flushes standard I/O streams and can corrupt the parent's buffers.
         */
        _exit(0);
    }

    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        const int child_terminated = WEXITSTATUS(status);
        printf("Child process terminated with code %d\n", child_terminated);
    }

    /*
        Because vfork() shares memory, the change made by the child to 'x'
        will be visible here in the parent process!
     */
    printf("After child exited at Parent process, x = %d\n", x);

    return 0;
}
/**
    Scenario:
        Demonstration of the fork-exec-wait pattern using execv().

    Details:
        This program creates a child process that replaces its execution image with the '/bin/ls -lR' command using execv().
        The parent process synchronizes with the child using wait() and analyzes its exit status.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("[Parent] Parent process created...\n");
    printf("[Parent] Parent process ID: %d\n", getpid());

    /* * Prepare the argument vector for execv.
     * argv[0] is conventionally the command name itself.
     * argv[1] is the flag "-l".
     * argv[2] MUST be NULL to terminate the array for execv().
     */
    char *argv[] = {"/bin/ls", "-lR", nullptr};

    // 1. Create a duplicate process
    const pid_t child = fork();

    // 2. Error Handling for fork()
    if (child < 0) {
        printf("[ERROR] FATAL: Failed to fork child process.\n");
        return -1;
    }

    // 3. Child Process Execution Flow
    if (child == 0) {
        printf("[Child] Child process created...\n");
        printf("[Child] Process ID: %d\n", getpid());
        printf("[Child] Preparing to replace process image with ls program '/bin/ls -lR'...\n\n");

        // execv takes the absolute path to the binary and the NULL-terminated argument array
        execv(argv[0], argv);

        /* * --- CRITICAL CONCEPT ---
         * If execv() is successful, the current process image is completely 
         * overwritten by the 'ls' program. The lines below will NEVER execute 
         * unless execv() fails (e.g., incorrect path).
         */
        perror("[Child ERROR] execv() failed");
        
        // Exit with status 1 so the parent knows the exec failed
        exit(1); 
    }

    // 4. Parent Process Execution Flow
    int status;
    printf("[Parent] Waiting for child (PID: %d) to complete...\n", child);

    // wait() blocks the parent until the child terminates
    pid_t terminated_process_id = wait(&status);
    printf("\n[Parent] Child process (PID: %d) has finished executing.\n", terminated_process_id);

    // 5. Safely analyze how the child process terminated
    if (WIFEXITED(status)) {
        // Extract the actual exit code returned by the child process
        const int exit_status = WEXITSTATUS(status);

        printf("[Parent] Child's replaced ls-program terminated normally with exit status: %d\n", exit_status);

        // If 'ls' succeeds, exit_status will be 0.
        // If it is 1, our error handling caught an execv() failure.
        if (exit_status == 1) {
            printf("[Parent] Warning: Exit status 1 detected. The execv() call likely failed.\n");
        }
    } else {
        printf("[Parent] Warning: Child terminated abnormally (e.g., killed by a signal).\n");
    }

    printf("[Parent] Parent process is terminating safely.\n");

    return 0;
}
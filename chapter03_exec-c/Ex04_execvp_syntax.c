/**
    Scenario:
        Demonstration of the fork-exec-wait pattern using execvp().

    Details:
        This program creates a child process that replaces its execution image with the 'cat Ex00_new_program.c' command using execvp().
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

    /* * Prepare the argument vector for execvp.
     * argv[0] is conventionally the command name itself ("cat").
     * argv[1] is the target file ("Ex00_new_program.c").
     * argv[2] MUST be NULL to terminate the array.
     */
    char *argv[] = {"cat", "Ex00_new_program.c", nullptr};

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
        printf("[Child] Process ID: %d\n\n", getpid());
        
        // execvp takes the command name (searches $PATH) and the NULL-terminated array
        execvp(argv[0], argv);

        /* * --- CRITICAL CONCEPT ---
         * If execvp() is successful, the current process image is completely 
         * overwritten by the 'cat' program. The lines below will NEVER execute 
         * unless execvp() fails (e.g., command not found).
         */
        perror("[Child ERROR] execvp() failed");
        
        // Exit with status 5 so the parent knows the exec failed
        exit(5); 
    }

    // 4. Parent Process Execution Flow
    int status;

    // wait() blocks the parent until the child terminates
    pid_t terminated_process_id = wait(&status);

    // 5. Safely analyze how the child process terminated
    if (WIFEXITED(status)) {
        // Extract the actual exit code returned by the child process
        const int exit_status = WEXITSTATUS(status);

        printf("\n[Parent] Child terminated normally with exit status: %d\n", exit_status);

        // If 'cat' succeeds, exit_status will generally be 0.
        // If it is 5, our error handling caught an execvp() failure.
        if (exit_status == 5) {
            printf("[Parent] Warning: Exit status 5 detected. The execvp() call likely failed.\n");
        }
    } else {
        printf("[Parent] Warning: Child terminated abnormally (e.g., killed by a signal).\n");
    }

    printf("[Parent] Parent process is terminating safely.\n");


    return EXIT_SUCCESS;
}
/**
     Scenario:
        Demonstration of the fork-exec-wait pattern in Linux.

    Details:
          This program creates a child process that replaces its execution
          image with the 'ls -il' command using execlp(). The parent process
          synchronizes with the child using wait() and analyzes its exit status.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // 1. Create a child/duplicate process
    pid_t pid = fork();

    // 2. Error Handling for fork()
    if (pid < 0) {
        perror("FATAL: fork() failed to create a new process");
        exit(EXIT_FAILURE);
    }

    // 3. Child Process Execution Flow
    if (pid == 0) {
        printf("[Child] Process ID (PID) is: %d\n", getpid());
        printf("[Child] Preparing to replace process image with 'ls -il'...\n\n");

        /* * execlp takes:
         * 1st arg: The command to execute ("ls")
         * 2nd arg: arg[0] (conventionally the command name itself)
         * 3rd arg: arg[1] (the flags "-il")
         * Nth arg: NULL/nullptr/0 (Signals the end of the argument list)
         */
        execlp("ls", "ls", "-il", nullptr);

        /* * --- CRITICAL CONCEPT ---
         * If execlp() is successful, the current process image is completely 
         * overwritten by the 'ls' program. Therefore, the lines below will 
         * NEVER execute unless execlp() fails (e.g., command not found).
         */
        perror("[Child ERROR] execlp() failed");
        
        // We exit with a specific code (5) so the parent knows the exec failed.
        exit(5); 
    }
    
    // 4. Parent Process Execution Flow

    int status;
    printf("[Parent] Process ID is: %d. Waiting for child (PID: %d) to complete...\n", getpid(), pid);

    // wait() blocks the parent until the child terminates
    pid_t terminated_pid = wait(&status);

    printf("\n[Parent] Child process (PID: %d) has finished executing.\n", terminated_pid);

    // 5. Safely analyze how the child process terminated
    if (WIFEXITED(status)) {
        // Extract the actual exit code returned by the child process
        int exit_status = WEXITSTATUS(status);

        printf("[Parent] Child terminated normally with exit status: %d\n", exit_status);

        // If 'ls' succeeds, exit_status will be 0.
        // If it is 5, our error handling caught an execlp() failure.
        if (exit_status == 5) {
            printf("[Parent] Warning: Exit status 5 detected. The execlp() call likely failed.\n");
        }
    } else {
        printf("[Parent] Warning: Child terminated abnormally (e.g., killed by a signal).\n");
    }

    printf("[Parent] Parent process is terminating safely.\n");


    return 0;
}
/**
Scenario:
    Demonstration of dynamically executing command-line arguments using execvp.

Details:
    This program takes commands directly from the terminal execution (e.g., ./program ls -la)
    and safely shifts the argument vector to isolate the target command from the shell executable.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int size, char *args[]) {
    // 1. Validate that the user actually provided a command to run
    if (size < 2) {
        printf("[Shell] Usage: %s <command> [args...]\n", args[0]);
        return EXIT_FAILURE;
    }

    printf("myshell> ");
    for (int i = 1; i < size; i++) {
        printf("%s ",args[i]);
    }
    printf("\n");
    
    // 2. Create a duplicate process refers myshell
    pid_t id = fork();

    if (id < 0) {
        perror("[ERROR] Failed to fork child process.");
        return EXIT_FAILURE;
    }

    // 3. Child Process Execution Flow
    if (id == 0) {
        /*
         * args[1]: The command to execute ("ls")
         * &args[1]: The shifted argument array starting at "ls".
         * The OS automatically ensures a NULL pointer exists at the end of 'args'.
         */
        execvp(args[1], &args[1]);

        /*
         * --- CRITICAL CONCEPT ---
         * If execvp() is successful, the myshell process is completely replaced by args[1] such ls.
         * The lines below ONLY execute if execvp() fails (e.g., typo in command).
         */
        printf("[%s ERROR] execvp() failed",args[1]);
        exit(5);
    }
    else {
        // 4. Parent Process Execution Flow: treat as terminal program
        int status;

        // wait() blocks the parent until the child terminates
        pid_t terminated_child = wait(&status);

        // 5. Safely analyze how the child process terminated
        if (WIFEXITED(status)) {
            const int exit_status = WEXITSTATUS(status);

            printf("\n[myshell] %s terminated normally with exit status: %d\n", args[1], exit_status);

            // If exit_status is 5, our error handling caught an execvp() failure.
            if (exit_status == 5) {
                printf("[myshell] Warning: Exit status 5 detected. The execvp() call likely failed.\n");
            }
        } else {
            printf("[myshell] Warning: %s terminated abnormally (e.g., killed by a signal).\n", args[1]);
        }

        printf("[]");
    }


    return EXIT_SUCCESS;
}
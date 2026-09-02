/**
Scenario:
    Demonstration of an interactive shell using fork, execvp, and wait.

Details:
    This program fixes memory allocation and parsing issues by reading input
    in the parent process, tokenizing the string into a NULL-terminated array,
    and executing the target binary in an isolated child process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 30

int main() {
    printf("[TERMINAL] I am running shell program with process id: %d\n", getpid());

    while (1) {
        char line[MAX_LINE];
        char *command[MAX_ARGS];

        printf("shell_%d$ ", getpid());

        // 1. Parent reads input safely using fgets
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break; // Exit on EOF (Ctrl+D)
        }

        // Strip the trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // 2. Parse the input string into a NULL-terminated array
        int i = 0;
        command[i] = strtok(line, " ");
        
        while (command[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            command[i] = strtok(nullptr, " ");
        }

        // If the user just pressed Enter, skip to the next loop iteration
        if (command[0] == NULL) {
            continue;
        }

        // Handle built-in 'exit' command in the parent
        if (strcmp(command[0], "exit") == 0) {
            printf("[Shell_%d] Exiting...\n",getpid());
            break;
        }

        // 3. Create a duplicate process
        pid_t shell = fork();

        if (shell < 0) {
            perror("[ERROR] Failed to fork!");
            continue;
        }

        // 4. Child Process Execution Flow
        if (shell == 0) {
            printf("[Shell_%d -> Executing: %s program]\n\n", getpid(), command[0]);
            
            // execvp requires the NULL-terminated array we built in the parent
            execvp(command[0], command);

            /*
             * --- CRITICAL CONCEPT ---
             * If execvp() is successful, it never returns. The lines below 
             * ONLY execute if execvp() fails (e.g., typo in command).
             */
            perror("[ERROR] execvp() malfunctioned");
            exit(5);
        } 
        // 5. Parent Process Execution Flow
        int status;
        pid_t terminated_shell_id = wait(&status);
            
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("\n[Shell_%d] process finished normally with exit code %d\n\n", terminated_shell_id, exit_code);
                
            if (exit_code == 5) {
                printf("\n[Shell_%d] exec member malfunctioned with exit code %d\n\n", terminated_shell_id, exit_code);
            }
        } else {
            printf("\n[Shell_%d] process terminated abnormally (e.g., via signal)\n\n", terminated_shell_id);
        }
    }

    return 0;
}
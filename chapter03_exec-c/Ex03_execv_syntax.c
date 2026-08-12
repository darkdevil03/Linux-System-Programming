#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    printf("[Parent] Parent process created...\n");
    printf("[Parent] Parent process ID: %d\n", getpid());
    char *argv[] = {"/bin/ls","-l"};
    pid_t child = fork();


    if (child < 0) {
        printf("[ERROR] Failed to fork child process.\n");
        return -1;
    }
    if (child == 0) {
        printf("[Child] Child process created...\n");
        printf("[Child] process ID: %d\n", getpid());
        execv(argv[0],argv);
        perror("[Child] execv() failed.");
        exit(1);
    }

    int status;
    pid_t terminated_process_id = wait(&status);
    if (WIFEXITED(status)) {
        // Extract the actual exit code returned by the child process
        int exit_status = WEXITSTATUS(status);

        printf("[Parent] Child terminated normally with exit status: %d\n", exit_status);

        // If 'ls' succeeds, exit_status will be 0.
        // If it is 5, our error handling caught an execlp() failure.
        if (exit_status == 1) {
            printf("[Parent] Warning: Exit status 5 detected. The execlp() call likely failed.\n");
        }
    } else {
        printf("[Parent] Warning: Child terminated abnormally (e.g., killed by a signal).\n");
    }

    printf("[Parent] Parent process is terminating safely.\n");


    return 0;
}

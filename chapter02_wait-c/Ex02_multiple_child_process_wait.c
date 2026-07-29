#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum check {
    SUCCESS = 0,
    FAILURE = 1
};

// Fixed: Using pass-by-reference with a pointer (pid_t *newChild)
static int checkProcess(const pid_t *newChild) {
    if (*newChild < 0) {
        perror("Fork failed");
        return FAILURE;
    }
    return SUCCESS;
}

static void createChildProcess(const int forkCheck, const int isChild) {
    sleep(10);
    if (forkCheck == SUCCESS && isChild == 0) {
        // Use getpid() to print the actual PID of the child process
        printf("\t >_Hey, this is child process (PID: %d, PPID: %d)\n", getpid(), getppid());
        printf("\t >_Child : Execution successful!\n");

        exit(0);
    }
}

int main() {
    pid_t childProcess1 = fork();

    // Passing address (&) to demonstrate pass-by-reference as you requested
    const int forkCheck1 = checkProcess(&childProcess1);
    createChildProcess(forkCheck1, childProcess1);

    pid_t childProcess2 = fork();
    const int forkCheck2 = checkProcess(&childProcess2);
    createChildProcess(forkCheck2, childProcess2);

    // Parent code path continues here only if it's the parent
    if (childProcess1 > 0) {
        printf("Parent : Waiting for which child exit first ! \n ");

        int status;
        pid_t terminatedChildId = wait(&status);
        int childReturnValue = WEXITSTATUS(status);

        if (WIFEXITED(status)) {
            printf("Parent : This child process %d exited first with code -> %d\n", terminatedChildId, childReturnValue);
        } else {
            printf("Parent : Child process %d did not exit properly!\n", terminatedChildId);
        }
    }

    return 0;
}
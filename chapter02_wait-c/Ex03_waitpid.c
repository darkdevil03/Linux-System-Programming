#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Simple custom status codes for checking if fork worked
enum check {
    SUCCESS = 0,
    FAILURE = 1
};

/**
 * Validates if the fork() operation completed successfully.
 * @param newChild Pointer to the PID returned by fork(). (Pass-by-reference simulation in C)
 * @return enum check Returns SUCCESS (0) if valid, or FAILURE (1) if fork failed.
 */
static int checkProcess(const pid_t *newChild) {
    if (*newChild < 0) {
        perror("Fork failed");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Executes child process specific logic and terminates the child.
 * @param forkCheck Status code indicating if fork succeeded.
 * @param isChild Value returned by fork() to identify the execution flow (0 means child).
 */
static void createChildProcess(const int forkCheck, const int isChild) {
    // Execute only if fork was successful and we are inside the child process context
    if (forkCheck == SUCCESS && isChild == 0) {
        // getpid() fetches the current process ID; getppid() fetches its parent's process ID
        printf("\t >_Hey, this is child process (PID: %d, PPID: %d)\n", getpid(), getppid());
        printf("\t >_Child : Execution successful!\n");

        // Terminate the child process and return exit code 0 to the operating system
        exit(0);
    }
}

int main() {

    printf("Parent : Waiting to see which child exits first... :-)\n");

    // --- Step 1: Create First Child Process ---
    pid_t childProcess1 = fork();

    // Validate the first fork result using pass-by-reference (& operator passes memory address)
    const int forkCheck1 = checkProcess(&childProcess1);

    // If we are in child 1 context, this function will execute and call exit(0)
    createChildProcess(forkCheck1, childProcess1);

    // --- Step 2: Create Second Child Process ---
    // Note: Both the original parent AND (if it survived) any active code paths reach here
    // depending on structural design. In a standard linear flow, the parent continues here.
    const pid_t childProcess2 = fork();
    const int forkCheck2 = checkProcess(&childProcess2);
    createChildProcess(forkCheck2, childProcess2);

    // --- Step 3: Parent Process Control Logic ---
    // childProcess1 > 0 ensures that ONLY the original parent process runs this block
    // (since fork() returns the child's positive PID to the parent).
    if (childProcess1 > 0) {

        int status;
        // Wait specifically for our child process
        // It returns the PID of the terminated child and populates the status variable.
        const pid_t terminatedChildId = waitpid(childProcess2, &status, 0);

        // Extract the actual exit status code passed by the child's exit() function
        const int childReturnValue = WEXITSTATUS(status);

        // Verify if the child process terminated normally via exit statements
        if (WIFEXITED(status)) {
            printf("Parent : Child process %d exited first with code -> %d (Normally Executed)\n ", terminatedChildId, childReturnValue);
        } else {
            printf("Parent : Child process %d did not exit properly!!!!\n", terminatedChildId);
        }
    }

    return 0;
}
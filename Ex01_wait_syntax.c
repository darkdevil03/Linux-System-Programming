#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
    pid_t (Process ID type) is a standard data type defined in C (via <sys/types.h> or <unistd.h>)
    that is specifically used to represent Process IDs and Process Group IDs in Unix-like operating systems (POSIX standards).
 */


int main() {

    pid_t newChild = fork();

    if (newChild == 0) {
        printf("Hey, this is child process!\n");
        printf("Child : One statement written after exit statement check it in code, then check for existence in output or not !!\n");

        exit(0);

        printf ("Child : This line won't prints !!\n");
    }else if (newChild < 0) {
        perror("Fork failed");
        exit(1);
    }else {
        printf ("Parent : Child process created, id :%d\n",newChild);
        printf("Parent process waiting for child to exit!\n");

        int status;
        // wait() stores exit info into 'status' and returns the Child's PID
        pid_t terminatedChildId = wait(&status);

        // Extract the actual return value using WEXITSTATUS
        int childReturnValue = WEXITSTATUS(status);

        // Check if the child exited normally
        if (WIFEXITED(status)) {
            printf("Parent : Child process %d ,exited with code -> %d\n", terminatedChildId,childReturnValue);
        }else {
            printf("Parent : Child process %d did not exited properly %d!!\n", terminatedChildId, childReturnValue);
        }

    }

    return 0;
}

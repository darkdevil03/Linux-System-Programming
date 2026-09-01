#include <stdio.h>
#include<unistd.h>

int main() {

    printf("[TERMINAL] I am running with a process id: %d",getpid());

    while (1) {
        char command[30];
        pid_t shell = fork();

        if (shell == 0) {
            printf("[Shell->%s]")
        }
    }

    return 0;
}
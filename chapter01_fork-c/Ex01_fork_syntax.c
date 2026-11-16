
#include <stdio.h>
#include <unistd.h>

int main() {

    int pid = fork(); // creating process inside a process(Ex01_fork_syntax.c)

    if (pid == 0) {
        printf("[CHILD] This is child process.\n");
    }
    else {
        sleep(5);
        printf("[MAIN] This is parent process.\n");
    }



    return 0;
}

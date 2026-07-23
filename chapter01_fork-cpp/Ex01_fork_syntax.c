
#include <stdio.h>
#include <unistd.h>

int main() {

    int pid = fork(); // creating process inside a process(Ex01_fork_syntax.c)

    if (pid == 0) {
        printf("This is child process\n");
    }
    else {
        printf("This is Parent process\n");
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>

int main (int argc , char *argv[]) {
    printf("[NEW PROGRAM] I am the %s running!\n", argv[0]);
    printf("[NEW PROGRAM] My Process ID (PID) is: %d\n", getpid());
    printf("[NEW PROGRAM] My Process ID (PID) is: %d\n", getppid());

    return 0;
}

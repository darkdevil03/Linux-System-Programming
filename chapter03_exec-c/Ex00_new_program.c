/**
    Note: Compile this code then compile and run Ex01_exec_syntax.c and other required programs for understanding of Exec family() and their members type.
    For explanation of Exec() family members are respectively added in this directory program fro Ex01_
 */

#include <stdio.h>
#include <unistd.h>

int main (int argc , char *argv[]) {
    printf("[NEW PROGRAM] I am the %s running!\n", argv[0]);
    printf("[NEW PROGRAM] My Process ID (PID) is: %d\n", getpid());
    printf("[NEW PROGRAM] My Process ID (PID) is: %d\n", getppid());

    return 0;
}

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>

int main() {

    printf("I am signal generator using kill function\n");
    printf("Enter the pid of process to kill : ");
    int pid;
    scanf("%d",&pid);
    kill(pid,SIGKILL);
    printf("Process id %d is terminated\n",pid);

    return 0;
}

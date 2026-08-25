#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {

    printf("[Target] process id %d",getpid());

    signal(SIGINT,SIG_IGN);

    while (1) {
        printf("Can you dare kill me using SIGINT or signal 2!\n");
        sleep(3);
    }


    return 0;
}
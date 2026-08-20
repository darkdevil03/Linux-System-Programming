#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void custom_handler(int signum) {
    printf("Received signal %d\n", signum);
    sleep(10);
}

int main() {

    printf("[Signal_Function] process id : %d",getpid());

    signal(SIGINT, custom_handler);

    while(1) {
        printf("I am running... \n");
        sleep(1);
    }
    return 0;
}

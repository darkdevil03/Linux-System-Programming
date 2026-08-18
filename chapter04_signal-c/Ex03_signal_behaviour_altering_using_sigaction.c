#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void myHandler(const int sig) {
    printf("[Received signal : SIGINT OR %d]\n", sig);
}

int main() {

    struct sigaction act;
    act.sa_handler = myHandler;

    act.sa_flags = 0;
    act.sa_mask;
    sigemptyset(&act.sa_mask);
    printf("[Signal action()] process id %d\n", getpid());

    sigaction(2,&act,nullptr);

    while (1) {
        printf("I am sleeping...\n");
        sleep(2);
    }

    return 0;
}

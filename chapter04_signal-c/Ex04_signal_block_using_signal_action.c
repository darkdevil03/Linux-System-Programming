#include <stdio.h>
#include<signal.h>
#include <unistd.h>

// 1. Define the custom signal handler
static void myHandler(const int sig) {
    printf("\n[Target] -> [Received signal: SIGINT (Signal %d)]\n", sig);
    printf("Now check with other program to kill with SIGINT and SIGILL once again...\n");
    // Simulate a long-running task inside the handler
    sleep(60);

    printf("[Target] -> Handler finished. Resuming main loop.\n\n");
}

int main() {

    printf("[Target] I am running, my process id : %d \n", getpid());

    struct sigaction act;
    act.sa_handler = myHandler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaddset(&act.sa_mask,SIGILL);

    sigaction(SIGINT,&act,nullptr);

    printf("I can block signal SIGILL while SIGINT signal invoked by one process.\n "
           "Then, no other process will pass those both signal which can blocked and in waiting buffer until..\n"
           "After completing the myhandler then mask set to default to zero, Then kernel pass the previous received to this process will execute. \n");

    while (1) {
        printf("I am running ...\n");
        sleep(2);
    }


    return 0;
}
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void myHandler(int sig) {
    printf("I am SIGINT\n");
    sleep(60);
    printf("Resum to Main\n");
}


int main() {

    printf("[Target] Process Id: %d\n",getpid());
    signal(SIGINT,&myHandler);

    sigset_t msk;
    sigemptyset(&msk);
    sigaddset(&msk,SIGILL);
    sigaddset(&msk,1);
    int val;
    printf("Now SIGILL, and signal number 1 has been blocked until input passed!. \n");
    sigprocmask(SIG_BLOCK, &msk, NULL);

    printf("Enter a your favourite number : ");
    scanf("%d",&val);

    sigprocmask(SIG_UNBLOCK, &msk, NULL);
    printf("Now SIGILL, and signal number 1 has been unblocked until input passed!. \n");
    printf("Your favourite number : %d\n",val);
    while (1) {
        printf("check unblock status !!\n");
        sleep(2);
    }


    return 0;
}

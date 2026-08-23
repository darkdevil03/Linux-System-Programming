#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Custom signal handler for SIGINT
static void myHandler(const int sig) {
    printf("\n[Target] -> [Received signal: SIGINT (Signal %d)]\n", sig);
    printf("[Target] -> Note, that current signal (Here, SIGINT) will also automatically temporarily Blocked.\n");
    printf("[Target] -> Handler active. SIGILL is now temporarily BLOCKED which set by us while SIGINT active case.\n");
    printf("[Target] -> Quickly run Ex04_c_kill_sigill_sender.c to send SIGILL!\n");

    // Simulate a long-running task to allow time to send SIGILL
    sleep(90);

    printf("[Target] -> Handler finished. Unblocking SIGINT and SIGILL. Resuming main loop.\n\n");
}

int main() {
    printf("[Target] I am running. My process ID is: %d \n", getpid());

    struct sigaction act;
    act.sa_handler = myHandler;
    act.sa_flags = 0;

    // Initialize the mask to empty
    sigemptyset(&act.sa_mask);

    // Explicitly add SIGILL to the mask.
    // SIGILL will be blocked while myHandler is executing.
    sigaddset(&act.sa_mask, SIGILL);

    // Register the handler for SIGINT
    if (sigaction(SIGINT, &act, nullptr) == -1) {
        perror("Failed to set sigaction");
        return 1;
    }

    printf("[Info] SIGILL is configured to be blocked while SIGINT is being handled.\n");
    printf("[Info] Blocked signals remain pending in the kernel buffer until the handler finishes.\n\n");

    while (1) {
        printf("[Target] I am running ...\n");
        sleep(2);
    }

    return 0;
}
/**
     Scenario:
        Demonstration of catching signals using the basic signal() function.

    Details:
        This program sets up a custom signal handler for SIGINT (Signal 2).
        Instead of terminating when receiving an interrupt,
        it executes custom_handler(), blocking main execution for 10 seconds, before resuming.
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// 1. Define the custom signal handler
static void custom_handler(int signum) {
    printf("\n[Target] -> [Received signal %d (SIGINT)]\n", signum);
    printf("[Target] -> Handler is sleeping for 10 seconds...\n");

    // Simulate a long-running task inside the handler
    sleep(10);

    printf("[Target] -> Handler finished. Resuming main loop.\n\n");
}

int main() {
    // Added a newline character for clean terminal output
    printf("[Signal_Function] Process ID : %d\n", getpid());

    /**
       2. Register the signal handler
           We tell the OS: "If you receive SIGINT, do not terminate.
           Instead, run custom_handler."
     */
    if (signal(SIGINT, custom_handler) == SIG_ERR) {
        perror("[ERROR] Failed to register signal handler");
        return 1;
    }

    // 3. Infinite loop to keep the process alive
    while(1) {
        printf("[Target] I am running... \n");
        sleep(1);
    }

    return 0;
}
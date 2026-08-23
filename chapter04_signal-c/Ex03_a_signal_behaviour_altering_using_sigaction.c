/**
    Scenario:
        Demonstration of catching signals using sigaction().

    Details:
        This program sets up a custom signal handler for SIGINT (Signal 2).
        Instead of terminating when receiving an interrupt, it will execute
        myHandler() and then resume its normal execution loop.
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <bits/sigaction.h>

// 1. Define the custom signal handler
static void myHandler(const int sig) {
    printf("\n[Target] -> [Received signal: SIGINT (Signal %d)]\n", sig);
    // Simulate a long-running task inside the handler
    sleep(10);

    printf("[Target] -> Handler finished. Resuming main loop.\n\n");
}

int main() {
    // 2. Configure the sigaction structure
    struct sigaction act;

    // Assign our custom function to handle the signal
    act.sa_handler = myHandler;

    // Set flags to 0 (default behavior)
    act.sa_flags = 0;

    // Initialize the signal set to empty (don't block any extra signals during handler execution)
    sigemptyset(&act.sa_mask);

    printf("[Target] Signal action setup complete. Process ID: %d\n", getpid());

    /* * 3. Register the signal handler
     * SIGINT (value 2) is the signal we want to catch.
     * We pass the address of our configured 'act' struct.
     * The third argument is NULL because we don't need to save the old action.
     */
    if (sigaction(SIGINT, &act, nullptr) == -1) {
        perror("[ERROR] sigaction() failed");
        return 1;
    }

    // 4. Infinite loop to keep the process alive
    while (1) {
        printf("[Target] I am sleeping...\n");
        sleep(2);
    }

    return 0;
}
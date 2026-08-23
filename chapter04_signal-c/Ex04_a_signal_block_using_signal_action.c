/**
Scenario:
    When writing robust Linux applications, handling one signal while being interrupted by another can cause race conditions or corrupt data.
    The sigaction structure provides a safe mechanism to block specific signals temporarily while a signal handler is actively running.

Details:
    -> Automatic Masking: By default, the signal currently being handled (e.g., SIGINT) is automatically blocked until the handler finishes.
    -> Manual Masking (sa_mask): You can explicitly block additional signals by adding them to the sa_mask set using sigaddset().
    -> Signal Queueing: If a blocked signal (like SIGILL in this example) is sent to the process during handler execution,
                        the kernel places it in a "pending" state. Once the handler completes and the mask resets,
                        the pending signal is immediately delivered.

Execution Flow to follow:
    To see signal masking in action, you will need three terminal windows:
        Terminal 1: Run this program (Ex04_a_signal_block_using_signal_action.c). Note the printed PID.
        Terminal 2: Run Ex04_b_kill_sigint_sender.c. Input the target PID. This triggers the 90-second sleep handler in Ex04_a_signal_block_using_signal_action.c.
        Terminal 3: While Ex04_a_signal_block_using_signal_action.c is sleeping, run Ex04_c_kill_sigint_sender.c and input the target PID.

    Observation: Ex04_a_signal_block_using_signal_action.c will not immediately die.
                 SIGILL is blocked by sa_mask. Once the 60-second SIGINT handler completes, the pending SIGILL is delivered, and
                 Ex04_a_signal_block_using_signal_action.c will terminate with an "Illegal instruction" error (the default OS behavior for SIGILL).
*/

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
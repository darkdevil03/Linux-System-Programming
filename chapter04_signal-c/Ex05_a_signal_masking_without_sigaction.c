/**
    Scenario:
        Demonstration of protecting a critical code from some signal kill using sigprocmask().

    Details:
        This program blocks SIGHUP (Signal 1), Modified SIGINT (Signal 2) behaviour and SIGILL (Signal 4) while waiting for user input.
        It also handles SIGINT with a custom 15-second sleep to demonstrate that blocked signals execution.
        It also handles SIGQUIT with a custom 15-second sleep to demonstrate that unblocked signals can still interrupt execution.
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// 1. Custom handler for SIGINT blocked signal execution
static void myHandlerSigInt(int sig) {
    // After giving input SIGINT change the mask bit 1 to 0 (blocked to unblocked) status.
    printf("\n[Target] -> [Received SIGINT (Signal %d) after the input passed!]\n", sig);
    printf("[Target] -> Sleeping for 15 seconds. Soon main will be resume...\n");
    sleep(15);
    printf("[Target] -> Handler finished. Resuming main execution.\n\n");
}

// 1. Custom handler for SIGQUIT unblocked signal execution
static void myHandlerSigQuit(int sig) {
    printf("\n[Target] -> [Received SIGQUIT (Signal %d) while waiting for the input!]\n", sig);
    printf("[Target] -> Sleeping for 15 seconds. Soon main will be resume...\n");
    sleep(15);
    printf("[Target] -> Handler finished. Resuming main execution.\n\n");
}

int main() {
    printf("[Target] Process ID: %d\n", getpid());

    // Register custom handler for SIGINT and SIGQUIT
    signal(SIGINT, myHandlerSigInt);
    signal(SIGQUIT, myHandlerSigQuit);

    // 2. Configure the signal mask
    sigset_t msk;
    sigemptyset(&msk);
    // We can add multiple signal to block for important code without signal interruption as follow below:
    sigaddset(&msk, SIGHUP); // Add Hangup (Signal 1)
    sigaddset(&msk, SIGINT); // Add Interrupt with custom behaviour or handler (Signal 2)
    sigaddset(&msk, SIGILL); // Add Illegal Instruction (Signal 4)


    printf("[Target] SIGHUP (1), SIGINT (2) and SIGILL (4) will be blocked until input is passed.\n");
    printf("[Target] Note: SIGQUIT not manually blocked. So, by default it is unblocked throughout the program.\n");

    // 3. Apply the mask to block the signals
    sigprocmask(SIG_BLOCK, &msk, nullptr);

    /* ============================================== *
     *              CRITICAL CODE                     *
     * (Assume below code is mandatory to execute   ) *
     * (Interrupting this could cause data loss case) *
     * ============================================== */
    int val;
    printf("\n>>> Enter your favourite number: ");
    scanf("%d", &val);
    /* ========================================= */

    // 4. Remove the mask to unblock the signals
    sigprocmask(SIG_UNBLOCK, &msk, nullptr);

    printf("[Target] SIGHUP (1), SIGINT (2) and SIGILL (4) have been unblocked.\n");
    printf("[Target] Your favourite number is: %d\n", val);

    // 5. Infinite loop
    while (1) {
        printf("[Target] Main loop running. Waiting for signals...\n");
        sleep(2);
    }

    return 0;
}
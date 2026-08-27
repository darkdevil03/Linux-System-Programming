/**
    Scenario:
      Demonstration of signal dispositions (IGN, BLOCK, and Unblockable).

    Details:
        Every signal in a Unix-like operating system has a "disposition"—a default action
        that dictates how the process will react when the signal is delivered.
        Using system calls, we can change this disposition for most signals.

        1. SIG_IGN (Ignore) and SIG_DFL (Default)
            When using the signal() function, instead of passing a custom function pointer, we can pass two special macros:
              a) SIG_IGN: Tells the kernel to completely discard the signal. The process will never know it was sent.
              b) SIG_DFL: Reverts the signal's disposition back to the operating system's default behavior
                          (usually terminating the process or ignoring it, depending on the specific signal).

        2. Blocking vs. Ignoring
            It is crucial to understand the difference between ignoring a signal and blocking it:
                a) Ignore (SIG_IGN): The signal is thrown away immediately upon arrival.
                b) Block (SIG_BLOCK): Used with sigprocmask(), this tells the kernel to put the signal in a "pending" queue.
                                      It is not thrown away; it is simply held until the program unblocks it.

        3. The Un-ignorable Signals (SIGKILL and SIGSTOP)**
            For system stability and security, the Linux kernel strictly enforces that two signals cannot be caught, blocked, or ignored:
                a) SIGKILL (Signal 9): Forces immediate termination.
                b) SIGSTOP (Signal 19): Forces immediate suspension (pause).
        If a programmer attempts to pass SIGKILL to signal(SIGKILL, SIG_IGN), the function will fail and return a SIG_ERR flag.

Note: The difference between Signal Disposition and Signal Mask Commands
    1. Signal Dispositions (The Action Commands)
        These dictate what the process should do the moment a signal is actually delivered.
        They are used as parameters for the signal() or sigaction() functions.
            a) SIG_IGN (Ignore): The disposition is to silently discard the signal.
            b) IG_DFL (Default): The disposition is to do whatever the operating system normally does for that signal (usually terminate or ignore).
            c) (Custom Handler): Pointing to your own custom function (e.g., myHandler) is the third type of disposition.

    2. Signal Mask Commands (The Queue Rules)
        These are not dispositions. They are operational commands used exclusively with the sigprocmask() function to
        tell the kernel how to modify the process's list of blocked (pending) signals.
            a) SIG_BLOCK: Command to add signals to the blocked queue.
            b) SIG_UNBLOCK: Command to remove signals from the blocked queue.
            c) SIG_SETMASK: Command to completely replace the blocked queue with a brand-new list.

    Think of Dispositions (SIG_IGN, SIG_DFL) as the "action plan" for when a signal arrives,
    and Mask Commands (SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK) as the tools used to manage the "waiting room" for signals.
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    printf("[Target] Process ID: %d\n\n", getpid());

    // 1. IGNORING A SIGNAL
    // Tell the kernel to throw away any incoming SIGINT (Interrupt) signals.
    printf("[Target] Setting SIGINT (2) disposition to SIG_IGN (Ignore)...\n");
    signal(SIGINT, SIG_IGN);

    // 2. BLOCKING A SIGNAL
    // Tell the kernel to queue SIGILL (Illegal Instruction) if it arrives.
    printf("[Target] Blocking SIGILL (4) using sigprocmask (Pending queue)...\n");
    sigset_t msk;
    sigemptyset(&msk);
    sigaddset(&msk, SIGILL);
    sigprocmask(SIG_BLOCK, &msk, nullptr);

    // 3. THE UN-IGNORABLE SIGNAL
    // Attempting to ignore SIGKILL will result in an error from the kernel.
    printf("[Target] Attempting to set SIGKILL (9) to SIG_IGN...\n");
    if (signal(SIGKILL, SIG_IGN) == SIG_ERR) {
        printf("[Target WARNING] Failed to ignore SIGKILL!!!");
        printf("[Target] -> Note: The OS strictly prevents ignoring or blocking SIGKILL.\n");
    }

    printf("\n========================================================\n");
    printf(" Setup complete. Waiting for signals from other programs.\n");
    printf("========================================================\n\n");

    while (1) {
        printf("[Target] I am running... Dare to kill me!\n");
        sleep(3);
    }

    return 0;
}
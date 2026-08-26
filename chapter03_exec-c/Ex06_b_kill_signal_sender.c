/**
Scenario:
    To Sends SIGINT to test the SIG_IGN disposition and Sends SIGILL to test the SIG_BLOCK disposition.
 */

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    int target_pid;

    printf("[Sender 1] Testing SIG_IGN (Ignore).\n");
    printf("[Sender 1] Enter the PID of the target to send SIGINT to: ");

    if (scanf("%d", &target_pid) != 1) return 1;

    // Send SIGINT (Signal 2). The target is configured to ignore this.
    if (kill(target_pid, SIGINT) == -1) {
        perror("[ERROR] Failed to send SIGINT");
        return 1;
    }

    printf("[Sender 1] SUCCESS: SIGINT sent. The target should ignore this.\n");


    printf("[Sender 2] Testing SIG_BLOCK (Pending Queue).\n");
    printf("[Sender 2] Enter the PID of the target to send SIGILL to: ");

    if (scanf("%d", &target_pid) != 1) return 1;

    // Send SIGILL (Signal 4). The target has blocked this, so it will pend.
    if (kill(target_pid, SIGILL) == -1) {
        perror("[ERROR] Failed to send SIGILL");
        return 1;
    }

    printf("[Sender 2] SUCCESS: SIGILL sent. It is now pending in the target's kernel queue.\n");

    return 0;
}
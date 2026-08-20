/**
    Scenario:
        A signal generator using the kill() system call.

    Details:
        This program prompts the user for a target PID and sends a SIGINT
        (Interrupt) signal to that process to test its custom signal handler.
 */

#include <stdio.h>
#include<sys/types.h>
#include <signal.h>

int main() {
    int target_pid;

    printf("[Sender] I am the signal generator using the kill() function.\n");
    printf("[Sender] Enter the PID of the process to send SIGINT to: ");

    // Read the target PID from standard input
    if(scanf("%d", &target_pid) != 1) {
        printf("[ERROR] Invalid input. Please enter a valid integer PID.\n");
        return 1;
    }

    /* * Send SIGINT (Signal 2) to the target process.
     * We wrap it in an if-statement to handle potential errors
     * (e.g., process doesn't exist, or lack of permissions).
     */
    if (kill(target_pid, SIGINT) == -1) {
        // perror will automatically print the specific reason for failure
        perror("[ERROR] Failed to send SIGINT");
        return 1;
    }

    printf("[Sender] SUCCESS: SIGINT sent to Process ID %d.\n", target_pid);

    return 0;
}

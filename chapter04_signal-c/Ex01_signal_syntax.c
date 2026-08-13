/**
    Scenario:
        A signal generator using the kill() system call.

    Details:
        This program prompts the user for a PID and sends a SIGKILL signal to that process,
        unconditionally terminating it.
 */

#include <signal.h>
#include <stdio.h>

int main() {
    int target_pid;

    printf("[Sender] I am the signal generator using the kill() function.\n");
    printf("[Sender] Enter the PID of the process to terminate: ");

    // Read the target PID from standard input
    if(scanf("%d", &target_pid) != 1) {
        printf("[ERROR] Invalid input. Please enter a valid integer PID.\n");
        return 1;
    }

    /* * Send SIGKILL to the target process.
     * SIGKILL (Signal 9) cannot be caught or ignored by the target.
     * We wrap it in an if-statement to handle potential errors
     * (e.g., process doesn't exist, or lack of permissions).
     */
    if (kill(target_pid, SIGKILL) == -1) {
        // perror will automatically print the specific reason for failure
        perror("[ERROR] Failed to send SIGKILL");
        return 1;
    }

    printf("[Sender] SUCCESS: SIGKILL sent. Process ID %d has been terminated.\n", target_pid);

    return 0;
}
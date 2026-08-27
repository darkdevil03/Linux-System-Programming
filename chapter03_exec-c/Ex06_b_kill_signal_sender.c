/**
Scenario:
    To Sends SIGINT to test the SIG_IGN disposition,
    also to sends SIGILL to test the SIG_BLOCK disposition, and also
    to sends SIGKILL to demonstrate kernel-enforced termination.
 */

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    short iterate = 1;

    while (iterate != 0) {
        int option;
        printf("\nThe option list for disposition and mask signal commands :\n");
        printf("   1.SIGIGN Disposition for SIGINT \n   2.SIG_BLOCK Disposition for SIGILL "
               "\n   3.Test Kernel-Enforced Termination For SIGIGN Applied To SIGKILL \n   4.Exit\n");
        printf(">>> Enter your option from above list: ");
        if (scanf("%d", &option) != 1) {
            printf("Please enter valid integer value to access the options from the main menu.\n");

            // Clear the invalid input from the buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF){}

            continue;
            //return 1;
        }

        switch (option) {
            case 1: {
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
                break;
            }

            case 2: {
                int target_pid;
                printf("[Sender 2] Testing SIG_BLOCK (Pending Queue).\n");
                printf("[Sender 2] Enter the PID of the target to send SIGILL to: ");

                if (scanf("%d", &target_pid) != 1) return 1;

                // Send SIGILL (Signal 4). The target has blocked this, so it will pend.
                if (kill(target_pid, SIGILL) == -1) {
                    perror("[ERROR] Failed to send SIGILL");
                    return 1;
                }

                printf("[Sender 2] SUCCESS: SIGILL sent. It is now pending in the target's kernel queue.\n");
                break;
            }

            case 3: {
                int target_pid;
                printf("[Sender 3] Testing un-ignorable termination.\n");
                printf("[Sender 3] Enter the PID of the process to forcibly terminate: ");

                if (scanf("%d", &target_pid) != 1) return 1;

                // Send SIGKILL (Signal 9). This cannot be caught, blocked, or ignored.
                if (kill(target_pid, SIGKILL) == -1) {
                    perror("[ERROR] Failed to send SIGKILL");
                    return 1;
                }
                printf("[Sender 3] SUCCESS: SIGKILL sent. Process ID %d has been immediately terminated.\n", target_pid);
                break;
            }

            case 4: {
                iterate = 0;
                printf("<<< Exiting.....\n");
                break;
            }

            default: {
                printf("[ERROR] Please enter the correct option range from 1-4.\n ");
            }
        }
    }

    return 0;
}
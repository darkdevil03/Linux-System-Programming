#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main() {

    int choice;
    int loop = 1;

    while (loop != 0) {
        printf("\n++====================================================++");
        printf("\n||               Process kill Sender                  ||\n");
        printf("++====================================================++\n");
        printf("Options: \n  1.SIGINT Custom Handler Kill  Isn't Manually Mask Set "
               "\n  2.SIGQUIT Custom Handler Kill Manually Mask Set To Blocked Till Input Pass \n  3.SIGHUP Kill Manually Mask Set To Blocked Till Input Pass"
               "\n  4.SIGILL Kill Manually Mask Set To Blocked Till Input Pass \n  5.Exit \n");
        printf(">>> Enter Your Choice : ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
            {
                int target_pid;

                printf("[Sender 1] I am the signal generator for SIGINT (Signal 2).\n");
                printf("[Sender 1] Enter the PID of the process to signal: ");

                if(scanf("%d", &target_pid) != 1) {
                    printf("[ERROR] Invalid input.\n");
                    return 1;
                }

                // Send SIGINT (Signal 2)
                if (kill(target_pid, SIGINT) == -1) {
                    perror("[ERROR] Failed to send SIGINT");
                    return 1;
                }

                printf("[Sender 1] SUCCESS: SIGINT sent to Process ID %d.\n", target_pid);
                break;
            }

            case 2:
            {
                int target_pid;

                printf("[Sender 2] I am the signal generator for SIGQUIT (Signal 3).\n");
                printf("[Sender 2] Enter the PID of the process to signal: ");

                if(scanf("%d", &target_pid) != 1) {
                    printf("[ERROR] Invalid input.\n");
                    return 1;
                }

                // Send SIGINT (Signal 2)
                if (kill(target_pid, SIGQUIT) == -1) {
                    perror("[ERROR] Failed to send SIGQUIT");
                    return 1;
                }

                printf("[Sender 2] SUCCESS: SIGQUIT sent to Process ID %d.\n", target_pid);
                break;
            }

            case 3: {
                int target_pid;

                printf("[Sender 3] I am the signal generator for SIGHUP (Signal 1).\n");
                printf("[Sender 3] Enter the PID of the process to signal: ");

                if(scanf("%d", &target_pid) != 1) {
                    printf("[ERROR] Invalid input.\n");
                    return 1;
                }

                // Send SIGHUP (Signal 1)
                if (kill(target_pid, SIGHUP) == -1) {
                    perror("[ERROR] Failed to send SIGHUP");
                    return 1;
                }

                printf("[Sender 3] SUCCESS: SIGHUP sent to Process ID %d.\n", target_pid);
                break;
            }

            case 4: {
                int target_pid;

                printf("[Sender 4] I am the signal generator for SIGILL (Signal 4).\n");
                printf("[Sender 4] Enter the PID of the process to signal: ");

                if(scanf("%d", &target_pid) != 1) {
                    printf("[ERROR] Invalid input.\n");
                    return 1;
                }

                // Send SIGILL (Signal 4)
                if (kill(target_pid, SIGILL) == -1) {
                    perror("[ERROR] Failed to send SIGILL");
                    return 1;
                }

                printf("[Sender 4] SUCCESS: SIGILL sent to Process ID %d.\n", target_pid);
                break;
            }
            case 5: {
                loop = 0;
                printf("Thank you, Exiting the program...\n");
                break;
            }
            default: {
                printf("[ERROR] Please enter a valid option from 1 to 5.\n");
            }
        }
    }

    return 0;
}
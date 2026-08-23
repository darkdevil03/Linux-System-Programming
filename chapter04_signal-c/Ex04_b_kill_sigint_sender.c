#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    int target_pid;

    printf("[Sender 1] I am the signal generator for SIGINT (Interrupt).\n");
    printf("[Sender 1] Enter the PID of the process to signal: ");

    if(scanf("%d", &target_pid) != 1) {
        printf("[ERROR] Invalid input.\n");
        return 1;
    }

    if (kill(target_pid, SIGINT) == -1) {
        perror("[ERROR] Failed to send SIGINT");
        return 1;
    }

    printf("[Sender 1] SUCCESS: SIGINT sent to Process ID %d.\n", target_pid);
    return 0;
}
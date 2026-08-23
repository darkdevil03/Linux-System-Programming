#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    int target_pid;

    printf("[Sender 2] I am the signal generator for SIGILL (Illegal Instruction).\n");
    printf("[Sender 2] Enter the PID of the process to signal: ");

    if(scanf("%d", &target_pid) != 1) {
        printf("[ERROR] Invalid input.\n");
        return 1;
    }

    if (kill(target_pid, SIGILL) == -1) {
        perror("[ERROR] Failed to send SIGILL");
        return 1;
    }

    printf("[Sender 2] SUCCESS: SIGILL sent to Process ID %d.\n", target_pid);
    return 0;
}
/**
    Scenario:
        A target process designed to loop infinitely until terminated.

    Details:
        This program prints its own Process ID (PID) and enters a sleep loop.
        It serves as a dummy process for our signal generator to target.
 */

#include <stdio.h>
#include <unistd.h>

int main() {
    printf("[Target] Hey, I am the signal test sample program.\n");

    // getpid() retrieves the Process ID of the currently running process
    printf("[Target] My PID is: %d\n", getpid());
    printf("[Target] Waiting for a signal... You can kill me from the other terminal.\n\n");

    // Infinite loop simulating a long-running background service
    while (1) {
        printf("[Target] I am sleeping...\n");

        // Sleep for 2 seconds to prevent spamming the terminal and saving CPU cycles
        sleep(2);
    }

    return 0; // This line is practically unreachable due to the infinite loop
}

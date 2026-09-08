#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>  // 1. Include this for signal handling functions

// 2. Create a simple signal handler function
// It doesn't actually have to do anything.
// Just printing signal number and catching the signal is enough
// to interrupt the read() function in main().
// Note: In previous programs Ex05 and Ex06 of this folder, reading all bytes.
//       But CTRL+C generated default behaviour of SIGINT interrupt and terminate
// To overcome that, Here we are using sigaction to test EINT error handling block.
void handle_signal(int sig) {
    printf("\n[SIGNAL] Got interrupt from keyboard with signal: %d !! \n",sig);
    sleep(2);
}

int main() {
    char buf[21];
    ssize_t bytes_read = 0;

    // 3. Set up the signal catcher for SIGINT (Ctrl+C)
    struct sigaction sa;
    sa.sa_handler = handle_signal; // Point to our custom function
    sigemptyset(&sa.sa_mask);

    // CRITICAL: We set flags to 0. If we used SA_RESTART here, the kernel
    // would automatically restart the read() call behind the scenes, and
    // we would never see the EINTR error!
    sa.sa_flags = 0;

    // Apply the signal catcher to SIGINT
    sigaction(SIGINT, &sa, nullptr);

    printf("--- BLOCKING MODE ---\n");
    printf("This program is now frozen.\n");
    printf("PRESS Ctrl+C to trigger the EINTR error!\n");
    printf("Or type normal text and press Enter: ");
    fflush(stdout);

    // 4. The program freezes here, waiting for the keyboard.
    bytes_read = read(STDIN_FILENO, buf, 20);

    if (bytes_read == -1) {
        if (errno == EINTR) {
            // 5. If we pressed Ctrl+C, the code jumps straight here!
            printf("\n\n[EINTR] Interrupted by signal via pressing CTRL+C from your keyboard!\n");
            printf("So, the kernel woke up the program, ran handle_signal(), and aborted the read().\n");
        }else {
            perror("Error reading");
        }
        return EXIT_FAILURE;
    }
    if (bytes_read == 0) {
        // This specifically catches Ctrl+D (End of File)
        printf("\n\n[EOF] You pressed Ctrl+D! The input stream was closed.\n");
        return EXIT_SUCCESS;
    }

    // Cap the string and print it
    buf[bytes_read] = '\0';
    printf("\nNumber of data bytes read: %zd\n", bytes_read-1);
    printf("Data received content: %s", buf);

    return 0;
}
/**
Scenario:
    In this program, it reaches the read() function and completely freezes.
    It cannot do any other work until we type something and press Enter.
Details:
    To test standard terminal input (our keyboard), we read from STDIN_FILENO (File Descriptor 0).
    Here are this program demonstrating action of reading from the keyboard.

 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char buf[21];
    ssize_t bytes_read = 0;

    printf("--- BLOCKING MODE ---\n");
    printf("The program is now frozen. Type something and press Enter: ");
    fflush(stdout); // Ensure the text prints before we freeze

    // We read directly from STDIN_FILENO (0)
    bytes_read = read(STDIN_FILENO, buf, 20);

    if (bytes_read == -1) {
        if (errno == EINTR) {
            printf("\nInterrupted by signal, but we could try again.\n");
        } else {
            perror("Error reading happened!!"); // press CTRL+D we get this statement invoked.
        }
        return EXIT_FAILURE;
    }

    // Cap the string and print it
    buf[bytes_read] = '\0';
    printf("Number of data read: %zd\n", bytes_read);
    printf("Data received: %s", buf);

    return 0;
}
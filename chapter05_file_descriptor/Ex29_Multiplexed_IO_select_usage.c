#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    fd_set watch_list;       // The list of file descriptors we want to watch
    struct timeval timeout;  // The maximum time we are willing to wait
    int ready_count;

    printf("--- Multiplexed I/O Demonstration ---\n");
    printf("You have 10 seconds to type something and press Enter...\n\n");

    // 1. Clear the watch list so it's empty
    FD_ZERO(&watch_list);

    // 2. Add the Keyboard (Standard Input, which is always File Descriptor 0) to our watch list
    FD_SET(STDIN_FILENO, &watch_list);

    // 3. Set our timeout clock to 10 seconds and 0 microseconds
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    // 4. THE MULTIPLEXER: select()
    // We hand the OS our watch list and our clock. The program goes to sleep here.
    // It will ONLY wake up if you press a key OR if the 10 seconds run out.
    ready_count = select(STDIN_FILENO + 1, &watch_list, nullptr, nullptr, &timeout);

    // 5. Figure out why we woke up
    if (ready_count == -1) {
        perror("[ERROR] select() failed");
        return EXIT_FAILURE;
    }
    else if (ready_count == 0) {
        // The timeout ran out before any file descriptors were ready
        printf("\n[TIMEOUT] You didn't type anything fast enough! The program didn't freeze forever.\n");
    }
    // If we get here, data is ready! Now we check WHO is ready.
    else {
        // Here, we checking that is STDIN_FILENO is present watch_list of readfds related!
        if (FD_ISSET(STDIN_FILENO, &watch_list) == 0) {
            fprintf(stderr,"[ERROR] File descriptor is not present in the watchlist to perform readfds operation!!");
            return EXIT_FAILURE;
        }

        // The watch list woke us up! That means the keyboard has data ready to be read.
        // Now it is 100% safe to call read() because we know it won't block.
        char buffer[100];
        ssize_t bytes = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

        if (bytes > 0) {
            buffer[bytes] = '\0'; // Null-terminate the string
            printf("\n[SUCCESS] You typed: %s", buffer);
        }
    }

    return EXIT_SUCCESS;
}
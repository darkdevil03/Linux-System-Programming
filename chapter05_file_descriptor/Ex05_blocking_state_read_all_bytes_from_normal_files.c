#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd = open("/home/xap/Clion/Linux-System-Programming/chapter05_file_descriptor/File00_test_file.txt", O_RDONLY);

    if (fd == -1) {
        perror("[ERROR] File descriptor failed to open");
        return EXIT_FAILURE;
    }

    // Declare a standard character array
    char buf[21];

    // Use a tracking pointer to advance through the buffer
    char *ptr = buf;

    ssize_t bytes_read = 0;
    size_t len = 20;

    while (len != 0 && (bytes_read = read(fd, ptr, len)) != 0) {
        if (bytes_read == -1) {
            if (errno == EINTR) {
                printf("Got interruption signal \n");
                continue; // Interrupted by signal, safely try again
            }
            perror("[ERROR] Failed to read from file");
            break;
        }

        len -= bytes_read;

        // Advance the tracking pointer, leaving the base 'buf' intact
        ptr += bytes_read;
    }
    if (bytes_read == 0) {
        printf("\nRead file was empty!! Please check the file...\n");
        return EXIT_SUCCESS;
    }

    // Add the null terminator exactly where the reading stopped
    *ptr = '\0';     // buf[bytes_read] = '\0'; // we can use this for adding null terminator.

    printf("Number bytes read from file : %zd\n",bytes_read);
    // Print from the original base address ('buf')
    printf("Content in buf : %s\n", buf);

    // 7. Always clean up
    close(fd);
    return 0;
}
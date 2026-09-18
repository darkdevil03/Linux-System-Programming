#define _XOPEN_SOURCE 500

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd;
    char buffer[20];
    ssize_t result;

    printf("--- Demonstrating pread() Return Values ---\n\n");

    // 1. Create a tiny file with just 5 bytes of data
    fd = open("Created_Files/File26_Positional_read_write_test_errors.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file! ");
        return EXIT_FAILURE;
    }
    write(fd, "HELLO", 5);

    // ---------------------------------------------------------
    // CASE A: SUCCESS (Returns > 0)
    // ---------------------------------------------------------
    // Read 3 bytes starting at offset 0
    result = pread(fd, buffer, 3, 0);
    if (result > 0) {
        buffer[result] = '\0';
        printf("[CASE A] Success! pread() returned %ld bytes. Data: '%s'\n", result, buffer);
    }

    // ---------------------------------------------------------
    // CASE B: END OF FILE (Returns 0)
    // ---------------------------------------------------------
    // Try to read 10 bytes, but start at offset 100.
    // The file is only 5 bytes long, so offset 100 is pure empty space.
    result = pread(fd, buffer, 10, 100);
    if (result == 0) {
        printf("[CASE B] EOF Reached! pread() returned 0. There is no data at offset 100.\n");
    }

    // ---------------------------------------------------------
    // CASE C: THE "lseek" ERROR (Returns -1)
    // ---------------------------------------------------------
    // Try to read from offset -10. This is physically impossible.
    // This proves the last line of your text: pread can throw lseek errors!
    result = pread(fd, buffer, 5, -10);
    if (result == -1) {
        printf("[CASE C] Error Triggered! pread() returned -1.\n");
        printf("         Error Code (errno): %d - %s\n", errno, strerror(errno));
        printf("         (Notice this is an 'lseek' style error, not a read error!)\n");
    }

    close(fd);
    return EXIT_SUCCESS;
}
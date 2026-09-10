#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    int fd = open("File00_test_file.txt", O_WRONLY | O_APPEND);

    if (fd == -1) {
        printf("[ERROR] Failed to open file\n");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File opened successfully\n");

    char write_buffer[] = "Line from Ex12_fdatasync.c : Fdatasync related data!";

    ssize_t written_bytes = write(fd, write_buffer, strlen(write_buffer));

    if (written_bytes == -1) {
        printf("[ERROR] Failed to write to file\n");
        return EXIT_FAILURE;
    }

    write_buffer[written_bytes] = '\0';

    if (fdatasync(fd) == -1) {
        printf("[ERROR] Failed to fsync file!\n");
        printf("[Warning]It's now in the control of kernel...\n");
    }


    printf("Number of bytes written : %zd\n",written_bytes);
    printf("Written content into file : %s\n",write_buffer);

    close(fd);

    return EXIT_SUCCESS;
}

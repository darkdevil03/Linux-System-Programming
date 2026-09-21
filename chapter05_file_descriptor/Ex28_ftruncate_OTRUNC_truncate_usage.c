#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static void print_file_size(const char *path, int fd) {
    off_t file_size = lseek(fd,0,SEEK_END);
    if (file_size == -1) {
        perror("[ERROR] File Descriptor closed might be!!!\n");
        exit(EXIT_FAILURE);
    }
    printf("Current size of '%s': %ld bytes\n", path, file_size);
}

int main() {
    const char *filepath = "Created_Files/File28_Different_types_truncation_demo.txt";

    // ---------------------------------------------------------
    // SETUP: Create a file with 40 bytes of data
    // ---------------------------------------------------------
    int fd = open(filepath, O_CREAT | O_WRONLY, 0644);
    char initial_data[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCD"; // 40 chars
    write(fd, initial_data, strlen(initial_data));


    printf("--- INITIAL SETUP ---\n");
    print_file_size(filepath, fd); // Expect: 40 bytes

    // ---------------------------------------------------------
    // DEMO 1: truncate() by path
    // We trim the file from 40 bytes down to 20 bytes without opening it.
    // ---------------------------------------------------------
    printf("\n--- 1. truncate() DEMO ---\n");

    if (truncate(filepath, 20) == -1) {
        perror("[ERROR] truncate() failed");
        return EXIT_FAILURE;
    }
    printf("Successfully ran truncate(filepath, 20).\n");
    print_file_size(filepath, fd); // Expect: 20 bytes

    close(fd); // Note: fd is opened till now for printing size purpose only.


    // ---------------------------------------------------------
    // DEMO 2: ftruncate() by file descriptor
    // We open the file, extend it to 50 bytes, and shrink it to 10 bytes.
    // ---------------------------------------------------------
    printf("\n--- 2. ftruncate() DEMO ---\n");

    fd = open(filepath, O_RDWR);
    if (fd == -1) {
        perror("[ERROR] Failed to open file for ftruncate");
        return EXIT_FAILURE;
    }

    // A. Extending the file (Padding with null bytes)
    if (ftruncate(fd, 50) == -1) {
        // Error handling for read-only / bad FD issues
        if (errno == EINVAL) {
            fprintf(stderr, "[ERROR] fd is not open for writing!\n");
        }
        perror("[ERROR] ftruncate() failed");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Successfully extended via ftruncate(fd, 50).\n");
    print_file_size(filepath,fd); // Expect: 50 bytes

    // B. Shrinking the file
    ftruncate(fd, 10);
    printf("Successfully shrank via ftruncate(fd, 10).\n");
    print_file_size(filepath,fd); // Expect: 10 bytes

    close(fd);


    // ---------------------------------------------------------
    // DEMO 3: O_TRUNC in open()
    // We wipe the file back to 0 bytes instantly upon opening.
    // ---------------------------------------------------------
    printf("\n--- 3. O_TRUNC DEMO ---\n");

    fd = open(filepath, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("[ERROR] Failed to open with O_TRUNC");
        return EXIT_FAILURE;
    }

    printf("Successfully opened with O_TRUNC flag.\n");
    print_file_size(filepath,fd); // Expect: 0 bytes

    // Clean up
    close(fd);

    return EXIT_SUCCESS;
}

/**
   |---------------|------------------------|---------------------------------|---------------------------------------|
   |    Feature    |        O_TRUNC         |           truncate()            |              truncate()               |
   |               |        (Flag)          |          (System Call)          |             (System Call)             |
   |---------------|------------------------|---------------------------------|---------------------------------------|
   | Target        |  Used inside open()    |     Uses a File Descriptor (fd) |   "Uses a File Path (""path.txt"")"   |
   |---------------|------------------------|---------------------------------|---------------------------------------|
   | Size control  |  Always 0 bytes        |     Any exact byte size         |    Any exact byte size                |
   |---------------|------------------------|---------------------------------|---------------------------------------|
   | Prerequisite  |  File is being opened  |     File is currently open      |    File is currently closed (or open) |
   |---------------|------------------------|---------------------------------|---------------------------------------|
   | Speed         |  Instant on open       |     Dynamic during runtime      |    External modification              |
   |---------------|------------------------|---------------------------------|---------------------------------------|

 */
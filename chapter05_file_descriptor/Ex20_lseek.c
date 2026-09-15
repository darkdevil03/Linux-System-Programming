#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd = open("/home/xap/Clion/Linux-System-Programming/chapter05_file_descriptor/File20_data.txt", O_RDONLY);
    char buffer[15]; // 10 bytes for data + 1 for null-terminator
    buffer[14] = '\0'; // Ensure string is safely terminated for printing

    if (fd == -1) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("--- Demonstrating lseek() ---\n\n");

    // ---------------------------------------------------------
    // 1. SEEK_SET: Jump exactly to Byte 30 (Start of Line 4)
    // ---------------------------------------------------------
    lseek(fd, 22, SEEK_SET);
    read(fd, buffer, 10);
    printf("1. SEEK_SET (Pos 30): %s\n", buffer); // Prints: Line 04...

    // Note: After reading 10 bytes, our cursor is now sitting at Byte 40.

    // ---------------------------------------------------------
    // 2. SEEK_CUR: Skip forward 20 bytes from our current spot
    // ---------------------------------------------------------
    // We are at Byte 40. We want to skip Line 5 and Line 6.
    // 40 + 20 = Byte 60 (Start of Line 7)
    lseek(fd, 3, SEEK_CUR);
    read(fd, buffer, 10);
    printf("2. SEEK_CUR (Pos +20): %s\n", buffer); // Prints: Line 07...

    // ---------------------------------------------------------
    // 3. SEEK_END: Jump backwards from the very end of the file
    // ---------------------------------------------------------
    // The file is 100 bytes long.
    // We want to read the 9th line, which starts at Byte 80.
    // End (100) - 20 = Byte 80.
    lseek(fd, 0, SEEK_END);
    read(fd, buffer, 10);
    printf("3. SEEK_END (Pos -20): %s\n", buffer); // Prints: Line 09...

    // Bonus Trick: Finding out where you are!
    // As the book states, passing a pos of 0 with SEEK_CUR returns your current position.
    off_t current_spot = lseek(fd, 0, SEEK_CUR);
    printf("\nCurrently sitting at byte: %ld\n", current_spot); // Prints: 90

    close(fd);
    return EXIT_SUCCESS;
}
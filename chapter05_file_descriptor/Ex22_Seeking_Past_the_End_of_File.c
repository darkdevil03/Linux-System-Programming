#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Create a new file for writing
    int fd = open("sparse_file.bin", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("--- Creating a Sparse File ---\n");

    // 2. Write data at the very beginning (Byte 0)
    write(fd, "START", 5);
    printf("Wrote 'START' at the beginning of the file.\n");

    // 3. The Magic Jump: Seek 100 Megabytes past the end of the file!
    // 100 MB = 100 * 1024 * 1024 bytes = 104,857,600 bytes

    // 3. The Magic Jump: Seek 5 MB past the end of the file!
    // 5 MB = 5 * 1024 * 1024 bytes = 5,242,880 bytes
    off_t massive_jump = 5 * 1024 * 1024;


    if (lseek(fd, massive_jump, SEEK_END) == (off_t)-1) {
        perror("Failed to jump");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Jumped 5 Megabytes into the void...\n");

    // 4. Write data at the new location to "lock in" the hole
    // If we don't write anything, the OS won't stretch the file.
    write(fd, "END", 3);
    printf("Wrote 'END' at the new location.\n");

    close(fd);

    printf("\n[SUCCESS] File 'sparse_file.bin' created.\n");
    printf("To the system, it looks like 100MB, but it takes up almost no physical disk space!\n");

    return EXIT_SUCCESS;
}
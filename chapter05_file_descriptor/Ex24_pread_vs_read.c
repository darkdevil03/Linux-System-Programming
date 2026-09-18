

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {

    int fd = open("Created_Files/File00_test_file.txt", O_RDONLY);

    if (fd == -1) {
        perror("[ERROR] Failed to open the file!!!\n");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File descriptor opened file successfully!!!\n");

    char buffer[256];
    off_t cursor_location = lseek(fd,0,SEEK_CUR);
    printf("[INITIAL] Current cursor location in file at %ld location.\n", cursor_location);

    printf("\n\t--- Reading the file using read() ---\n");
    ssize_t bytes_read = read(fd, buffer, 13);

    if (bytes_read == -1) {
        perror("[READ-ERROR] Failed to read from the file!!!\n");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0';

    printf("[SUCCESS] %zd Bytes read from the file...\n", bytes_read);
    printf("[READ] Fetch content from file to buffer : %s\n",buffer);

    cursor_location = lseek(fd,0,SEEK_CUR);
    printf("[AFTER-READ] Current cursor location in file at %ld location.\n", cursor_location);

    printf("\n\t--- Reading the file using pread() ---\n");

    ssize_t pread_bytes = pread(fd,buffer,44, 31);

    if (pread_bytes == -1) {
        perror("[PREAD-ERROR] Failed to read from the file!!!\n");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[pread_bytes] = '\0';

    printf("[SUCCESS] %zd Bytes read from the file...\n", pread_bytes);
    printf("[PREAD] Fetch content from file to buffer : %s\n",buffer);

    cursor_location = lseek(fd,0,SEEK_CUR);
    printf("[AFTER-PREAD] Current cursor location in file at %ld location.\n", cursor_location);


    close(fd);
    return EXIT_SUCCESS;
}

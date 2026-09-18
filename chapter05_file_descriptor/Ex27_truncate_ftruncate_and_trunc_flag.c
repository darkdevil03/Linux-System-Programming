#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    int fd = open("Created_Files/File27_Truncate.txt", O_RDWR);

    if (fd == -1) {
        perror("Failed to open file! ");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File opened successfully!\n");

    printf("\n--- [BEFORE-TRUNCATE] Reading the file content --- \n");
    off_t total_bytes = lseek(fd, 0, SEEK_END);
    char read_buffer[total_bytes + 1];
    size_t bytes_read = pread(fd, read_buffer, total_bytes,0);

    if (bytes_read == -1) {
        perror("\t[READ-ERROR] Failed to read from the file!!!\n");
        close(fd);
        return EXIT_FAILURE;
    }

    read_buffer[bytes_read] = '\0';

    printf("\t[SUCCESS] %ld Bytes read from the file...\n", bytes_read);
    printf("\t[READ] Fetch content from file to buffer : \n%s\n\n",read_buffer);


    int ret = truncate("Created_Files/File27_Truncate.txt", 30);
    if (ret == -1) {
        printf("[FAILED] Failed to truncate the file! \n");
        printf("Check the given file path/file name is correct!!\n");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] Truncating the file content to 30-Bytes!! \n");

    printf("\n--- [AFTER-TRUNCATE] Reading the file content --- \n");

    total_bytes = lseek(fd, 0, SEEK_END);
    char new_read_buffer[total_bytes + 1];
    bytes_read = pread(fd, new_read_buffer, total_bytes,0);

    if (bytes_read == -1) {
        perror("\t[READ-ERROR] Failed to read from the file!!!\n");
        close(fd);
        return EXIT_FAILURE;
    }

    new_read_buffer[bytes_read] = '\0';

    printf("\t[SUCCESS] %ld Bytes read from the file...\n", bytes_read);
    printf("\t[READ] Fetch content from file to buffer : \n%s\n\n",new_read_buffer);

    close(fd);
    return EXIT_SUCCESS;
}


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    int fd = open("Created_Files/File25_pwrite_vs_write_data.txt", O_WRONLY | O_APPEND);

    if (fd == -1) {
        perror("[ERROR] Failed to open file!! ");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File opened successfully!!\n");

    off_t cursor_location = lseek(fd,0,SEEK_CUR);
    printf("[INITIAL] Current cursor location in file at %ld location.\n", cursor_location);

    printf("\n\t--- Writing data into the file using write() ---\n");

    char *write_buffer = malloc(256 * sizeof(char));
    size_t write_buffer_content_length = 0;

    printf("Enter the data to write into file : ");

    if (fgets(write_buffer,256,stdin)!= nullptr) {
        write_buffer[strcspn(write_buffer,"\n")] = '\0';
        write_buffer_content_length = strlen(write_buffer);
    }

    ssize_t bytes_written = write(fd, write_buffer, write_buffer_content_length);

    if (bytes_written == -1) {
        perror("[WRITE-ERROR] Failed to write into the file!!!\n");
        free(write_buffer);
        close(fd);
        return EXIT_FAILURE;
    }

    write_buffer[bytes_written] = '\0';

    printf("[SUCCESS] %zd Number of bytes written into the file...\n", bytes_written);
    printf("[WRITE] Content written into file : %s\n",write_buffer);

    cursor_location = lseek(fd,0,SEEK_CUR);
    printf("[AFTER-WRITE] Current cursor location in file at %ld location.\n", cursor_location);

    printf("\n\t---Writing data into the file using pwrite()---\n");

    char *pwrite_buffer = malloc(256 * sizeof(char));
    size_t pwrite_buffer_content_length = 0;

    printf("Enter the data to write into file : ");

    if (fgets(pwrite_buffer,256,stdin)!= nullptr) {
        pwrite_buffer[strcspn(pwrite_buffer,"\n")] = '\0';
        pwrite_buffer_content_length = strlen(pwrite_buffer);
    }

    ssize_t pwrite_bytes = pwrite(fd,pwrite_buffer,pwrite_buffer_content_length, (int)(write_buffer_content_length + 1));

    if (pwrite_bytes == -1) {
        perror("[PWRITE-ERROR] Failed to write into the file!!!\n");
        free(write_buffer);
        free(pwrite_buffer);
        close(fd);
        return EXIT_FAILURE;
    }

    pwrite_buffer[pwrite_bytes] = '\0';

    printf("[SUCCESS] %zd Number of bytes written into the file...\n", pwrite_bytes);
    printf("[PWRITE] Content written into file : %s\n",pwrite_buffer);

    cursor_location = lseek(fd,0,SEEK_CUR);
    printf("[AFTER-PWRITE] Current cursor location in file at %ld location.\n", cursor_location);

    free(write_buffer);
    free(pwrite_buffer);
    close(fd);

    return EXIT_SUCCESS;
}

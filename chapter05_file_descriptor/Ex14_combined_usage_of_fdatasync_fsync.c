
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    int fd = open("File00_test_file.txt", O_WRONLY | O_APPEND);

    if (fd == -1) {
        printf("[ERROR] File open error!!\n");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File open success...\n");

    char write_buffer[] = "Line from Ex13_combined_usage_of_fdatasync_fsync.c : Usage of both fsync & fdatasync! ";
    char *write_buffer_ptr = write_buffer;
    size_t write_buffer_len = strlen(write_buffer);
    ssize_t written_bytes = 0;

    // For reading all bytes scenario applied here
    while (write_buffer_len!=0 && (written_bytes = write(fd, write_buffer_ptr, write_buffer_len)) != 0) {
        if (written_bytes == -1) {
            if (errno == EINTR) {
                printf("[ALERT] Got interrupt signal!!\n");
                continue;  // If you want terminate to EINTR signal, you can use return EXIT_FAILURE;
            }
            perror("[ERROR] Write error!!");
            return EXIT_FAILURE;
        }

        write_buffer_ptr += written_bytes;
        write_buffer_len -= written_bytes;
    }

    *write_buffer_ptr = '\0';

    if (fsync(fd) == -1) {
        printf("[ERROR] Fsync error, might file doesn't have metadata or not support file type!!\n");
        if (errno == EINVAL) {
            if (fdatasync(fd) == -1) {
                printf("[ERROR] Fdatasync error!!\n");
                printf("[WARNING-LAZY_SYNC] Now it's kernel will control sync written data update to disk!\n");
            }else {
                printf("[CRITICAL-ERROR] Not supported file for synchronization of both data and metadata!!\n");
                return EXIT_FAILURE;
            }
            printf("[SUCCESS] Fdatasync done: FORCE_DATA_SYNC_ONLY happen! But, Metadata will be done kernel in lazy-sync mode only!!\n");
        }
    }else {
        printf("[SUCCESS] Fsync done: FORCE_DATA_AND_METADATA_SYNC happen!!\n");
    }

    printf("Number of bytes written: %zd\n", written_bytes);
    printf("Content written to file: %s\n",write_buffer);

    close(fd);
    return EXIT_SUCCESS;
}

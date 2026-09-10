#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>

int main() {

    int fd = open("File15_Fsync_With_Create_Flag.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);

    if (fd == -1) {
        printf("[ERROR] File open or create error!!\n");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File created and access success...\n");

    char write_buffer[] = "Line from Ex14_fsync_and_create_flag_case.c : "
                          "Usage fsync & new file created by program using create flag! ";
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
        printf("[ERROR] Fsync error!!\n");
    }
    else {
        printf("[SUCCESS] Fsync done: FORCE_DATA_AND_METADATA_SYNC happen!!\n");
    }

    close(fd);

    int fd_dir = open("/home/usr/Clion/Linux-System-Programming/chapter05_file_descriptor/",O_RDONLY);

    if (fsync(fd_dir) == -1) {
        printf("[ERROR] Parent Directory fsync error!!\n");
        printf("[WARNING-LAZY_SYNC] Kernel will take the control to sync newly created file to parent directory!!\n");
    }

    printf("[SUCCESS] Fsync of parent directory success...\n");

    printf("Number of bytes written: %zd\n", written_bytes);
    printf("Content written to file: %s\n",write_buffer);

    close(fd_dir);

    return EXIT_SUCCESS;
}

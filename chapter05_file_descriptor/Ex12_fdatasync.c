#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char file_path[] = "File00_test_file.txt";
    int fd = open(file_path, O_WRONLY | O_APPEND);

    if (fd == -1) {
        printf("[ERROR] Failed to open file %s\n", file_path);
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] File %s opened successfully\n",file_path);

    char write_buffer[] = "Line from Ex12_fdatasync.c : Fdatasync related data!";

    ssize_t written_bytes = write(fd, write_buffer, strlen(write_buffer));

    if (written_bytes == -1) {
        printf("[ERROR] Failed to write to file %s !!\n",file_path);
        return EXIT_FAILURE;
    }

    write_buffer[written_bytes] = '\0';

    if (fdatasync(fd) == -1) {
        printf("[ERROR] Failed to fsync file %s to hard disk!!\n",file_path);
        printf("[Warning]It's now in the control of kernel...\n");
    }

    printf("[SUCCESS] File's data only successfully stored to existing file of {%s} to hard disk.. \n", file_path);

    printf("Number of bytes written : %zd\n",written_bytes);
    printf("Written content into file : %s\n",write_buffer);

    close(fd);

    return EXIT_SUCCESS;
}

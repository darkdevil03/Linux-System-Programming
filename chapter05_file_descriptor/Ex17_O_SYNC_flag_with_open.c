
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/fcntl-linux.h>

int main() {

    int fd = open("File17_O_SYNC_flag_with_open.txt", O_CREAT | O_WRONLY | O_APPEND |O_SYNC, 0644);

    if (fd == -1) {
        printf("[ERROR] Failed to create/open file for writing.\n");
        return EXIT_FAILURE;
    }

    char write_buffer[] = "Line from Ex17_O_SYNC_flag_with_open.c : O_SYNC usage for write flag.. \n";
    ssize_t written_bytes = write(fd, write_buffer, strlen(write_buffer));

    if (written_bytes == -1) {
        printf("[ERROR] Failed to write to file for writing!!\n");
        close(fd);
        return EXIT_FAILURE;
    }

    write_buffer[written_bytes] = '\0';

    printf("Number of bytes written : %zd\n", written_bytes);
    printf("Content Written into file : %s\n", write_buffer);

    close(fd);

    return EXIT_SUCCESS;
}

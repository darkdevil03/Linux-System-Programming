#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {

    char *fifo_path = "/tmp/ex02a_fifo_pipes";

    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("[ERROR] Failed to fifo file path!!");
        return EXIT_FAILURE;
    }

    char buffer[25] = "This data from client";
    ssize_t bytes_write = write(fd, buffer, strlen(buffer));
    if (bytes_write == -1) {
        perror("[CLIENT-ERROR] Failed to read data of client-side!");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("[CLIENT] Sent %zd bytes data \n", bytes_write);

    close(fd);

    return EXIT_SUCCESS;
}

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    char *fifo_path = "/tmp/ex02a_fifo_pipes";

    // 1. Open the existing FIFO for writing.
    // This unlocks the server's blocking open() call.
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("[ERROR] Failed to fifo file path!!");
        return EXIT_FAILURE;
    }

    // 2. Write data into the named pipe
    printf("Client is sending a message...\n");
    char msg[25] = "This data from client";

    ssize_t bytes_write = write(fd, msg, strlen(msg));
    if (bytes_write == -1) {
        perror("[CLIENT-ERROR] Failed to write data for server-side!");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("[CLIENT] Sent %zd bytes data \n", bytes_write);

    // 3. Close the file descriptor
    close(fd);
    return 0;
}
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {

    char *fifo_path = "/tmp/ex02a_fifo_pipes";
    mkfifo(fifo_path, 0666);
    printf("[SERVER] Waiting the request from client\n");
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("[ERROR] Failed to fifo file path!!");
        return EXIT_FAILURE;
    }

    char buffer[25];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer)+1);
    if (bytes_read == -1) {
        perror("[SERVER-ERROR] Failed to read data of client-side!");
        close(fd);
        return EXIT_FAILURE;
    }
    buffer[bytes_read] = '\0';

    printf("[SERVER] Received %zd bytes\n[DATA]: %s\n", bytes_read,buffer);

    close(fd);

    return EXIT_SUCCESS;
}

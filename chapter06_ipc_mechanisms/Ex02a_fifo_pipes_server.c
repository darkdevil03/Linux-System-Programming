#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main() {
    char *fifo_path = "/tmp/ex02a_fifo_pipes";
    char buffer[100];

    // 1. Create the Named Pipe (FIFO) special file
    // 0666 provides read/write permissions
    mkfifo(fifo_path, 0666);

    printf("[SERVER] Server is waiting for a client to open the FIFO...\n");

    // 2. Open the FIFO for reading.
    // This blocks until another process opens it for writing!
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("[ERROR] Failed to fifo file path!!");
        return EXIT_FAILURE;
    }

    // 3. Read the data sent by the client
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("[SERVER-ERROR] Failed to read data from client-side!");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0'; // Null-terminate
    printf("[SERVER] Received %zd bytes \n"
           "\t|-->[RECIVED-DATA]: %s\n", bytes_read,buffer);


    // 4. Close the file descriptor
    close(fd);
    return 0;
}
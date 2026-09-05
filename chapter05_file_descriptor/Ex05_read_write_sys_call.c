#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    // For O_RDWR, O_CREAT flags
#include <unistd.h>   // For open(), read(), write(), close()

int main() {
    int fd;
    const char *text = "Hello, Linux File Descriptors!\n";
    char read_buf[100];

    // 1. Open (or create) a file. Returns the File Descriptor (an integer).
    // O_RDWR: Read/Write mode
    // O_CREAT: Create file if it doesn't exist
    // O_TRUNC: Truncate file to 0 length if it exists
    // 0644: Permissions (rw-r--r--)
    fd = open("File00_test_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }
    printf("Successfully opened file. Assigned FD: %d\n", fd);

    // 2. Write data to the kernel via the FD
    ssize_t bytes_written = write(fd, text, strlen(text));
    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }
    printf("Wrote %zd bytes to FD %d\n", bytes_written, fd);

    // 3. Rewind the file offset back to the beginning to read it
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking");
        close(fd);
        return 1;
    }

    // 4. Read the data back from the kernel
    ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes_read == -1) {
        perror("Error reading from file");
        close(fd);
        return 1;
    }

    // Null-terminate the buffer to print it safely as a string
    read_buf[bytes_read] = '\0';
    printf("Read %zd bytes from FD %d: %s", bytes_read, fd, read_buf);

    // 5. Release the file descriptor back to the OS
    if (close(fd) == -1) {
        perror("Error closing file");
        return 1;
    }
    printf("Closed FD %d successfully.\n", fd);

    return 0;
}
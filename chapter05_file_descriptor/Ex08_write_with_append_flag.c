

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>

int main() {
    char file_name[] = "File08_Append_Write_Operation.txt";
    int fd = open(file_name,O_RDWR | O_APPEND);
    // O_RDWR used for before-write view the content then performing write combined with append flag,
    // For results then check that open file is append or over-write the existing content of that file what we wrote using write(). .

    if (fd == -1) {
        perror("[ERROR] Failed to open the given file!!\n");
        return EXIT_FAILURE;
    }
    printf("|-------------------------------------------------------------------------------------|\n");
    printf("|      [SUCCESS] Given file is exist. Now the program's operation will preform.       |\n");
    printf("|-------------------------------------------------------------------------------------|\n\n");

    printf("|-------------------------------------------------------------------------------------|\n");
    printf("|       [READ] Before writing to the file. Here, we are viewing the file content.     |\n");
    printf("|-------------------------------------------------------------------------------------|\n");

    char read_buffer[30];
    char *read_buffer_tracker = read_buffer;
    size_t read_buffer_len = sizeof(read_buffer);
    ssize_t bytes_read = 0;
    ssize_t total_bytes_read = 0;

    while ( read_buffer_len != 0 && (bytes_read = read(fd,read_buffer,read_buffer_len)) != 0) {
        if (bytes_read == -1) {
            if (errno == EINTR) {
                printf("[SIGNAL] Got interrupt signal (EINTR)\n");
                continue;
            }
            printf("[ERROR] Failed to read from the file!!\n");
            return EXIT_FAILURE;
        }

        read_buffer_len -= bytes_read;
        read_buffer_tracker += bytes_read;
        total_bytes_read += bytes_read;
    }

    *read_buffer_tracker = '\0';

    printf("|-->\tNumber of bytes read : %zd\n", total_bytes_read);
    printf("|-->\tRead data content : %s\n", read_buffer);
    printf("|-------------------------------------------------------------------------------------|\n\n");

    printf("|-------------------------------------------------------------------------------------|\n");
    printf("|        [WRITE] Writing data to the file with append flag.                           |\n");
    printf("|-------------------------------------------------------------------------------------|\n");
    char write_buffer[] = " This data from Ex08_write_with_append_flag.c program. ";
    size_t write_buffer_len = strlen(write_buffer);
    ssize_t bytes_wrote = write(fd,write_buffer,write_buffer_len);

    write_buffer[bytes_wrote] = '\0';

    printf("|-->\tNumber of bytes written : %zd\n", bytes_wrote);
    printf("|-->\tAppended data : %s\n", write_buffer);
    printf("|-------------------------------------------------------------------------------------|\n");

    printf("\n\t\t[APPEND SUCCESS] Now open the %s file to check for append or over-write!!\n",file_name);

    close(fd);
    return EXIT_SUCCESS;
}

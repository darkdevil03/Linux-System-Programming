#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd_append_flag_index = open("/File09_New_Write_File_Created.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd_append_flag_index == -1) {
        printf("[ERROR] File not exist!!\n");
        return EXIT_FAILURE;
    }
    printf("[SUCCESS] This program created new .txt file with name: File09_New_Write_File_Created.txt ...\n");

    printf("Enter your text to write into newly created txt file : ");
    char text_buffer[256];
    fgets(text_buffer,sizeof(text_buffer),stdin);
    printf("\n");

    // Here, write overwrite the new data into existing file not append that you wrote.
    ssize_t byte_written = write(fd_append_flag_index,text_buffer,strlen(text_buffer));

    if (byte_written == -1) {
        printf("[ERROR] Failed to write data into newly created file ");
        close(fd_append_flag_index);
        return EXIT_FAILURE;
    }
    printf("[SUCCESS] Written %zd bytes.\n", byte_written);


    if (close(fd_append_flag_index) == -1) {
        printf("[ERROR] Failed To Close File Descriptor");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

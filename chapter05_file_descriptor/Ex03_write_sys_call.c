#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    printf("Enter your text to write into File03_New_Write_File_Created.txt : ");
    char text_buffer[256];
    fgets(text_buffer,sizeof(text_buffer),stdin);
    printf("\n");

    int fd_append_flag_index = open("File03_New_Write_File_Created.txt", O_WRONLY | O_CREAT , 0644);

    if (fd_append_flag_index == -1) {
        printf("[ERROR] Failed to creating new file if not exist!!\n");
        return -1;
    }
    printf("[SUCCESS] This program mapped to New_Write_File_Created.txt..\n");

    ssize_t byte_written = write(fd_append_flag_index,text_buffer,strlen(text_buffer));

    if (byte_written == -1) {
        printf("[ERROR] Failed to write data into newly created file ");
        close(fd_append_flag_index);
        return -1;
    }
    printf("[SUCCESS] Written %zd bytes.\n", byte_written);


    if (close(fd_append_flag_index) == -1) {
        printf("[ERROR] Failed To Close File Descriptor");
        return -1;
    }

    return 0;
}

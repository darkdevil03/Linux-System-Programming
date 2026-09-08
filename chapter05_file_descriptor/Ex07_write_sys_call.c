#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    char file_path[] = "File07_Existing_File_Write_Operations.txt";
    int fd_append_flag_index = open(file_path, O_WRONLY);

    if (fd_append_flag_index == -1) {
        printf("[ERROR] File not exist!!\n");
        return EXIT_FAILURE;
    }
    printf("|-------------------------------------------------------------------------------------|\n");
    printf("|      [SUCCESS] Given file is exist. Now the program's operation will preform.       |\n");
    printf("|-------------------------------------------------------------------------------------|\n\n");
    printf("|-------------------------------------------------------------------------------------|\n");
    printf("|                 Before write(), see the file content for difference!                |\n");
    printf("|-------------------------------------------------------------------------------------|\n");

    printf("|---> Now, enter your text below to write into %s\n\t|----> Type here your data: ",file_path);
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

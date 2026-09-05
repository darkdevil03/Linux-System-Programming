#include <fcntl.h>
#include <stdio.h>

int main() {

    // O_CREAT
    int fd_append_flag_index = open("File02_New_File_Created.txt", O_RDWR | O_CREAT, 0644);
    // If File02_New_File_Created.txt and this program exist in same directory absolute is optional.
    // Incase of File02_New_File_Created.txt is available in another folder then we need to absolute path for it.
    // But the File02_New_File_Created.txt is not available in current directory we use flag O_CREATE for create a .txt with given name(File02_New_File_Created.txt) in this program directory.

    if (fd_append_flag_index == -1) {
        printf("[ERROR] Failed to creating new file if not exist!!\n");
    }else {
        printf("[Success] File02_New_File_Created.txt is created..\n");
    }


    return 0;
}
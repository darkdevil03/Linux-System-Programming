#include <fcntl.h>
#include <stdio.h>

int main() {
    char path[] = "/home/xap/Clion/Linux-System-Programming/chapter05_file_descriptor/hello.txt";
    int fd_index = open(path, O_RDONLY);
    if (fd_index == -1) {
        printf("[ERROR] Failed to locate the file from hard disk to RAM and fd via sys_call");
    }else {
        printf("Successfully located the path file from hard disk to RAM and fd via sys_call");
    }

    return 0;
}
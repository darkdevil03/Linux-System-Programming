#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {

    printf("Enter the absolute path, relative path, or file (i.e existing in this program diectory) : ");
    char file_path[512];
    //fgets(file_path,sizeof(file_path),stdin);
    printf("Reading File...\n");

    int fd = open("File01_hello.txt",O_RDONLY);

    if (fd == -1) {
        perror("[ERROR] File is not exist!!\n");
        close(fd);
        return -1;
    }

    char read_buf[20];
    ssize_t read_bytes = read(fd,read_buf,20);

    if (read_bytes == -1) {
        printf("[ERROR] File was corrupted or got signal!!\n");
        close(fd);
        return -1;
    }

    printf("Number od bytes read : %zd\n",read_bytes);
    printf("Read content : %s\n",read_buf);

    return 0;
}

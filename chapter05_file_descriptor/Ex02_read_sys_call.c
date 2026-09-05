#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {

    printf("Enter the absolute path, relative path, or file name : ");
    char file_path[512];
    if (fgets(file_path,sizeof(file_path),stdin) == NULL) {
        perror("[ERROR] Don't enter null parameter!!");
        return -1;
    }
    file_path[strcspn(file_path,"\n")] = '\0';
    printf("Reading File...\n");

    int fd = open(file_path,O_RDONLY);

    if (fd == -1) {
        perror("[ERROR] Enter correct file name ");
        return -1;
    }

    char read_buf[21];
    ssize_t read_bytes = read(fd,read_buf,20);

    if (read_bytes == -1) {
        printf("[ERROR] File was corrupted or got signal!!\n");
        close(fd);
        return -1;
    }

    // Safely cap off the string with a null terminator based on how many bytes were actually read
    read_buf[read_bytes] = '\0';

    printf("Number of bytes read : %zd\n",read_bytes);
    printf("Read content : %s\n",read_buf);

    return 0;
}

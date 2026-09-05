
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {

    int fd = open ("File00_test_file.txt",O_RDONLY);

    if (fd == -1) {
        perror("[ERROR] File descriptor failed create!!!\n");
        return -1;
    }

    char *buf[21];
    ssize_t bytes_read;
    int len = 20;

    while (len != 0 && ((bytes_read = read(fd,*buf,len)!= 0) )) {

        if (bytes_read == -1) {
            if (errno == EINT) {
                continue;
            }
            perror("[ERROR] Failed to read from file!!");
            break;
        }

        len = len - bytes_read;
        buf = buf + bytes_read;
    }

    return 0;
}

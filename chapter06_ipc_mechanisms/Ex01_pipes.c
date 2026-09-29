#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int pipe_fds[2];

    if (pipe(pipe_fds) == -1) {
        perror("[ERROR] pipe() failed !!");
        return EXIT_FAILURE;
    }

    int child = fork();

    if (child == -1) {
        perror("[ERROR] fork() failed !!");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return EXIT_FAILURE;
    }

    if (child == 0) {
        close(pipe_fds[1]);

        printf("[CHILD-%d] I am reading data from pipe\n", getpid());
        char buffer[25];

        ssize_t bytes_read = read(pipe_fds[0], buffer, sizeof(buffer));
        buffer[bytes_read] = '\0';
        printf("%zd Bytes read from pipe fds\n", bytes_read);
        printf("Read Data : %s\n", buffer);

        close(pipe_fds[0]);

        exit(0);
    }else {
        close(pipe_fds[0]);

        printf("[PARENT-%d] I am writing data into pipe\n",getpid());

        char buffer[25] = "Hello World!";
        ssize_t bytes_write = write(pipe_fds[1],buffer,strlen(buffer));
        printf("%zd Bytes written to pipe fds \n", bytes_write);

        close(pipe_fds[1]);
        wait(nullptr);
    }


    return EXIT_SUCCESS;
}

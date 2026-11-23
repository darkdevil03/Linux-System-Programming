#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

//int socketcreator(int domain, int type, int protocol);

int main() {
    int sock;
    sock = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("[ERROR] Failed to create a socket!");
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] LOCAL-TCP Socket created...\n");

    return EXIT_SUCCESS;
}
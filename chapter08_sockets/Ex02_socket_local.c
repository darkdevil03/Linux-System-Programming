#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main() {

    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("[ERROR] Could not create socket\n");
    }

    printf("[SUCCESS] INET-TCP Socket created...\n");

    return EXIT_SUCCESS;
}

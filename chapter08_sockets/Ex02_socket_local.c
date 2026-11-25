#define _XOPEN_SOURCE 700

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static int send_all(int socket_fd, const void *buffer, size_t length) {
    const char *data = buffer;
    size_t sent = 0;

    while (sent < length) {
        ssize_t result = send(socket_fd, data + sent, length - sent, 0);
        if (result == -1 && errno == EINTR) {
            continue;
        }
        if (result <= 0) {
            if (result == 0) {
                errno = EPIPE;
            }
            return -1;
        }
        sent += (size_t) result;
    }

    return 0;
}



int main(void) {
    char socket_directory[] = "/tmp/ex02_socket_local_XXXXXX";
    char socket_path[sizeof(((struct sockaddr_un *) 0)->sun_path)];
    struct sockaddr_un address = {0};
    const char request[] = "Hello from the local socket client";
    const char response[] = "Hello from the local socket server";
    char received_message[sizeof(request)];
    int server_fd = -1;
    int client_fd = -1;
    int connection_fd = -1;
    int directory_created = 0;
    int socket_bound = 0;
    int result = EXIT_FAILURE;

    /* Step 1: Create the server socket. */
    server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket (server)");
        goto cleanup;
    }

    if (mkdtemp(socket_directory) == NULL) {
        perror("mkdtemp");
        goto cleanup;
    }
    directory_created = 1;

    if (snprintf(socket_path, sizeof(socket_path), "%s/socket", socket_directory) >=
        (int) sizeof(socket_path)) {
        fprintf(stderr, "Socket path is too long\n");
        goto cleanup;
    }

    address.sun_family = AF_LOCAL;
    memcpy(address.sun_path, socket_path, strlen(socket_path) + 1);

    /* Step 2: Bind the server socket to its local filesystem address. */
    socklen_t address_length = (socklen_t) (offsetof(struct sockaddr_un, sun_path) +
                                            strlen(socket_path) + 1);
    if (bind(server_fd, (struct sockaddr *) &address, address_length) == -1) {
        perror("bind");
        goto cleanup;
    }
    socket_bound = 1;

    /* Step 3: Listen for a client connection. */
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        goto cleanup;
    }

    /* Step 4: Create the client socket. */
    client_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket (client)");
        goto cleanup;
    }

    /* Step 5: Connect the client socket to the server. */
    if (connect(client_fd, (struct sockaddr *) &address, address_length) == -1) {
        perror("connect");
        goto cleanup;
    }

    /* Step 6: Accept the pending client connection. */
    connection_fd = accept(server_fd, NULL, NULL);
    if (connection_fd == -1) {
        perror("accept");
        goto cleanup;
    }

    /* Step 7: Exchange messages over the connected sockets. */
    if (send_all(client_fd, request, sizeof(request)) == -1) {
        perror("send (request)");
        goto cleanup;
    }
    if (receive_all(connection_fd, received_message, sizeof(received_message)) == -1) {
        perror("recv (request)");
        goto cleanup;
    }
    printf("[SERVER] Received: %s\n", received_message);

    if (send_all(connection_fd, response, sizeof(response)) == -1) {
        perror("send (response)");
        goto cleanup;
    }
    if (receive_all(client_fd, received_message, sizeof(received_message)) == -1) {
        perror("recv (response)");
        goto cleanup;
    }
    printf("[CLIENT] Received: %s\n", received_message);

    result = EXIT_SUCCESS;

cleanup:
    if (connection_fd != -1) {
        close(connection_fd);
    }
    if (client_fd != -1) {
        close(client_fd);
    }
    if (server_fd != -1) {
        close(server_fd);
    }
    if (socket_bound) {
        unlink(socket_path);
    }
    if (directory_created) {
        rmdir(socket_directory);
    }

    return result;
}

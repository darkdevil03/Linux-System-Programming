#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    fd_set watch_list;

    struct timeval timeout;
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    FD_ZERO(&watch_list);

    printf("[SELECT] I am going for sleep for 15sec using select()\n");
    int ret = select( 0, nullptr, nullptr, nullptr, &timeout);

    if (ret == -1) {
        printf("[ERROR] Failed to invoke select() !!!\n");
        return EXIT_FAILURE;
    }

    printf("[SELECT] Kernel wake-up me  after 15sec!!\n");

    return EXIT_SUCCESS;
}

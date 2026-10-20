#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* back_ground_task(void* args) {

    printf("[BACKGROUND] Thread started, work in progress... \n");
    sleep(15);
    printf("[BACKGROUND] Thread ended, resource reclaim by OS. \n]");

    return NULL;
}

int main() {


    return EXIT_SUCCESS;
}

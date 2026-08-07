#include <stdio.h>
#include <unistd.h>

int main () {

    printf("Hello ppl, This is new program!!!\n");
    printf("New program \n");
    printf("     |-------------> pid : %d\n", getpid());
    printf("     |-------------> ppid : %d\n", getppid());

    return 0;
}

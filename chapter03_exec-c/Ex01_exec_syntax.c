#include<stdio.h>
#include <unistd.h>

int main() {

    printf("Hello PPL, This exec_syntax program!!!\n");
    printf("exec_syntax program \n");
    printf("     |-------------> pid : %d\n", getpid());
    printf("     |-------------> ppid : %d\n", getppid());

    execl("./new_program", "new_program", nullptr);

    return 0;
}

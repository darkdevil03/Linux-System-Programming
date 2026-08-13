#include <stdio.h>
#include <unistd.h>

int main() {

    printf("Hey, I am Ex00_signal_test_sample.c program\n");
    printf("My pid is %d\n", getpid());
    while (1) {
        printf("I am sleeping...\n");
        sleep(2);
    }

    return 0;
}

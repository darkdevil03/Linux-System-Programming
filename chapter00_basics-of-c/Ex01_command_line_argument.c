#include <stdio.h>

int main(int argc, char *argv[]) {
    // argc stores the number of arguments passed (including the program name itself)
    printf("Total arguments provided: %d\n\n", argc);

    // Loop through the argv array to print each argument
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    return 0;
}
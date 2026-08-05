/**
    To use command-line arguments,
        We need to modify your main function to accept argc (argument count) and argv (argument vector/values).
 */

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
/*
     In terminal go to this program directory then:
        -> syntax : gcc_compiler program_name.c

         $ gcc Ex01_command_line_argument.c

         -> ./a.out arguments
         $ ./a.out ram dev

    sample output:
            Total arguments provided: 3

            Argument 0: ./a.out
            Argument 1: ram
            Argument 2: dev


        Note: The first argument (argv[0]) will always be the name/path of the executable itself (e.g., ./my_project),
              and the subsequent arguments will be the words you typed after it.
 */
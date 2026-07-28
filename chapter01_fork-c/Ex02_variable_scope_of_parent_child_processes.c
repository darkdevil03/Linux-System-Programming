#include <stdio.h>
#include <stdlib.h>  // dynamic memory
#include <unistd.h> // fork
#include <sys/wait.h> // wait

int l = 78; // global value

// Note: the order of calling process's statements. it's depends on the process scheduler of subsystem!
// Therefore, the output lines order will changes while run them multiple times!

int main()
{
    int *ptr = malloc(1024 * 1024); // Allocate 1MB
    *ptr = 42;


    pid_t pid = fork(); // <--- COW HAPPENS HERE

    if (pid == 0) {
        // Child Process
        // At this point, Child and Parent share the SAME 1MB page.
        // No copy happened yet.

        // IF we read:
        printf("\n|-------------------------------------------|\n");
        printf("|           Child Process start!            |\n");
        printf("|-------------------------------------------|\n");
        printf("Child : \n"); // Fast! Just a read.
        printf("   |-------> Ptr = %d\n",*ptr);

        // IF we write:
        *ptr = 99;          // TRAP!
                            // Kernel stops here.
                            // Allocates NEW 1MB page.
                            // Copies 1MB.
                            // Updates Child's pointer.
                            // Resumes.
        // Now Child has its own 1MB. Parent still has 42.
        printf("Child process modify the ptr value to : %d\n",*ptr);
        printf("Child process done! \n");
    }else if (pid > 0) {
        // Parent Process
        printf("|-------------------------------------------|\n");
        printf("|            Parent Process start!          |\n");
        printf("|-------------------------------------------|\n");
        printf("Parent: \n");
        printf("   |-------> Child PID = %d\n", pid);
        printf("   |-------> Ptr = %d\n",*ptr);

        *ptr=*ptr+1;
        printf("Incremented ptr by 1 : %d\n",*ptr);

        printf("\nWaiting for child process complete!,Then other statements of parent process will be completes \n");

        // Wait for child to finish
        wait(nullptr); // Requires <sys/wait.h>

        printf("\n|-------------------------------------------|\n");
        printf("|    Parent start remaining instructions!   |\n");
        printf("|-------------------------------------------|\n");

        // Parent's view of *ptr remains unchanged (42)
        printf("Parent: \n");
        printf("   |-------> Ptr = %d, value is unchanged by child process !\n ",*ptr);
        printf("Parent process instructions completed successfully! \n");
        } else {
            perror("fork failed");
            return 1;
        }

    free(ptr); // ptr is free from both parent and child memory layout independently.
    // sample for above comment line!
    printf("\n\t I called by both parent and child before their last statement!!\n");
    return 0;
}

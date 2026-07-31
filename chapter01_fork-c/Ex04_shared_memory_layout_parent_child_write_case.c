/**
 * @file cow_memory_layout.c
 * @brief Demonstrates Linux Copy-on-Write (COW) mechanism and process memory segments.
 * @details This program shows how fork() replicates virtual memory layouts,
 *          sharing physical pages until a write operation triggers an OS-level copy.
 */

#include <stdio.h>
#include <stdlib.h>   // Required for dynamic memory allocation (malloc, free)
#include <unistd.h>   // Required for fork() system call
#include <sys/wait.h> // Required for wait() process synchronization

/* ========================================================================== */
/* MEMORY SEGMENT: INITIALIZED DATA SEGMENT (.data)                           */
/* ========================================================================== */
int global_initialized_var = 78; // Stored in the .data segment

/* ========================================================================== */
/* MEMORY SEGMENT: UNINITIALIZED DATA SEGMENT (.bss)                          */
/* ========================================================================== */
int global_uninitialized_var;    // Stored in the .bss segment (automatically zeroed)

int main(void)
{
    /* ====================================================================== */
    /* MEMORY SEGMENT: STACK & HEAP                                           */
    /* ====================================================================== */
    int local_stack_var = 10;     // Stored on the Stack (local to main)

    // Allocate 1MB on the Heap using malloc()
    int *ptr = (int *)malloc(1024 * 1024);
    if (ptr == NULL) {
        perror("Allocation failed");
        return 1;
    }
    *ptr = 42; // Initialize the allocated heap memory

    // Note: The execution order of parent and child processes depends entirely
    // on the OS process scheduler. Outputs may interleave on multiple runs.

    /* ====================================================================== */
    /* PROCESS DUPLICATION VIA FORK()                                         */
    /* ====================================================================== */
    pid_t pid = fork(); // <--- COPY-ON-WRITE (COW) HAPPENS HERE

    if (pid < 0) {
        // Error handling if fork fails
        perror("fork failed");
        free(ptr);
        return 1;
    }
    else if (pid == 0) {
        /* ================================================================== */
        /* CHILD PROCESS CONTEXT                                              */
        /* ================================================================== */
        // At this exact point, Child and Parent share the exact SAME physical
        // 1MB heap page. No physical duplication has occurred yet.

        printf("\n|-------------------------------------------|\n");
        printf("|            Child Process start!           |\n");
        printf("|-------------------------------------------|\n");

        // Reading is fast because no memory copy is needed
        printf("Child  : Initial read of shared Ptr = %d\n", *ptr);

        // WRITE OPERATION: Modifying the shared memory block
        *ptr = 99; // TRAP!
                   // 1. CPU hardware triggers a protection fault.
                   // 2. Kernel stops the child process momentarily.
                   // 3. Kernel allocates a NEW physical 1MB page in memory.
                   // 4. Kernel copies the old 1MB content into the new page.
                   // 5. Kernel updates child's page table mapping to the new page.
                   // 6. Child process resumes execution safely.

        printf("Child  : Modified Ptr value to = %d (Triggered COW)\n", *ptr);
        printf("Child process execution completed successfully!\n");
    }
    else {
        /* ================================================================== */
        /* PARENT PROCESS CONTEXT                                             */
        /* ================================================================== */
        printf("\n|-------------------------------------------|\n");
        printf("|            Parent Process start!          |\n");
        printf("|-------------------------------------------|\n");

        printf("Parent : Child PID created = %d\n", pid);
        printf("Parent : Initial read of Ptr = %d\n", *ptr);

        // Modify parent's pointer value independently
        *ptr = *ptr + 1;
        printf("Parent : Incremented Ptr by 1 to = %d\n", *ptr);

        printf("\nParent : Waiting for child process to complete...\n");

        // Synchronize: Block parent execution until the child process terminates
        wait(NULL);

        printf("\n|-------------------------------------------|\n");
        printf("|    Parent resumed remaining instructions! |\n");
        printf("|-------------------------------------------|\n");

        // Parent's view of *ptr remains isolated from the child's modifications
        printf("Parent : Final Ptr value = %d (Unchanged by child process due to COW)\n", *ptr);
        printf("Parent process instructions completed successfully!\n");
    }

    /* ====================================================================== */
    /* CLEANUP INDEPENDENTLY                                                  */
    /* ====================================================================== */
    // Both processes reach here independently. Each process must free its own
    // virtual memory allocation mapping (Parent frees its page, Child frees its copy).
    free(ptr);

    printf("\n\t[INFO] Executed by both parent and child before termination.\n");

    return 0;
}
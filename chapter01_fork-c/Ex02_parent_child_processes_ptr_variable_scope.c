/**
    Scenario: Demonstrates pointer scope and virtual memory independence across processes.

    Details:
            Explains how local pointer variables are duplicated on individual process
            stacks during fork(), while sharing underlying physical memory via COW.

     Note: The order of calling process's statements. it's depends on the process scheduler of subsystem!
           Therefore, the output lines order will changes while run them multiple times!
 */

#include <stdio.h>
#include <stdlib.h>   // Required for dynamic memory management (malloc, free)
#include <unistd.h>   // Required for fork() system call
#include <sys/wait.h> // Required for wait() process synchronization

/* ========================================================================== */
/* GLOBAL VARIABLE SCOPE (.data segment)                                      */
/* ========================================================================== */
int global_val = 78; // Accessible to both processes, but changes do not sync back.

/*
 * NOTE: The execution order of parent and child processes depends entirely
 * on the operating system's process scheduler. Output lines may interleave
 * differently across multiple runs.
 */

int main()
{
    /* ====================================================================== */
    /* STACK & HEAP INITIALIZATION                                            */
    /* ====================================================================== */
    // 'ptr' is a local variable residing on the Stack of the main process.
    // It holds a virtual memory address pointing to a 1MB block on the Heap.
    int *ptr = (int *)malloc(1024 * 1024);
    if (ptr == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    *ptr = 42; // Storing a value inside the dynamically allocated heap memory

    /* ====================================================================== */
    /* PROCESS CREATION VIA FORK()                                            */
    /* ====================================================================== */
    pid_t pid = fork(); // <--- STACK DUPLICATION & COW HAPPENS HERE

    if (pid < 0) {
        // Error handling if process creation fails
        perror("fork failed");
        free(ptr);
        return 1;
    }
    else if (pid == 0) {
        /* ================================================================== */
        /* CHILD PROCESS SCOPE                                                */
        /* ================================================================== */
        // The child process receives an exact copy of the parent's stack.
        // Therefore, the child has its own independent local variable named 'ptr'
        // which holds the same virtual address value as the parent's 'ptr'.

        printf("\n|-------------------------------------------|\n");
        printf("|            Child Process start!           |\n");
        printf("|-------------------------------------------|\n");

        printf("Child  : Local stack pointer address value = %p\n", (void *)ptr);
        printf("Child  : Initial read through pointer (*ptr) = %d\n", *ptr);

        // NOTE: If we uncommented code to modify *ptr here, it would trigger COW,
        // isolating the child's heap memory completely from the parent.

        printf("Child process execution complete!\n");
    }
    else {
        /* ================================================================== */
        /* PARENT PROCESS SCOPE                                               */
        /* ================================================================== */
        printf("\n|-------------------------------------------|\n");
        printf("|            Parent Process start!          |\n");
        printf("|-------------------------------------------|\n");

        printf("Parent : Child process PID = %d\n", pid);
        printf("Parent : Local stack pointer address value = %p\n", (void *)ptr);
        printf("Parent : Initial read through pointer (*ptr) = %d\n", *ptr);

        // Modify the value at the heap address pointed to by parent's 'ptr'
        *ptr = *ptr + 1;
        printf("Parent : Modified heap value to = %d\n", *ptr);

        printf("\nParent : Waiting for the child process to terminate...\n");

        // Synchronize execution: block parent until the child process completes
        wait(nullptr);

        printf("\n|-------------------------------------------|\n");
        printf("|    Parent resumed remaining instructions! |\n");
        printf("|-------------------------------------------|\n");

        printf("Parent : Final pointer value (*ptr) = %d\n", *ptr);
        printf("Parent process instructions completed successfully!\n");
    }

    /* ====================================================================== */
    /*                      INDEPENDENT MEMORY CLEANUP                        */
    /* ====================================================================== */
    // Both parent and child execute this line independently because they are
    // separate processes. Each must free its own virtual memory mapping.
    free(ptr);

    printf("\n\t[INFO] Executed independently by both parent and child before exit.\n");

    return 0;
}
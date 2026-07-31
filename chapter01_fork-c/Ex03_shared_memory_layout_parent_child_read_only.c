/**
    Scenario: Demonstrates zero-cost shared memory reading without triggering Copy-on-Write (COW).

    Details:
            Explains how parent and child processes safely read from the same underlying
           physical memory page simultaneously without causing a kernel-level page copy.
 */

#include <stdio.h>
#include <stdlib.h>   // Required for dynamic memory allocation (malloc, free)
#include <unistd.h>   // Required for fork() system call
#include <sys/wait.h> // Required for wait() process synchronization

/* ========================================================================== */
/* GLOBAL VARIABLE SCOPE (.data segment)                                      */
/* ========================================================================== */
int global_val = 78; // Initialized global variable shared in concept

/* 
 * NOTE: The execution order of parent and child processes depends entirely 
 * on the operating system's process scheduler. Output lines may interleave 
 * differently across multiple runs.
 */

int main()
{
    /* ====================================================================== */
    /* HEAP ALLOCATION & INITIALIZATION                                       */
    /* ====================================================================== */
    // Allocate 1MB on the heap. 'ptr' holds the virtual address on the stack.
    int *ptr = (int *)malloc(1024 * 1024); 
    if (ptr == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    *ptr = 42; // Initialize the heap memory value to 42

    /* ====================================================================== */
    /* PROCESS CREATION VIA FORK()                                            */
    /* ====================================================================== */
    pid_t pid = fork(); // <--- PROCESS DUPLICATION OCCURS HERE

    if (pid < 0) {
        // Error handling if fork fails
        perror("fork failed");
        free(ptr);
        return 1;
    } 
    else if (pid == 0) {
        /* ================================================================== */
        /* CHILD PROCESS CONTEXT (READ-ONLY ACCESS)                           */
        /* ================================================================== */
        // At this point, the child shares the exact same physical memory page 
        // as the parent. No write is performed here.

        printf("\n|-------------------------------------------|\n");
        printf("|            Child Process start!           |\n");
        printf("|-------------------------------------------|\n");
        
        // READ OPERATION: Fast and zero-cost. 
        // No COW is triggered because the child only reads the value.
        printf("Child  : Reading shared memory value -> Ptr = %d (No COW triggered)\n", *ptr);

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
        printf("Parent : Initial read -> Ptr = %d\n", *ptr);

        // WRITE OPERATION: Parent modifies the value. 
        // This triggers a write operation in the parent's context.
        *ptr = *ptr + 1; 
        printf("Parent : Incremented ptr by 1 -> New Ptr = %d\n", *ptr);

        printf("\nParent : Waiting for the child process to complete...\n");

        // Synchronize execution: block parent until the child process finishes
        wait(NULL); 

        printf("\n|-------------------------------------------|\n");
        printf("|    Parent resumed remaining instructions! |\n");
        printf("|-------------------------------------------|\n");

        // Parent verifies its own isolated state post-synchronization
        printf("Parent : Final Ptr value = %d (Unaffected by child since child only read)\n", *ptr);
        printf("Parent process instructions completed successfully!\n");
    }

    /* ====================================================================== */
    /* INDEPENDENT MEMORY CLEANUP                                             */
    /* ====================================================================== */
    // Both parent and child reach this point independently and must free 
    // their respective virtual memory mapping references.
    free(ptr); 

    printf("\n\t[INFO] Executed by both parent and child before process termination.\n");
    
    return 0;
}
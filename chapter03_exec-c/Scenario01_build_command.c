/**
Scenario:
    Single-Shot Command Execution (Wrapper Shell)

Details:
    Demonstration of dynamically executing command-line arguments using execvp.
    Rather than running an infinite while loop to capture continuous user input,
    this implementation isolates and executes exactly one dynamic command passed via terminal arguments.
    It utilizes the fork() system call to create the duplicate child and relies on execvp() to
    automatically search the system's $PATH for the executable program.
    The parent process remains suspended via wait() until the child process terminates,
    allowing the parent to cleanly read the exit status.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int size, char *args[]) {
    // 1. Validate that the user actually provided a command to run
    if (size < 2) {
        printf("[AutoUserShell_Error] Usage: %s <command> [args...]\n", args[0]);
        return EXIT_FAILURE;
    }
    printf("[NOTE] AutoUserShell runs commands automatically with SUDO privileges!! \n\n");

    printf("SudoShell> ");
    for (int i = 1; i < size; i++) {
        printf("%s ",args[i]);
    }
    printf("\n");

    // 2. Create a duplicate process
    pid_t id = fork();

    if (id < 0) {
        perror("[ERROR] Failed to fork SUDO privileges .");
        return EXIT_FAILURE;
    }

    // 3. Child Process Execution Flow
    if (id == 0) {
        /*
         * args[1]: The command to execute ("ls")
         * &args[1]: The shifted argument array starting at "ls".
         * The OS automatically ensures a NULL pointer exists at the end of 'args'.
         */
        execvp(args[1], &args[1]);

        /*
         * --- CRITICAL CONCEPT ---
         * If execvp() is successful, the SudoShell process is completely replaced by args[1] such ls.
         * The lines below ONLY execute if execvp() fails (e.g., typo in command).
         */
        printf("[%s ERROR] execvp() failed",args[1]);
        exit(5);
    }
    else {
        // 4. Parent Process Execution Flow
        int status;

        // wait() blocks the parent until the child terminates
        pid_t terminated_child = wait(&status);

        // 5. Safely analyze how the child process terminated
        if (WIFEXITED(status)) {
            const int exit_status = WEXITSTATUS(status);

            printf("\n[AutoUserShell] SudoShell terminated and switched back to AutoUserShell with exit status: %d\n", exit_status);

            // If exit_status is 5, our error handling caught an execvp() failure.
            if (exit_status == 5) {
                printf("[AutoUserShell] Warning: Exit status 5 detected. The %s program call likely failed.\n", args[1]);
            }
        } else {
            printf("[AutoUserShell] Warning: SudoShell> %s terminated abnormally (e.g., killed by a signal).\n", args[1]);
        }

        printf("AutoUserShell> autoexit\n");
    }


    return EXIT_SUCCESS;
}

/*
Usage:
    This program takes commands directly from the terminal execution (e.g., ./program ls -la)
    and safely shifts the argument vector to isolate the target command from the shell executable.

Our Terminal Usage:
    Ubuntu@Ubuntu:~/Clion/Linux-System-Programming/chapter03_exec-c$ gcc -o AutoUserShell Scenario01_build_command.c
    Ubuntu@Ubuntu:~/Clion/Linux-System-Programming/chapter03_exec-c$ ./AutoUserShell ls
    [NOTE] AutoUserShell runs commands automatically with SUDO privileges!!

    SudoShell> ls
    AutoUserShell  Ex00_new_program.c  Ex01_exec_syntax.c  Ex02_execlp_syntax.c  Ex03_execv_syntax.c  Ex04_execvp_syntax.c  Scenario01_build_command.c

    [AutoUserShell] SudoShell terminated and switched back to AutoUserShell with exit status: 0
    AutoUserShell> autoexit

Real-World Application:
    Implemented Application:
        Security & Privilege Wrappers: Tools like sudo or custom security sandboxes use single-shot execution to intercept a command,
        apply strict permission boundaries, and then trigger the target binary on behalf of the user.

    Other Application:
        Container Entrypoints: Docker and Kubernetes use this exact architecture for init scripts.
                               The wrapper script performs initial environment setup,
                               spawns the main container application using an exec function,
                               and waits to catch the application's exit code to determine if the container crashed or shut down normally.

        CI/CD Pipeline Runners: Automation systems (like GitHub Actions or Jenkins) spin up child processes for individual build steps using this pattern.
                                The parent runner waits for the compilation or test command to finish and evaluates the integer exit status to mark a pipeline step as passed or failed.

        Task Scheduling (Cron): Background schedulers wake up at specified times,
                                fork a duplicate process to run the scheduled script, and evaluate the termination status to log whether the job succeeded.


 */
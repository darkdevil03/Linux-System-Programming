#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child process exits immediately
        printf("Child process (PID: %d) is exiting...\n", getpid());
        printf("Check the zombie status now using below of any command in terminal !\n");
        printf("                                  |--------------------------------> ps aux | grep Z \n");
        printf("                                  |--------------------------------> ps -el | grep Z \n");
        exit(0);
    }
    else {
        // Parent process does NOT call wait(), instead it sleeps
        printf("Parent process (PID: %d) is sleeping. Check for zombie now!\n", getpid());

        // Sleep for 40 seconds (giving We time to check the process list)
        sleep(40);


        // Parent eventually cleans up the child using wait()
        wait(NULL);
        printf("Parent cleaned up the child zombie process.\n");
    }

    return 0;
}
/*

    To check the zombie process in linux: sample output of our running time are pasted!

    1) While the program is running and the parent is sleeping, open a second terminal window and run this command:

        Ubuntu@Ubuntu:~/Clion/Linux-System-Programming$ ps aux | grep Z
        USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
        xap        17153  0.0  0.0      0     0 ?        Z    15:52   0:00 [Ex04_zombie_pro] <defunct>

                            (OR)
    2) look specifically for our program's defunct process using:

        Ubuntu@Ubuntu:~/Clion/Linux-System-Programming$ ps -el | grep Z
        F S   UID     PID    PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
        1 Z  1000   17738   17737  0  80   0 -     0 -      ?        00:00:00 Ex04_zombie_pro


    We will see an entry marked with a Z (or labeled as <defunct>),
    meaning the child process has completed its life cycle layout as a zombie waiting for its parent to read its exit status.

*/
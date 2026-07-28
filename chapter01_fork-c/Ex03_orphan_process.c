#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    WHAT IS AN ORPHAN PROCESS ?
        An orphan process is a child process whose parent process has finished or terminated, leaving the child process running.
        When this happens, the init process (process ID 1, or systemd on modern Linux systems) adopts the orphan process.

*/

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // This is the child process
        printf("Child Process: My PID is %d, and my Parent PID is %d\n", getpid(), getppid());

        // Sleep for 2-5 seconds to let the parent process terminate first
         sleep(0); // I have used 0 to print the below instruction to print.

        // After the parent dies, check the Parent PID again
        printf("Orphan Child Process: My PID is %d, and my new Parent PID is %d (adopted by init/systemd)\n", getpid(), getppid());
    }
    else {
        // This is the parent process
        printf("Parent Process: My PID is %d, and my child's PID is %d\n", getpid(), pid);

        // Parent terminates immediately, creating an orphan process
        printf("Parent Process is terminating...\n");
    }

    return 0;
}

/*
 Sample Output at our run-time:

    Parent Process: My PID is 15043, and my child's PID is 15044
    Parent Process is terminating...
    Child Process: My PID is 15044, and my Parent PID is 3216
    Orphan Child Process: My PID is 15044, and my new Parent PID is 3216 (adopted by init/systemd)


    For Above output :
        The Parent PID is 3216 right away: Notice that even in the first child print statement,
        the Parent PID is already 3216 instead of the parent's actual PID (15043).
        This means the parent died so fast (before the child could even execute its first line of code) that the child was already orphaned and adopted by 3216 instantly.

    The reason the parent process ID (ppid) is 3216 (instead of 1) comes down to how modern Linux and systemd handle user sessions, rather than anything specific to VirtualBox.

    What is happening?
        Sub-reapers and User-level systemd:Traditionally, all orphaned processes in Linux were automatically adopted by the root system initializer (PID 1).
        However, modern Linux kernels added a feature allowing user-space service managers (like systemd --user, which manages your desktop session or SSH login session) to act as a "sub-reaper".

    Where did 3216 come from?
        The process with PID 3216 is likely your user-specific instance of systemd (or a shell/session manager like gnome-session or ssh daemon tied to your user session).
        When your parent process (12000) terminates instantly, instead of handing the child (12001) over to root (PID 1), your local session manager (3216) catches and adopts the orphan process.

    To see PID 1 instead:
     -> If you want to force the child to be adopted by system-wide PID 1,
     -> you have to ensure the parent process exits before the child finishes,
     -> but you also need the child to outlive the user session context (or run it as a true background daemon outside of a tracked user scope).
     -> However, seeing 3216 on Ubuntu is completely normal behavior for modern Linux desktop/server environments—it means your user session manager successfully acted as the adoptive parent for the orphan.
*/
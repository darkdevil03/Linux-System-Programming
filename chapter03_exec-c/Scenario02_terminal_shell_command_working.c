#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/wait.h>

int main() {

    printf("[TERMINAL] I am running with a process id: %d\n",getpid());

    while (1) {
        char *command[30];
        pid_t shell = fork();
        printf("shell_%d$",getpid());
        if (shell == 0) {
            scanf("%p",command);
            printf("\n[Shell_%d->%s]\n",getpid(),command[0]);
            execvp(command[0],command);

            exit(5);
        }else {
            int status ;
            pid_t terminated_shell_id = wait(&status);
            int exit_code = WEXITSTATUS(status);
            if (WIFEXITED(status) == 0) {
                printf("[Shell_%d] terminated normally with exit code %d\n",getpid(),exit_code);
                if (exit_code == 5) {
                    printf("[Shell_%d] exec member malfunctioned exit code %d\n",getpid(),exit_code);
                }
            }else {
                printf("[Shell_%d] got signal %d\n",getpid(),exit_code);
            }
        }
    }

    return 0;
}

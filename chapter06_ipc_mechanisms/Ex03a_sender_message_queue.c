
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


int main() {

    int key = ftok("userdev", 54);

    int msg_id = msgget(key, IPC_CREAT | 0666);

    char msg[25] = "Hello, there!";

    msgsnd(msg_id, msg, strlen(msg), 0);

    return EXIT_SUCCESS;
}



#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {

    int key = ftok("userdev",54);

    int msg_id = msgget(key, 0);
    if (msg_id < 0) {
        perror("[ERROR] msgget failed at receiver-end");
        return EXIT_FAILURE;
    }
    printf("[RECEIVED-MSG] Received message from sender process %d\n", msg_id);

    char rcv_msg[25];

    msgrcv(msg_id,rcv_msg,25,0,0);

    printf("Received data : %s\n", rcv_msg );

    msgctl(msg_id, IPC_RMID, nullptr);

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_type{
    long type;
    char msg[256];
};

int main() {

    int key = ftok("kitkat",128);

    int msg_id = msgget(key, 0);

    printf("[CONSUMER] To read the producer message of type-1\n");
    struct msg_type data;

    msgrcv(msg_id, &data, sizeof(data.msg), 1, 0);
    printf("[RECEIVED-MSG] Message : %s", data.msg);

    msgctl(msg_id, IPC_RMID, nullptr);

    return EXIT_SUCCESS;
}

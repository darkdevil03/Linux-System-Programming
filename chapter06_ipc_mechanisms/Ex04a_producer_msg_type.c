

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

struct msg_type {
    long type;
    char msg[256];
};

int main() {

    int key  = ftok("kitkat",128);
    printf("[PRODUCER] Initiating the type-1 messages \n");
    int msg_id = msgget(key, IPC_CREAT | 0666);

    struct msg_type data;
    data.type = 1;
    strcpy(data.msg, "Hello, This is number type messages");

    msgsnd(msg_id, &data, sizeof(data.msg), 0);
    printf("Message sent successfully\n");


    return EXIT_SUCCESS;
}

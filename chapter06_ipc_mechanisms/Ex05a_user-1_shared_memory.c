#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    // 1. Generate a unique key
    key_t key = ftok("jellybean", 128);

    // 2. Create shared memory block (1024 bytes) with read/write permissions
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // 3. Attach the memory to this process's address space
    char *str = shmat(shmid, nullptr, 0);

    // 4. Write data directly to the memory (No system calls needed here!)
    printf("[USER-1] Writing data to shared memory...\n");
    strcpy(str, "Hello from the Writer Process! This is instant.");
    printf("[STATUS] Text data written to shared memory location : %p\n", str);

    // 5. Detach from shared memory
    shmdt(str);

    return EXIT_SUCCESS;
}

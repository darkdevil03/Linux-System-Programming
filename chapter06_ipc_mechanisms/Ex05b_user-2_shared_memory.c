#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // 1. Generate the SAME key
    key_t key = ftok("jellybean", 128);

    // 2. Locate the existing shared memory block
    int shmid = shmget(key, 1024, 0666);

    // 3. Attach to the memory
    char *str =  shmat(shmid, nullptr, 0);
    printf("[USER-2] Access the data of shared memory %p location successfully! \n", str);
    // 4. Read the data directly via pointer
    printf("[READER] Data read from shared memory is : %s\n", str);

    // 5. Detach from shared memory
    shmdt(str);

    // 6. Destroy the shared memory block
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
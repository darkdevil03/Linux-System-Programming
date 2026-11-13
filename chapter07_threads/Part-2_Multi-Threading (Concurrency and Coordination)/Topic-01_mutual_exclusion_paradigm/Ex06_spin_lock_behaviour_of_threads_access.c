/**
Scenario:
        To expose the spinning behavior and print a message every time
        the thread checks the lock, you must build your own spin loop
        using pthread_spin_trylock().

    Details:
        To see a thread constantly asking for the lock,
        you cannot use the standard pthread_spin_lock() function.
        That function handles the "spinning" (polling) internally
        at the hardware/OS level, meaning it blocks your code from
        executing print statements until it successfully gets the lock.

        pthread_spin_trylock() checks if the lock is available.
        If it is, it locks it and returns 0. If it is already locked
        by someone else, it returns immediately with an error code (usually EBUSY),
        allowing you to execute code (like a print statement) before checking again.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_spinlock_t spinlock;
int trade_count = 0;

void* trade_task(void* arg) {
    int id = *(int*)arg;
    printf("[THREAD-%d] Trying to gain lock !!\n", id);

    unsigned int spin_count = 0;

    // CUSTOM SPIN LOCK:
    // pthread_spin_trylock returns 0 if successful, or non-zero (EBUSY) if locked.
    while (pthread_spin_trylock(&spinlock) != 0) {
        spin_count++;
        // Only print every 10 millionth check to prevent crashing the terminal
        // with massive amounts of text, demonstrating how fast it burns CPU.
        if (spin_count % 10000000 == 0) {
            printf("[THREAD-%d] Still spinning... asked %d times so far!\n", id, spin_count);
        }
    }

    printf("[THREAD-%d] Gained access to lock after spinning %d times!\n", id, spin_count);

    trade_count++;
    usleep(700000); // Hold lock for 0.7 seconds

    printf("[THREAD-%d] Releasing trade count after changed to : %d \n", id, trade_count);
    pthread_spin_unlock(&spinlock);

    printf("[THREAD-%d] completed the task!!\n", id);

    return NULL;
}

int main() {

    pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);
    printf("[MAIN] Spinlock initialized!\n");
    printf("Enter how many threads have to create?\n");
    int num_of_threads;
    scanf("%d",&num_of_threads);
    printf("\n");

    int t_id[num_of_threads] = {};
    pthread_t threads[num_of_threads] = {};

    for (int i = 1; i <= num_of_threads; i++) {
        t_id[i] = i;
        pthread_create(&threads[i],NULL,trade_task,&t_id[i]);
        // Slight delay to ensure Thread-1 and following threads grabs the lock first
        usleep(10000);

    }

    for (int i = 1; i <= num_of_threads; i++) {
        pthread_join(threads[i],NULL);
    }

    printf("[MAIN] Spinlock between threads completed!! \n");

    return EXIT_SUCCESS;
}

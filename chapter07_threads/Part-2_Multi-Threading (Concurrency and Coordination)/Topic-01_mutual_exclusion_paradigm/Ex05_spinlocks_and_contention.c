/**
    Scenario: High-frequency trading system. Putting a thread to sleep (context switching) takes 5 microseconds.
              The task takes 1 microsecond. Using a mutex is too slow; a spinlock forces the thread to stay awake
              and loop continuously until the lock is free.

    Details:  pthread_spin_lock (The Pacer): When a thread tries to acquire a locked spinlock, the OS is not involved.
              The thread stays completely awake on the CPU, running a tight infinite loop (spinning) at the hardware
              level, constantly asking, "Is it open yet? How about now? Now?"
                  Pros: Instantaneous access. The absolute microsecond the lock opens,
                        the spinning thread grabs it without waiting for the OS to wake it up.
                  Cons: It burns 100% of its CPU core doing absolutely nothing but checking a lock,
                        starving other threads of processing power.

    Usage: Use a Spinlock only in ultra-low-latency environments (like high-frequency trading or kernel drivers)
            where the lock is only held for a few nanoseconds (e.g., just incrementing a single integer).
            Furthermore, spinlocks should only be used on multi-core systems; on a single-core system,
            a spinlock will instantly deadlock your program because the spinning thread will never yield the CPU to
            let the holding thread finish and unlock it.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_spinlock_t spinlock;
int trade_count = 0;

void* trade_task(void* arg) {
    printf("[THREAD-%d] Trying to gain lock !!\n", *(int*)arg);

    // Burns CPU cycles polling the lock, but acquires it 10x faster than a Mutex
    pthread_spin_lock(&spinlock);
    printf("[THREAD-%d] Gained access to lock \n",*(int*)arg);
    trade_count++;
    printf("[THREAD-%d] Releasing trade count after changed to : %d \n", *(int*)arg, trade_count);
    pthread_spin_unlock(&spinlock);

    printf("[THREAD-%d] completed the task!!\n",*(int*)arg);

    return NULL;
}

int main() {
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

    pthread_t t1, t2;
    int t_id1 = 1, t_id2 = 2;
    pthread_create(&t1, NULL, trade_task, &t_id1);
    pthread_create(&t2, NULL, trade_task, &t_id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_spin_destroy(&spinlock);
    return 0;
}

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
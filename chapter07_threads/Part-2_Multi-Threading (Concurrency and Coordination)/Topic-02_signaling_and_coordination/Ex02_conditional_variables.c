/**
    Scenario: Condition Variables (pthread_cond_t)
        A thread waiting for a specific state change (e.g., waiting for the buffer to hit exactly 5 items).
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t lock; // 1. Added the mandatory mutex
int amount = 0;

void* debit_transaction(void* arg) {
    int thread_id = *(int*)arg;
    printf("[Thread-%d] Trying to access transaction of SHJ-1 account\n", thread_id);

    pthread_mutex_lock(&lock); // 2. Lock before checking the state
    
    // 3. ALWAYS use a while loop for condition variables, never an 'if'
    while (amount < 500) {
        printf("[Thread-%d] Insufficient funds. Waiting...\n", thread_id);
        // 4. Pass the locked mutex into the wait function
        pthread_cond_wait(&cond, &lock); 
    }
    
    amount -= 500;
    printf("[Thread-%d] Debited 500rs successfully!\n", thread_id);
    
    pthread_mutex_unlock(&lock); // 5. Release the lock when done
    return NULL;
}

void* credit_transaction(void* arg) {
    int thread_id = *(int*)arg;
    printf("[Thread-%d] Trying to access transaction of SHJ-1 account\n", thread_id);

    pthread_mutex_lock(&lock); // 6. Lock before modifying shared state
    
    amount += 500;
    printf("[Thread-%d] 500 Credited successfully!\n", thread_id);
    
    pthread_cond_signal(&cond); // 7. Signal the waiting thread
    
    pthread_mutex_unlock(&lock); // 8. Unlock so the waiting thread can grab the lock and wake up

    return NULL;
}

int main() {
    printf("[MAIN] Welcome to Credit and Debit Transaction by two person for same account\n");

    pthread_mutex_init(&lock, NULL); // Initialize mutex
    pthread_cond_init(&cond, NULL);
    printf("[MAIN] Initialized the condition variable synchronization!\n");

    pthread_t debit_t1, credit_t2;
    int t1_id = 1, t2_id = 2;

    // Debit thread starts first, realizes it has 0 funds, and goes to sleep
    pthread_create(&debit_t1, NULL, debit_transaction, &t1_id);
    sleep(1);
    
    // Credit thread comes in, adds funds, and wakes up the debit thread
    pthread_create(&credit_t2, NULL, credit_transaction, &t2_id);

    pthread_join(debit_t1, NULL);
    pthread_join(credit_t2, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock); // Destroy mutex
    printf("[MAIN] Transaction environment completed!!\n");

    return EXIT_SUCCESS;
}
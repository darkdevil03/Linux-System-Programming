
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t cond;
int amount = 0;

void* debit_transaction(void* arg) {
    int thread_id = *(int*)arg;
    printf("[Thread-%d] Trying to access transaction of SHJ-1 account", thread_id);

    pthread_cond_wait(&cond,NULL);
    if (amount < 500) {
        printf("[Thread-%d] Transaction completed successfully!\n", thread_id);
    }else {
        amount -= 500;
        printf("[Thread-%d] Transaction completed successfully!\n", thread_id);
    }



    return NULL;
}

int main() {

    printf("[MAIN] Welcome to Credit and Debit Transaction by two person for same account  \n");

    pthread_cond_init(&cond,NULL);
    printf("[MAIN] Initialized the condition variable synchronization! \n");

    pthread_t debit_t1, credit_t2;
    int t1_id = 1, t2_id = 2;

    pthread_create(&debit_t1, NULL, transaction, &t1_id);
    sleep(1);
    pthread_create(&credit_t2, NULL, transaction, &t2_id);

    pthread_join(debit_t1, NULL);
    pthread_join(credit_t2, NULL);

    pthread_cond_destroy(&cond);
    printf("[MAIN] Transaction environment completed!! \n");

    return EXIT_SUCCESS;
}

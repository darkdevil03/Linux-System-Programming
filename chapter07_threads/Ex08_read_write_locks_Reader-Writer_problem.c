
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/pthreadtypes.h>

pthread_rwlock_t rw_lock;
int shared_var = 0;

void *writer(void *arg) {
    printf("[Writer] I gonna start write\n");
    pthread_rwlock_wrlock(&rw_lock);
    shared_var++;
    printf("[Writer] updated shared var : %d\n", shared_var);
    pthread_rwlock_unlock(&rw_lock);

    return NULL;
}

void *reader(void* arg) {
    int id = (*(int*) arg);
    printf("[Reader-%d] I am reading \n", id);
    pthread_rwlock_rdlock(&rw_lock);
    printf("[Reader-%d] reading shared var : %d\n", id, shared_var);
    pthread_rwlock_unlock(&rw_lock);

    return NULL;
}

int main() {
    pthread_t r1, r2, r3, w1;
    pthread_t ids[3] = {1, 2, 3};

    pthread_rwlock_init(&rw_lock, NULL);

    pthread_create(&r1, NULL, reader, &ids[0]);
    pthread_create(&r2, NULL, reader, &ids[1]);


    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&r3, NULL, reader, &ids[2]);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);

    pthread_rwlock_destroy(&rw_lock);

    return EXIT_SUCCESS;
}

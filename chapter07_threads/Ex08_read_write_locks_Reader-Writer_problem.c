#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int shared_data = 0;

void* reader(void* arg) {
    int id = *((int*)arg);

    pthread_rwlock_rdlock(&rwlock); // Multiple threads can hold this
    printf("[READER-%d] read: %d\n", id, shared_data);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

void* writer(void* arg) {
    pthread_rwlock_wrlock(&rwlock); // Blocks all other readers AND writers
    shared_data++;
    printf("Writer updated shared_data to: %d\n", shared_data);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

int main() {
    pthread_t r1, r2, r3, w1;
    int ids[] = {1, 2, 3};

    pthread_rwlock_init(&rwlock, NULL);
    
    pthread_create(&r1, NULL, reader, &ids[0]);
    pthread_create(&r2, NULL, reader, &ids[1]);
    pthread_create(&w1, NULL, writer, NULL); // Writer interrupts
    pthread_create(&r3, NULL, reader, &ids[2]);
    
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);
    pthread_join(r3, NULL);
    
    pthread_rwlock_destroy(&rwlock);

    return EXIT_SUCCESS;
}
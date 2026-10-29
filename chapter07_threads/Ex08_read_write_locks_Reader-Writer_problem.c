/**
    Scenario: Read-Write Locks (Reader-Writer Problem)
        A caching server has many threads reading data frequently,
        but writes happen rarely. A standard mutex blocks everyone.
        How do you optimize this so multiple readers can read simultaneously,
        but a writer gets exclusive access?

    Details:
        pthread_rwlock_t distinguishes between read and write intents.
        It allows massive parallel read throughput while ensuring total safety during writes.
        Introduces optimized locking mechanisms based on the type of memory operation.
    Usage:
        High-performance caching layers, filesystem implementations, DNS resolvers.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int shared_data = 0;

void* reader(void* arg) {
    int id = *((int*)arg);

    pthread_rwlock_rdlock(&rwlock); // Multiple threads can hold this
    printf("[READER-%d] Read data: %d\n", id, shared_data);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

void* writer(void* arg) {
    pthread_rwlock_wrlock(&rwlock); // Blocks all other readers AND writers
    shared_data++;
    printf("[WRITER] Updated shared_data to: %d\n", shared_data);
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

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t db_connections;

void* handler(void* arg) {
    int id = *(int*)arg;
    sem_wait(&db_connections);

    printf("[THREAD-%d] Got DB connection\n", id);
    sleep(5);
    printf("[THREAD-%d] Completed DB work\n", id);

    sem_post(&db_connections);

    return NULL;
}

int main() {

    sem_init(&db_connections, 0, 3);
    pthread_t t_id[10] = {0};
    pthread_t threads[10] = {0};


    for (int i = 0; i < 10; i++) {
        t_id[i] = i;
        pthread_create(&threads[i],NULL, handler, &t_id[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i],NULL);
    }

    sem_destroy(&db_connections);

    return EXIT_SUCCESS;
}

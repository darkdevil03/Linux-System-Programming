/**
    Question:
        How do you safely manage multiple independent shared resources
        among different groups of worker threads, ensuring that threads
        in one group do not unnecessarily block threads in another group?

    Scenario
        Imagine an online ticket booking system processing orders for
        two different concerts simultaneously. You have four worker threads:
        Threads 1 and 2 are processing tickets for a rock concert (Group 1),
        while Threads 3 and 4 are processing tickets for a jazz concert (Group 2).
        If all four threads shared a single global lock, the rock concert threads
        would force the jazz concert threads to wait, slowing down the entire system.
        Instead, you give each concert its own independent lock and counter,
        allowing both groups to process orders in parallel without interfering with each other.

    Description:
        Multi-Group Synchronization & Local Batching Optimization
         This program demonstrates how to isolate shared state using structs.
         It creates distinct synchronization domains (Group 1 and Group 2),
         allowing independent thread groups to process and update their respective
         counters concurrently without cross-group lock contention. It also utilizes
         local counting to minimize the time spent holding the mutex.

    Usage:
        Multiplayer Game Servers, Database Sharding/Partitioning, and
        Web Server Analytics
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct data {
    int counter;
    pthread_mutex_t lock;
}SharedData;

void* group_func(void* arg) {
    SharedData* data = (SharedData*) arg;

    int local_counter = 0;

    for (int i = 0; i < 1000000000; i++) {
        local_counter++;
    }

    pthread_mutex_lock(&data->lock);
    data->counter += local_counter;
    pthread_mutex_unlock(&data->lock);
}

int main() {
    pthread_t thread1, thread2, thread3, thread4;

    SharedData group1, group2;
    group1.counter = 0;
    group2.counter = 0;

    pthread_mutex_init(&group1.lock, NULL);
    pthread_mutex_init(&group2.lock, NULL);
    pthread_create(&thread1, NULL, group_func, &group1);
    pthread_create(&thread2, NULL, group_func, &group1);


    pthread_create(&thread3, NULL, group_func, &group2);
    pthread_create(&thread4, NULL, group_func, &group2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    printf("[GROUP-1] Global counter = %d\n", group1.counter);
    printf("[GROUP-2] Global counter = %d\n", group2.counter);

    pthread_mutex_destroy(&group1.lock);
    pthread_mutex_destroy(&group2.lock);

    return EXIT_SUCCESS;
}

/**
Real-Time Usage:

   1. Multiplayer Game Servers: Players are divided into different "instances" or "rooms."
        Each room has its own state and its own mutex. Threads processing actions for players
        in Room A lock only Room A's mutex, so they never freeze the game for players in Room B.

   2. Database Sharding/Partitioning: High-performance databases (like Redis or Memcached) split
        memory into separate "shards." When multiple threads attempt to write data,
        they only lock the specific memory shard they are writing to, allowing massive
        parallel throughput across the database.

   3. Web Server Analytics: A multi-tenant SaaS application aggregating live traffic metrics for
        different corporate clients. Threads processing traffic for Client X update Client X's
        metrics structure, entirely independent of the threads processing traffic for Client Y.
 */

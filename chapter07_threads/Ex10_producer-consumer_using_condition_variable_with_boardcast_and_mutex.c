/**
    Scenario:
        where a master thread tells all waiting worker threads to shut down using broadcast.

    Details and Usage:
        When to use broadcast vs signal
            Use pthread_cond_signal when you have only produced one unit of work.
            If there is only one new task in the queue, it makes no sense to wake up 5 consumers,
            because 4 of them will just see an empty queue and go back to sleep.
            Waking them all up just to put them back to sleep is called a
            "Thundering Herd" problem, and it destroys performance.

        Use pthread_cond_broadcast when the state change affects everyone.
           -> System Shutdown: You are shutting down the server and need all 10 sleeping worker
                                threads to wake up, see a keep_running == false flag, and exit cleanly.
            -> State Change: A "Game Start" barrier where 20 players are waiting in a lobby.
                                nce the host clicks "Start", the lobby flag changes, and
                                you broadcast to wake all 20 player threads simultaneously to load into the map.
            -> Bulk Production: You just dumped 50 new items into the buffer at once,
                                so you want all your sleeping consumers to wake up and start chewing
                                through them simultaneously.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int keep_running = 1; // 1 means run, 0 means shut down
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* worker(void* arg) {
    int id = *((int*)arg);

    pthread_mutex_lock(&lock);

    // All workers wait here
    while (keep_running == 1) {
        printf("Worker %d going to sleep...\n", id);
        pthread_cond_wait(&cond, &lock);
    }

    // When they break the loop, it means keep_running is 0
    printf("Worker %d received shutdown signal. Exiting.\n", id);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};

    // Start 3 workers
    for(int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    sleep(2); // Let them all go to sleep

    // Initiate shutdown
    printf("\nMain: Initiating global shutdown...\n");
    pthread_mutex_lock(&lock);
    keep_running = 0; // Change the state
    pthread_mutex_unlock(&lock); // Open the door

    // WAKE EVERYONE UP AT ONCE
    pthread_cond_broadcast(&cond);

    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main: All workers shut down cleanly.\n");
    return 0;
}

/**
Theory:
    At the moment main is about to act, Workers 1, 2, and 3
    are all fast asleep in the Condition Queue.
    Because pthread_cond_wait automatically releases the lock
    when a thread goes to sleep, the Mutex is currently unlocked and free.

    Now, main steps in and executes this highly optimized sequence:
        -> main locks the mutex.
        -> main sets keep_running = 0.
        -> main unlocks the mutex. (The door is now open).

    Let's evaluate exactly what happens next in your two scenarios.

    Case 1: main calls pthread_cond_signal
        If main uses signal after unlocking, it creates a fatal bug and cause deadlock also.
        -> The Wakeup: main fires signal. The OS picks exactly one thread
                        (let's say Worker 1) and pulls it out of the Condition Queue.
        -> The Sprint: Worker 1 moves to the Mutex Queue. Because main already unlocked the door,
                        Worker 1 grabs the lock instantly without hitting a brick wall.
        -> The Exit:   Worker 1 returns from wait, sees keep_running == 0, prints its message,
                        unlocks the mutex, and safely dies.
        -> THE DISASTER: Worker 2 and Worker 3 were never signaled. They are still fast asleep
                         in the Condition Queue. Because main will now call pthread_join on
                         all three threads, the program will deadlock forever.
                         main is waiting for Workers 2 and 3 to finish,
                         but Workers 2 and 3 will sleep until the end of time.

    Verdict for Case 1: Extreme failure. When changing a global state that affects
                        multiple sleeping threads, signal leaves threads behind.

    Case 2: main calls pthread_cond_broadcast
        If main uses broadcast after unlocking, it triggers the most perfectly
        optimized wakeup sequence possible in Linux system programming.

        -> The Wakeup: main fires broadcast. The OS grabs Workers 1, 2, and 3 out of the
                        Condition Queue all at once and drops them into the Mutex Queue.
        -> The Sprint (No Brick Wall!): Because main already unlocked the door before shouting,
                        the door is wide open. The OS immediately hands the lock to Worker 1.
        -> The Bottleneck: Worker 1 enters the room. Workers 2 and 3 are awake,
                        but they must wait in the Mutex Queue for Worker 1 to finish.

        The Domino Effect:
            -> Worker 1 sees keep_running == 0, unlocks the door, and dies.
            -> The OS instantly gives the lock to Worker 2.
                Worker 2 sees keep_running == 0, unlocks the door, and dies.
            -> The OS instantly gives the lock to Worker 3. Worker 3 does the same.

    Verdict for Case 2: Flawless and highly optimized. All threads exit safely.

    Why "Unlock then Broadcast" is the Ultimate Optimization
        Compare Case 2 to the alternative (Broadcast then Unlock).

        If main fired broadcast while still holding the lock:
            -> All 3 workers are moved to the Mutex Queue.
            -> Worker 1 tries to grab the lock, but main is still holding it!
            -> Worker 1 slams into a brick wall, and the CPU is forced to perform
                a heavy context switch to put Worker 1 back to sleep in the Mutex Queue.
            -> A microsecond later, main unlocks the door, forcing the CPU to wake Worker 1 up a second time.
            -> By doing Unlock -> Broadcast, main ensures that when the workers wake up,
                the door is already open for the first one in line. You save the CPU from
                wasting time on unnecessary context switches.
 */
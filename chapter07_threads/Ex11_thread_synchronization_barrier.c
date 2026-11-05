#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_barrier_t barr;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int gate = 0;

void* func(void* arg) {
    // 1. Protect the shared 'gate' state with a Mutex to prevent data races
    pthread_mutex_lock(&lock);
    if (gate == 0) {
        printf("[BARRIER-UP-BY-%d] Threads to join for barrier to down\n", *(int*) arg);
        gate = 1;
    }
    pthread_mutex_unlock(&lock);

    printf("[THREAD-%d] Waiting for thread to join.\n", *(int*) arg);

    // 2. Wait at the barrier
    int leader_thread = pthread_barrier_wait(&barr);

    // Add a print statement right after the barrier for ALL threads
    printf("[THREAD-%d] Crossed the barrier!\n", *(int*) arg);

    sleep(1);

    // 3. PTHREAD_BARRIER_SERIAL_THREAD is a special POSIX feature.
    // The OS guarantees this is returned to exactly ONE thread when the barrier drops.
    // This allows one thread to handle post-barrier cleanup safely.
    if (leader_thread == PTHREAD_BARRIER_SERIAL_THREAD) {
        // you can put the mutex_lock also but who's gain first lock by os will goes to below statement.
        // Whereas using PTHREAD_BARRIER_SERIAL_THREAD explained in-detailed from line 65.
        printf("[BARRIER-DOWN-BY-%d] All threads passed.\n", *(int*) arg);
        gate = 0; // Safe to reset here since all threads are unblocked
    }

    return NULL;
}

int main() {
    pthread_t t_id[] = {1, 2, 3};
    pthread_t t[3];

    // FIX: Here num = 3;
    // Match the barrier count (num) to the total number of threads (num)
    // or multiple of num to avoid deadlock
    pthread_barrier_init(&barr, NULL, 3);

    for (int i = 0; i < 3; i++) {
        pthread_create(&t[i], NULL, func, &t_id[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(t[i], NULL);
    }

    printf("[BARRIER] Closure!!\n");

    pthread_barrier_destroy(&barr);
    return 0;
}

/**
Overall Barrier Flow Explanation:

    When a barrier is initialized with a count of 3 (pthread_barrier_init(&barr, NULL, 3)),
    it acts like a turnstile that only unlocks when exactly three people are standing against it.

    The two lines of code in question work together to hold the threads at a specific checkpoint and
    then elect exactly one "leader" thread to perform a unique action right after the barrier opens,
    preventing all three threads from doing the same post-barrier work simultaneously.

    Here is the exact step-by-step, microsecond trace of what happens when three threads
    (Thread A, Thread B, and Thread C) hit these lines of code.

        Phase 1: The Wait (int res = pthread_barrier_wait(&barr))
            Thread A Arrives: Thread A executes pthread_barrier_wait.
                The OS checks the barrier's internal counter, which is currently 0.
                Thread A increments it to 1. Because 1 < 3, the OS puts Thread A to sleep.
                The res variable is not assigned yet.

            Thread B Arrives: A millisecond later, Thread B executes pthread_barrier_wait.
                It increments the counter to 2. Because 2 < 3, the OS puts Thread B to sleep.
                The res variable is not assigned yet.

            Thread C Arrives: Thread C executes pthread_barrier_wait. It increments the counter to 3.
                The OS sees that the required quota (3 out of 3) has been met.

        The Drop: The barrier drops. The OS instantly resets the barrier's internal counter back to 0
        so it is ready for the next cycle. It then wakes up Thread A and Thread B,
        while Thread C is allowed to continue.

        Phase 2: The Return Value (PTHREAD_BARRIER_SERIAL_THREAD)
            As the three threads wake up and the pthread_barrier_wait function finishes,
            the OS must hand each thread a return value to store in their local int res variable.

            The OS guarantees that exactly one thread in the group will receive
            the special constant PTHREAD_BARRIER_SERIAL_THREAD (which is typically defined as -1).

            The other two threads will receive a return value of 0.

        Note: The OS chooses the serial thread arbitrarily. It is not necessarily the last thread to arrive.

            Let's assume the OS randomly selects Thread B as the serial thread.

            Thread A wakes up. Its pthread_barrier_wait returns 0. Therefore, res = 0.

            Thread B wakes up. Its pthread_barrier_wait returns the special constant. Therefore, res = PTHREAD_BARRIER_SERIAL_THREAD.

            Thread C continues. Its pthread_barrier_wait returns 0. Therefore, res = 0.

        Phase 3: The If-Statement Evaluation
            All three threads now move to the next line of code simultaneously: if (res == PTHREAD_BARRIER_SERIAL_THREAD)

            Thread A: Evaluates if (0 == PTHREAD_BARRIER_SERIAL_THREAD). This is false.
                Thread A skips the if-block entirely and continues executing the rest of the function.

            Thread C: Evaluates if (0 == PTHREAD_BARRIER_SERIAL_THREAD). This is false.
                Thread C skips the if-block entirely.

            Thread B: Evaluates if (PTHREAD_BARRIER_SERIAL_THREAD == PTHREAD_BARRIER_SERIAL_THREAD).
                This is true. Thread B enters the if-block.

        Because Thread B is the only one that enters the block, it can safely reset a shared variable
        (like gate = 0), print a "Closure" message, or clean up memory without needing to use a Mutex.
        It is impossible for a race condition to occur inside this specific if-block because
        the OS strictly ensures no other thread from that barrier cycle possesses the serial ticket.
 */
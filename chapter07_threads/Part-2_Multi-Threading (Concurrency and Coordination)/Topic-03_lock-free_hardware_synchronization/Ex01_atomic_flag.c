#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

// Initialize the atomic flag to false (unlocked state)
atomic_flag hardware_lock = {0};

// The shared resource we need to protect
int shared_counter = 0;

void* critical_task(void* arg) {
    for (int i = 0; i < 100000; i++) {

        // 1. ACQUIRE LOCK (The Spin)
        // atomic_flag_test_and_set() atomically sets the flag to true and
        // returns the PREVIOUS value.
        // If it returns true: Someone else has the lock. Keep looping (spinning).
        // If it returns false: The lock was free, and we just claimed it. Break the loop.
        while (atomic_flag_test_and_set(&hardware_lock)) {
            // Busy-wait (spinning). The CPU actively burns cycles here
            // without ever going to sleep or triggering an OS context switch.
        }

        // 2. CRITICAL SECTION
        shared_counter++;

        // 3. RELEASE LOCK
        // Sets the flag back to false, allowing the next spinning thread to enter.
        atomic_flag_clear(&hardware_lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    printf("Starting threads with custom atomic spinlock...\n");

    // Spawn threads that will fiercely compete for the lock
    pthread_create(&t1, NULL, critical_task, NULL);
    pthread_create(&t2, NULL, critical_task, NULL);

    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // If the lock works, this will be exactly 200,000
    printf("Final Counter Value: %d\n", shared_counter);

    return 0;
}
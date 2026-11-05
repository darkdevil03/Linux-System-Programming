/**
    Scenario:
        A thread computes the square of a number.
        How does it return that computed value back to the main thread?

    Details:
        A thread returns a void*.
        We cannot return a pointer to a local stack variable, so we use malloc.
        pthread_join retrieves this pointer via a double pointer argument.

    Usage:
        Offloading heavy mathematical calculations or database queries
        where the main thread needs the answer later.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* square_func(void* arg) {
    int num = *((int*)arg);

    // Must allocate on heap!
    int* sq = malloc(sizeof(int));
    *sq = num * num;

    // Return the pointer
    return (void*) sq;
}

int main() {
    pthread_t thread;
    int input = 30;
    void* thread_result = nullptr;
    
    pthread_create(&thread, nullptr, square_func, &input);
    // Capture the return value
    pthread_join(thread, &thread_result);
    
    printf("Result from thread: %d\n", *((int*)thread_result));
    // Clean up memory
    free(thread_result);

    return EXIT_SUCCESS;
}
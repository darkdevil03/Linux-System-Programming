
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* square_func(void* arg) {
    int num = *(int*)arg;

    int *sq = malloc(sizeof(int));
    *sq = num * num;

    return (void*)sq;
}

int main() {

    pthread_t thread1;
    int input = 30;
    void* thread_result = nullptr;

    pthread_create(&thread1, nullptr,  square_func, &input);

    pthread_join(thread1, &thread_result);

    printf("Square of %d is %d\n", input, *(int*)thread_result);

    free(thread_result);

    return EXIT_SUCCESS;
}

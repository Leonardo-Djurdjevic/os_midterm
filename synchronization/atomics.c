#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>

#define THREAD_COUNT 5

atomic_int counter = 0;

void* thread_func(void* arg) {
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&counter, 1);
    }
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    printf("Final counter value: %d\n", atomic_load(&counter));

    return 0;
}
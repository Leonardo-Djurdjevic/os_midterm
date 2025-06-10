#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int counter = 0;
pthread_mutex_t lock;

void *thread_function(void *arg) {
    pthread_mutex_lock(&lock);
    counter++;
    printf("Thread %ld finished. Counter: %d\n", (long)arg, counter);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[10];
    pthread_mutex_init(&lock, NULL);

    for (long i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, (void*)i) != 0) {
            fprintf(stderr, "Failed to create thread %ld.\n", i);
            return 1;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread %d.\n", i);
            return 1;
        }
    }

    printf("All threads finished. Final counter: %d\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 5

int rolls[NUM_THREADS];
pthread_barrier_t barrier;

void* thread_func(void* arg) {
    int id = *(int*)arg;
    unsigned int seed = time(NULL) ^ (id * 12345);

    rolls[id] = (rand_r(&seed) % 6) + 1;
    printf("Thread %d rolled: %d\n", id, rolls[id]);

    int status = pthread_barrier_wait(&barrier);

    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        int min_roll = rolls[0];
        int winner = 0;
        for (int i = 1; i < NUM_THREADS; i++) {
            if (rolls[i] < min_roll) {
                min_roll = rolls[i];
                winner = i;
            }
        }
        printf("\nThread %d announces: Winner is thread %d with roll %d\n",
               id, winner, min_roll);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    srand(time(NULL));

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}
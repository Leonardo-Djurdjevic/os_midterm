#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define ITERATIONS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* producer(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        ready = 1;
        printf("Produced %d\n", i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (!ready) {
            pthread_cond_wait(&cond, &mutex);
        }
        ready = 0;
        printf("Consumed %d\n", i);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Task {
    void (*func)(void *);
    void *arg;
    struct Task *next;
} Task;

typedef struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t *threads;
    Task *task_head;
    Task *task_tail;
    int thread_count;
    int stop;
} ThreadPool;

void *worker(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);
        while (!pool->task_head && !pool->stop) {
            pthread_cond_wait(&pool->cond, &pool->lock);
        }
        if (pool->stop && !pool->task_head) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }
        Task *task = pool->task_head;
        if (task) {
            pool->task_head = task->next;
            if (!pool->task_head) {
                pool->task_tail = NULL;
            }
        }
        pthread_mutex_unlock(&pool->lock);
        if (task) {
            task->func(task->arg);
            free(task);
        }
    }
    return NULL;
}

ThreadPool *tp_create(int num_threads) {
    ThreadPool *pool = malloc(sizeof(ThreadPool));
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    pool->task_head = NULL;
    pool->task_tail = NULL;
    pool->thread_count = num_threads;
    pool->stop = 0;
    for (int i = 0; i < num_threads; ++i) {
        pthread_create(&pool->threads[i], NULL, worker, pool);
    }
    return pool;
}

void tp_add_task(ThreadPool *pool, void (*func)(void *), void *arg) {
    Task *task = malloc(sizeof(Task));
    task->func = func;
    task->arg = arg;
    task->next = NULL;
    pthread_mutex_lock(&pool->lock);
    if (pool->task_tail) {
        pool->task_tail->next = task;
    } else {
        pool->task_head = task;
    }
    pool->task_tail = task;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
}

void tp_destroy(ThreadPool *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->stop = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
    for (int i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    Task *task = pool->task_head;
    while (task) {
        Task *next = task->next;
        free(task);
        task = next;
    }
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}

void example_task(void *arg) {
    int *num = (int *)arg;
    printf("Task %d processed by thread %lu\n", *num, (unsigned long)pthread_self());
    usleep(100000);
}

int main() {
    ThreadPool *pool = tp_create(4);
    int tasks[20];

    for (int i = 0; i < 20; i++) {
        tasks[i] = i;
        tp_add_task(pool, example_task, &tasks[i]);
    }

    sleep(2);
    tp_destroy(pool);
    return 0;
}
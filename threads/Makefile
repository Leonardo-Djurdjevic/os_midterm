CFLAGS = -Wall -Werror -pthread -DEFAULT_SOURCE

.PHONY: all
all: pthread semaphore signal thread_pool

.PHONY: pthread
pthread: pthread.c
	$(CC) $(CFLAGS) pthread.c -o pthread

.PHONY: semaphore
semaphore: semaphore.c
	$(CC) $(CFLAGS) semaphore.c -o semaphore

.PHONY: signal
signal: signal.c
	$(CC) $(CFLAGS) signal.c -o signal

.PHONY: thread_pool
thread_pool: thread_pool.c
	$(CC) $(CFLAGS) thread_pool.c -o thread_pool

.PHONY: clean
clean:
	$(RM) pthread semaphore signal thread_pool

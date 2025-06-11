CFLAGS = -Wall -Werror -pthread

.PHONY: all
all: atomics barrier mutex_condition_variable

.PHONY: atomics
atomics: atomics.c
	$(CC) $(CFLAGS) atomics.c -o atomics

.PHONY: barrier
barrier: barrier.c
	$(CC) $(CFLAGS) barrier.c -o barrier

.PHONY: mutex_condition_variable
mutex_condition_variable: mutex_condition_variable.c
	$(CC) $(CFLAGS) mutex_condition_variable.c -o mutex_condition_variable

.PHONY: clean
clean:
	$(RM) atomics barrier mutex_condition_variable

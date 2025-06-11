CFLAGS = -Wall -Werror -DEFAULT_SOURCE

.PHONY: all
all: fifo message_queues pipes shared_memory tcp_server tcp_client

.PHONY: fifo
fifo: fifo.c
	$(CC) $(CFLAGS) fifo.c -o fifo

.PHONY: message_queues
message_queues: message_queues.c
	$(CC) $(CFLAGS) message_queues.c -o message_queues

.PHONY: pipes
pipes: pipes.c
	$(CC) $(CFLAGS) pipes.c -o pipes

.PHONY: shared_memory
shared_memory: shared_memory.c
	$(CC) $(CFLAGS) shared_memory.c -o shared_memory

.PHONY: tcp_server
tcp_server: tcp_server.c
	$(CC) $(CFLAGS) tcp_server.c -o tcp_server

.PHONY: tcp_client
tcp_client: tcp_client.c
	$(CC) $(CFLAGS) tcp_client.c -o tcp_client


.PHONY: clean
clean:
	$(RM) fifo message_queues pipes shared_memory tcp_server tcp_client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MSG_SIZE 128

typedef struct {
    int socket;
    char username[MSG_SIZE];
    pthread_t thread;
} client_t;

int main() {
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(8080),
    };
    bind(server_socket, (struct sockaddr *) &addr, sizeof(addr));
    listen(server_socket, 10);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);

        client_t *client = malloc(sizeof(client_t));
        client->socket = client_sock;
        char buffer[MSG_SIZE];
        recv(client_sock, buffer, MSG_SIZE - 1, 0);
        strncpy(client->username, buffer, MSG_SIZE);
        printf("%s connected\n", client->username);

        while (recv(client_sock, buffer, MSG_SIZE - 1, 0) > 0) {
            printf("%s: %s\n", client->username, buffer);
            if (strcmp(buffer, "/quit") == 0) break;
        }
        close(client_sock);
        free(client);
    }
    close(server_socket);
    return 0;
}
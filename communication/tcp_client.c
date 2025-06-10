#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MSG_SIZE 128

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
    };
    connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    char *username = "user";
    send(sockfd, username, strlen(username), 0);

    char buffer[MSG_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        send(sockfd, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "/quit\n") == 0) break;
    }
    close(sockfd);
    return 0;
}
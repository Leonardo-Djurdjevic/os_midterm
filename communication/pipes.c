#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    int pipe_fd[2];
    pid_t pid;
    char buffer[128];

    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipe_fd[0]);
        const char *message = "Hello from the child via pipe!";
        write(pipe_fd[1], message, strlen(message));
        close(pipe_fd[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe_fd[1]);
        ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Parent received: \"%s\"\n", buffer);
        }

        close(pipe_fd[0]);
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(void) {
    const char *fifo_path = "/tmp/my_simple_fifo";

    if (mkfifo(fifo_path, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        int fd = open(fifo_path, O_WRONLY);
        if (fd == -1) {
            perror("child open failed");
            exit(EXIT_FAILURE);
        }

        const char *message = "Hello from the FIFO writer!";
        write(fd, message, strlen(message));
        close(fd);
        exit(EXIT_SUCCESS);
    } else {
        int fd = open(fifo_path, O_RDONLY);
        if (fd == -1) {
            perror("parent open failed");
            exit(EXIT_FAILURE);
        }

        char buffer[128];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Reader received: \"%s\"\n", buffer);
        }

        close(fd);
        wait(NULL);
        unlink(fifo_path);
    }

    return EXIT_SUCCESS;
}
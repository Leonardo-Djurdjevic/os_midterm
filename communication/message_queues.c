#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <sys/stat.h>

int main(void) {
    const char *mq_name = "/my_simple_mq";
    mqd_t mq;
    struct mq_attr attr;
    pid_t pid;
    char buffer[128];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(buffer);
    attr.mq_curmsgs = 0;

    mq = mq_open(mq_name, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == -1) {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        const char *message = "Hello from the child via MQ!";
        if (mq_send(mq, message, strlen(message), 0) == -1) {
            perror("child mq_send failed");
            exit(EXIT_FAILURE);
        }
        mq_close(mq);
        exit(EXIT_SUCCESS);
    } else {
        ssize_t bytes_read = mq_receive(mq, buffer, sizeof(buffer), NULL);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            printf("Parent received: \"%s\"\n", buffer);
        } else {
            perror("parent mq_receive failed");
        }

        wait(NULL);
        mq_close(mq);
        mq_unlink(mq_name);
    }

    return EXIT_SUCCESS;
}
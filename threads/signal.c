#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = true;

void handle_signal(int signum) {
    printf("\nReceived signal: %s (%d). Shutting down...\n", strsignal(signum), signum);
    keep_running = false;
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("Process ID: %d. Press Ctrl+C to send SIGINT.\n", getpid());

    const time_t work_seconds = 1;
    struct timespec work_duration = {.tv_sec = work_seconds};
    struct timespec remaining = {0};

    while (keep_running) {
        printf("Doing work...\n");

        if (nanosleep(&work_duration, &remaining) == -1 && errno == EINTR) {
            continue;
        }
        work_duration.tv_sec = work_seconds;
    }

    printf("Cleanup complete. Exiting.\n");

    return EXIT_SUCCESS;
}
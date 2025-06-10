#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SHM_NAME "/shm_example"
#define SEM_NAME "/sem_example"

struct data {
    int counter;
};
typedef struct data shared_data;

int main() {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);

    int fd = shm_open(SHM_NAME, O_CREAT|O_RDWR, 0666);
    ftruncate(fd, sizeof(shared_data));
    shared_data *ptr = mmap(NULL, sizeof(shared_data), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    ptr->counter = 0;

    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 5; i++) {
            sem_wait(sem);
            printf("Child inc: %d -> %d\n", ptr->counter, ++ptr->counter);
            sem_post(sem);
            usleep(1000);
        }
        exit(0);
    }
    if (pid > 0) {
        for (int i = 0; i < 5; i++) {
            sem_wait(sem);
            printf("Parent inc: %d -> %d\n", ptr->counter, ++ptr->counter);
            sem_post(sem);
            usleep(1000);
        }
        waitpid(pid, NULL, 0);
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);
    munmap(ptr, sizeof(shared_data));
    close(fd);
    shm_unlink(SHM_NAME);

    return 0;
}
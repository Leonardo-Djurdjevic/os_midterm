#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shm"
#define BUFFER_SIZE 10

struct shared_data {
    unsigned long long result;
    unsigned long long buffer[];
};
typedef struct shared_data shared_data;

int main() {
    size_t shm_size = sizeof(shared_data) + BUFFER_SIZE * sizeof(unsigned long long);
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, shm_size);
    shared_data *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    shm_ptr->result = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        shm_ptr->buffer[i] = i + 1;
        shm_ptr->result += shm_ptr->buffer[i];
    }

    if (fork() == 0) {
        printf("Child reads: result=%llu\n", shm_ptr->result);
        exit(0);
    } else {
        wait(NULL);
        printf("Parent reads: result=%llu\n", shm_ptr->result);
        munmap(shm_ptr, shm_size);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
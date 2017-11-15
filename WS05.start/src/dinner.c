#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define ARRAY "/semaphore"

sem_t **sem_chop, **sem_phil;
char *chop_name, *phil_name;
int *identifier;
int N;

void visualize(int *array, int length);
void clean(int signal);
int main(int argc, char *argv[]) {
    int i, j;
    int round;

    N = (int)strtol(argv[1], NULL, 10);
    round = (int)strtol(argv[2], NULL, 10);

    int fd;
    if ((fd = shm_open(ARRAY, O_CREAT | O_RDWR, 0600)) == -1) {
        perror("shm_open");
        exit(-1);
    }
    if (ftruncate(fd, N * sizeof(int)) == -1) {
        perror("ftruncate");
        exit(-1);
    }
    identifier = (int *)mmap(NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_chop = (sem_t **)malloc(N * sizeof(sem_t *));
    sem_phil = (sem_t **)malloc(((int)ceil(N / 2)) * sizeof(sem_t *));

    for (i = 0; i < N; i++) {
        identifier[i] = 0;
        asprintf(&chop_name, "/chop_%d_sem:0", i);
        sem_chop[i] = sem_open(chop_name, O_CREAT | O_RDWR, 0666, 1);
        if (i % 2 == 0) {
            asprintf(&phil_name, "/phil_%d_sem:0", i / 2);
            sem_phil[i / 2] = sem_open(phil_name, O_CREAT | O_RDWR, 0666, 1);
        }
    }

    struct sigaction action;
    action.sa_handler = &clean;
    sigaction(SIGINT, &action, NULL);

    for (i = 0; i < N && fork() > 0; i++);

    if (i == N) {
        for (i = 0; i < N; i++) {
            wait(NULL);
        }
        clean(SIGINT);
    } else {
        for (j = 0; j < round; j++) {
            sem_wait(sem_phil[(int)ceil(i / 2)]);
            sem_wait(sem_chop[i]);
            sem_wait(sem_chop[(i + 1) % N]);
            sem_post(sem_phil[(int)ceil(i / 2)]);

            identifier[i] = 1;
            // Eating
            usleep(rand() % 1000);

            int k;
            for (k = 0; k < N; k++) {
                printf("%d ", identifier[k]);
            }
            printf("\n");

            identifier[i] = 0;

            sem_post(sem_chop[i]);
            sem_post(sem_chop[(i + 1) % N]);
        }
    }
    printf("Finished\n");
    return 0;
}

void visualize(int *array, int length) {
    char *buffer;
    buffer = (char*)malloc(length * sizeof(char));
    int i;
    for (i = 0; i != length - 1; i++) {
        buffer[i * 2] = '0' + array[i];
        buffer[i * 2 + 1] = ' ';
    }
    buffer[i * 2] = '0' + array[i];
    printf("%s\n", buffer);
    free(buffer);
}

void clean(int signal) {
    int k;
    for (k = 0; k < N; k++) {
        sem_close(sem_chop[k]);
        asprintf(&chop_name, "/chop_%d_sem:0", k);
        sem_unlink(chop_name);
        if (k % 2 == 0) {
            sem_close(sem_phil[k / 2]);
            asprintf(&phil_name, "/phil_%d_sem:0", k / 2);
            sem_unlink(phil_name);
        }
    }
    free(sem_chop);
    free(sem_phil);
    munmap(identifier, N * sizeof(int));
    shm_unlink(ARRAY);
    printf("Done Clean\n");
}
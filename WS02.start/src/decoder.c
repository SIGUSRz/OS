/**** decoder.c ****/
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include <decoder.h>


#define _XOPEN_SOURCE 700
#define N 3


int magicsq[N][N] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};
int size = N * N;


int check(int *temp) {
    int num = 2 * N + 2;
    int i, j, sums[num];
    for (i = 0; i < num; i++)
        sums[i] = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sums[i] += temp[i * N + j];
            sums[i + N] += temp[j * N + i];
            if (i == j) sums[2 * N] += temp[i * N + j];
            if ((i+j) == N - 1) sums[2 * N + 1] += temp[i * N + j];
        }
    }
    int result = 1;
    i = 1;
    while ((i < num) && (result == 1)) {
        if (sums[0] != sums[i])
            result = 0;
        i++;
    }
    return result;
}


void display(int *temp) {
    int i;
    for (i = 0; i < size; i++) {
        printf("  %d", temp[i]);
    }
    printf("\n");
}

int check_empty(int *temp) {
    int i, flag = 0;
    for (i = 0; i < size; i++) {
        if (temp[i] == -1) {
            flag = 1;
            break;
        }
    }
    if (flag) {
        return i;
    }
    return -1;
}

int solve(int x, int *src) {
    int i, flag = 0;
    int *temp = malloc(size * sizeof(int));
    memcpy(temp, src, size * sizeof(int));
    int p = check_empty(&temp[0]);
    temp[p] = x;
    p = check_empty(&temp[0]);
    if (p == -1) {
        if (check(&temp[0])) {
            display(&temp[0]);
            flag = 1;
        }
    } else {
        for (i = 0; i < 10; i++) {
            flag += solve(i, &temp[0]);
        }
    }
    return flag;
}

int main(int argc, char **argv)
{
    int i, pid;
    int counter = 0;
    for (i = 0; i < 10; i++) {
        pid = fork();
        if (pid == 0) {
            printf("Process %d for a = %d created\n", getpid(), i);
            counter = solve(i, &magicsq[0][0]);
            printf("Process %d for a = %d terminates with %d answer\n", getpid(), i, counter);
            exit(0);
        }
    }
    for (i = 0; i < 10; i++) {
        wait(0);
    }
    printf("Parent Dies\n");
    return 0;
}

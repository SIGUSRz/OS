#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define N_CHILDREN 3
int main(int argc, char *argv[]) {
    int pid, end;
    int i;
    if (argv[1] == NULL) {
        for (i = 0;i < N_CHILDREN && (pid = fork()) > 0;i++);
        if (pid == 0) {
            printf("Child process %d created by %d\n", getpid(), getppid());
        }
        exit(0);
    }
    char *flag = argv[1];
    switch (flag[0]) {
        case 'f':
            for (i = 0; i < N_CHILDREN; i++) {
                pid = fork();
                if (pid == 0) {
                    printf("Child process %d created by %d\n", getpid(), getppid());
                    exit(0);
                }
            }
            if (pid > 0) {
                wait(&end);
                printf("Parent process %d received exit code %d from %d\n", \
                      getpid(), WEXITSTATUS(end), pid);
            }
            break;
        case 'l':
            for (i = 0; i < N_CHILDREN; i++) {
                pid = fork();
                if (pid == 0) {
                    printf("Child process %d created by %d\n", getpid(), getppid());
                    exit(0);
                } else if (pid > 0 && i == N_CHILDREN - 1) {
                    waitpid(pid, &end, 0);
                    printf("Parent process %d received exit code %d from %d\n", \
                          getpid(), WEXITSTATUS(end), pid);
                }
            }
            break;
        case 'a':
            for (i = 0;i < N_CHILDREN; i++) {
                pid = fork();
                if (pid == 0) {
                    printf("Child process %d created by %d\n", getpid(), getppid());
                    exit(0);
                }
            }
            if (pid > 0) {
                for (i = 0;i < N_CHILDREN; i++) {
                    wait(&end);
                    printf("Parent process %d received exit code %d\n", \
                          getpid(), WEXITSTATUS(end));
                }
            }
            break;
    }
    return EXIT_SUCCESS;
}

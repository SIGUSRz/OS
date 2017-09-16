#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define N_CHILDREN 3

int main(int argc, char *argv[0]) {
    if (argv[1] == NULL) {
        printf("No Parameter\n");
        return 0;
    }
    char *param = argv[1];
    void create(int counter, char flag);
    int count = 0;
    create(count, param[0]);
    return 0;
}

void create(int counter, char flag) {
    int pid, status;
    if (counter == N_CHILDREN) {
        exit(0);
    }
    pid = fork();
    if (pid == 0) {
        printf("Child process %d created by %d\n", getpid(), getppid());
        exit(0);
    } else {
        switch (flag) {
            case 'f':
                if (counter == 0) {
                    waitpid(pid, &status, 0);
                    printf("Parent process %d received exit code %d from %d\n", \
                          getpid(), WEXITSTATUS(status), pid);
                }
                break;
            case 'l':
                if (counter == N_CHILDREN - 1) {
                    waitpid(pid, &status, 0);
                    printf("Parent process %d received exit code %d from %d\n", \
                          getpid(), WEXITSTATUS(status), pid);
                }
                break;
            case 'a':
                wait(&status);
                printf("Parent process %d received exit code %d\n", \
                      getpid(), WEXITSTATUS(status));
                break;
        }
        counter += 1;
        create(counter, flag);
    }
}

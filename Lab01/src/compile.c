#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i, status;
    pid_t pid;
    char logger[100];
    char arg1[100], arg2[100];
    if (argv[1] == NULL) {
        printf("No Parameter\n");
        return 0;
    }
    for (i = 1; i < argc; i++) {
        switch (pid = fork()) {
            case -1:
                sprintf(logger, "Fork %d Error\n", i);
                perror(logger);
                exit(-1);
            case 0:
                sprintf(arg1, "obj/%s.o", argv[i]);
                sprintf(arg2, "src/%s.c", argv[i]);
                if (execl("/usr/bin/gcc", "gcc", "-c", "-o", arg1, arg2, 0) == -1) {
                    sprintf(logger, "Object Compilation Fork %d Error\n", i);
                    perror(logger);
                    exit(-1);
                }
                exit(0);
                break;
            default:
                break;
        }
    }
    for(i = 1; i < argc; i++) {
        wait(&status);
    }
    for (i = 1; i < argc; i++) {
        switch (pid = fork()) {
            case -1:
                sprintf(logger, "Fork %d Error\n", i);
                perror(logger);
                exit(-1);
            case 0:
                sprintf(arg1, "bin/%s", argv[i]);
                sprintf(arg2, "obj/%s.o", argv[i]);
                if (execlp("/usr/bin/gcc", "gcc", "-o", arg1, arg2, 0) == -1) {
                    sprintf(logger, "Binary Compilation Fork %d Error\n", i);
                    perror(logger);
                    exit(-1);
                }
                exit(0);
                break;
            default:
                break;
        }
    }
    for(i = 1; i < argc; i++) {
        wait(&status);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i, status;
    pid_t pid;
    char logger[100];
    if (argv[1] == NULL) {
        printf("No Parameter\n");
        return 0;
    }
    for (i = 0; i < argc; i++) {
        switch (pid = fork()) {
            case -1:
                sprintf(logger, "Fork %d Error\n", i);
                perror(logger);
                exit(-1);
            case 0:
                if (execl("/usr/bin/gcc", "-c", "-o", "obj/n_children_loop.o", "src/n_children_loop.c", 0) == -1) {
                    sprintf(logger, "Object Compilation Fork %d Error\n", i);
                    perror(logger);
                    exit(-1);
                }
                break;
            default:
                break;
        }
    }
    for(i = 0; i < argc; i++) {
        wait(&status);
        if (execl("/usr/bin/gcc", "obj/n_children_loop.o", "-o", "bin/n_children_loop", 0) == -1) {
            sprintf(logger, "Binary Compilation Fork %d Error\n", i);
            perror(logger);
            exit(-1);
        }
    }
    for(i = 0; i < argc; i++) {
        wait(&status);
        if (execl("./bin/n_children_loop", NULL) == -1) {
            sprintf(logger, "Binary Execution Fork %d Error\n", i);
            perror(logger);
            exit(-1);
        }
    }
    return 0;
}

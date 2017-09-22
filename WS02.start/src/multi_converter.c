#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <converters.h>
#define N 5

int main(int argc, char **argv)
{
    int i;
    pid_t pid;
    if (argv[1] == NULL || argv[2] == NULL) {
        printf("No Parameter\n");
        return 0;
    }
    printf("Conversion for: %s %s\n", argv[1], argv[2]);
    double result;
    for (i = 0; i < N; i++) {
        pid = fork();
        if (pid == 0) {
            result = convert(argv[1], determine_currency(i), atoi(argv[2]));
            display_result(i, result);
            exit(0);
        }
    }
    for (i = 0; i < N; i++) {
        wait(0);
    }
    printf("End of Conversion\n");
    return 0;
}

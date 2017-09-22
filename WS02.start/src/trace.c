#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main (int arg, char * argv [])
{
    int i, j, p;
    for (i = 0; i < 3; i++) {
        if ((p = fork ()) == 0) {
            printf("i = %d\n", i);
            j = 0;
            while ((j < i) && ((p = fork ()) == 0))
                j++;
            if (p == 0) {
                printf ( "j = %d\n", j);
            } else {
                while (j >= 0) {
                    wait(0);
                    j--;
                }
            }
            exit(j);
        }
    }
    for (i = 0; i < 3; i++) {
        wait(0);
    }
    printf("Parent dies.\n");
    return 0;
}

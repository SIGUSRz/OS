#include <head.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
    time_t start, end;
    start = time(NULL);
    mysleep(5);
    end = time(NULL);
    printf("sleeping time: %ds\n", (int)(end - start));
    return 0;
}

#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int val;

void* thread_control(void* arg) {
    int local_val = 0;

    local_val = rand()%11;
    pthread_mutex_lock(&mutex);
    val += local_val;
    pthread_mutex_unlock(&mutex);
    printf("tid = %ld -- val %d\n", (long)pthread_self(), local_val);
    pthread_exit((void*)0);
}


int main(int argc, char** argv) {

    int i;
    int *pt_i;

    for (i = 0; i < N; i++){
        pt_i = (int*)malloc(sizeof(i));
        *pt_i = i;
        pthread_create((pthread_t*)&tid[i], NULL, thread_control, (void*)pt_i);
    }

    for (i = 0; i < N; i++) {
        printf("MT> tid[%d] = %ld\n", i, (long)tid[i]);
        if (pthread_join(tid[i], NULL)!= 0) {
            perror("join");
            exit(1);
        }
    }

    printf("MT> val -- %d\n", return_value);

    return 0;
}


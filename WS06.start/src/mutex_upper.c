#include <header.h>

#define NB_THREAD 5

int file_idx;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char **args;
int N;

void clean(int signal);
void *convert_file(void *file_name);

int main (int argc, char ** argv) {
    struct sigaction action;
    action.sa_handler = &clean;
    sigaction(SIGINT, &action, NULL);
    N = argc - 1;
    args = argv + 1;

    pthread_t *tid;
    tid = (pthread_t *)malloc(sizeof(pthread_t) * N);

    int i;
    file_idx = 0;
    for (i = 0; i < NB_THREAD; i++) {
        if (pthread_create(&(tid[i]), NULL, convert_file, (void *) &i) != 0) {
            perror("pthread create");
            kill(getpid(), SIGINT);
        }
    }

    for (i = 0; i < NB_THREAD; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            perror("pthread join");
            kill(getpid(), SIGINT);
        }
    }

    printf("Finished\n");

    clean(SIGINT);

    return 0;
}

void *convert_file(void *idx) {
    if (*((int *)idx) >= N) {
        printf("Waste at %d\n", *((int *)idx));
        return NULL;
    }
    while(1) {
        pthread_mutex_lock(&mutex);
        if (file_idx >= N) {
            pthread_mutex_unlock(&mutex);
            pthread_exit((void *) 0);
        }
        char *file = args[file_idx];
        FILE *fp1, *fp2;
        char *src_fname;
        char *dest_fname;
        int c = 1;

        asprintf(&src_fname, "input/%s", file);
        printf("Open %s\n", src_fname);
        fp1 = fopen (src_fname, "r");
        asprintf(&dest_fname, "out/%s.UPPER.txt", file);
        printf("Write to %s\n", dest_fname);
        fp2 = fopen (dest_fname, "w+");

        if (fp1 == NULL) {
            perror ( "fopen 1");
            exit (1);
        }

        if (fp2 == NULL) {
            perror("fopen 2");
        }

        file_idx += 1;
        pthread_mutex_unlock(&mutex);

        while (c != EOF) {
            c = fgetc(fp1);
            if (c != EOF)
                fputc(toupper(c), fp2);
        }

        fclose (fp1);
        fclose (fp2);

        free(dest_fname);
        free(src_fname);
    }
}

void clean(int signal) {

}
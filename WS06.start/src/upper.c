#include <header.h>

pthread_t *tid;
int N;

void clean(int signal);
void *convert_file(void *file_name);

int main (int argc, char ** argv) {
    struct sigaction action;
    action.sa_handler = &clean;
    sigaction(SIGINT, &action, NULL);
    N = argc - 1;

    tid = (pthread_t *)malloc(sizeof(pthread_t) * N);

    int i;
    for (i = 0; i < N; i++) {
        if (pthread_create(&(tid[i]), NULL, convert_file, (void *) argv[i + 1]) != 0) {
            perror("pthread create");
            kill(getpid(), SIGINT);
        }
    }

    for (i = 0; i < N; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            perror("pthread join");
            kill(getpid(), SIGINT);
        }
    }

    printf("Finished\n");

    clean(SIGINT);

    return 0;
}

void *convert_file(void *file_name) {
    char *file = (char *)file_name;
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

    while (c != EOF) {
        c = fgetc(fp1);
        if (c != EOF)
            fputc(toupper(c), fp2);
    }

    fclose (fp1);
    fclose (fp2);

    free(dest_fname);
    free(src_fname);
    pthread_exit((void *) 0);
}

void clean(int signal) {

}

#include <header.h>

int counter = 0;
int N;

pthread_mutex_t m_counter = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_signal = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_create = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_signal = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_terminate = PTHREAD_COND_INITIALIZER;

void handler(int signal);
void *thread(void *pointer);

int main(int argc, char *argv[]) {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    struct sigaction action;
    action.sa_handler = &handler;
    sigaction(SIGINT, &action, NULL);

    N = atoi(argv[1]);

    pthread_t tid;
    counter++;
    if (pthread_create(&tid, NULL, thread, NULL) != 0) {
        perror("pthread create");
        exit(-1);
    }

    pthread_mutex_lock(&m_counter);
    if (counter != N) {
        pthread_cond_wait(&cond_create, &m_counter);
    }
    pthread_mutex_unlock(&m_counter);

    printf("All my descendants are created\n");

    sigemptyset(&mask);
    sigsuspend(&mask);

    pthread_mutex_lock(&m_signal);
    pthread_cond_broadcast(&cond_signal);
    pthread_mutex_unlock(&m_signal);

    pthread_mutex_lock(&m_counter);
    if (counter != 0) {
        pthread_cond_wait(&cond_terminate, &m_counter);
    }
    pthread_mutex_unlock(&m_counter);

    printf("All my descendants have ended\n");
    return 0;
}

void handler(int signal) {
    return;
}

void *thread(void *pointer) {
    printf("Create Thread\n");

    pthread_mutex_lock(&m_counter);
    if (counter != N) {
        pthread_t tid;
        if(pthread_create(&tid, NULL, thread, NULL) != 0) {
            perror("child pthread create");
            exit(-1);
        }
        counter++;
    } else {
        pthread_cond_signal(&cond_create);
    }
    pthread_mutex_unlock(&m_counter);

    pthread_mutex_lock(&m_signal);
    pthread_cond_wait(&cond_signal, &m_signal);
    pthread_mutex_unlock(&m_signal);

    pthread_mutex_lock(&m_counter);
    counter--;
    printf("Terminate Thread\n");
    if (counter == 0) {
        pthread_cond_signal(&cond_terminate);
    }
    pthread_mutex_unlock(&m_counter);
    pthread_exit((void *) 0);
}
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void calc1 () {
    int i;
    for (i = 0; i <1E8; i ++);
    printf("Done 1\n");
}

void calc2 () {
    int i;
    for (i = 0; i <1E8; i ++);
    printf("Done 2\n");
}

void handler(int sig) {}

int main(int argc, char *argv[]) {
    int N = 2;
    pid_t pid_child[2];
    int i;

    struct sigaction action;
    action.sa_handler = &handler;
    sigaction(SIGUSR1, &action, NULL);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);


    for (i = 0; i < N && (pid_child[i] = fork()) != 0; i++);

    calc1();

    sigemptyset(&mask);
    if (i == N) { // In parent
        sigsuspend(&mask); // Wait for first wave, first wave end
        kill(pid_child[N - 1], SIGUSR1); // Start second wave
    } else {
        if (i == N - 1) { // In LD
            kill(pid_child[N - 2], SIGUSR1); // Start first wave
        } else if (i == 0) { // In FD
            sigsuspend(&mask); // Wait for first wave
            kill(getppid(), SIGUSR1); // Pass on first wave
        } else { // In Middle
            sigsuspend(&mask); // Wait for first wave
            kill(pid_child[i - 1], SIGUSR1); // Pass on first wave
        }
        sigsuspend(&mask); // Wait for second wave
        if (i != 0) {
            kill(pid_child[i - 1], SIGUSR1); // Pass on second wave
        }
    }

    calc2();

    return 0;
}
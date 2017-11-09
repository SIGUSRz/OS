#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pid_t child_pid, root_pid;

void handler(int sig) {
    pid_t parent_pid = getppid();
    switch (sig) {
    case SIGUSR1:
        if (parent_pid != root_pid) {
            kill(parent_pid, SIGUSR1);
        } else {
            printf("Parent starts 2nd wave of SIGUSR2\n");
            kill(child_pid, SIGUSR2);
        }
        break;
    case SIGUSR2:
        if (child_pid) {
            kill(child_pid, SIGUSR2);
        } else {
            printf("LD starts 3rd wave of SIGINT\n");
            kill(parent_pid, SIGINT);
        }
        break;
    case SIGINT:
        if (parent_pid != root_pid) {
            kill(parent_pid, SIGINT);
        } else {
            kill(parent_pid, SIGINT);
            printf("End of Program");
        }
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int N = 10;
    if (argc > 1) {
        N = atoi(argv[1]);
    }

    root_pid = getppid();
    struct sigaction action;
    action.sa_handler = &handler;

    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
    sigaction(SIGINT, &action, NULL);

    sigset_t full_mask;
    sigfillset(&full_mask);
    sigset_t empty_mask;
    sigemptyset(&empty_mask);
    sigprocmask(SIG_SETMASK, &full_mask, NULL);

    int i;
    for (i = 0; i != N && (child_pid = fork()) == 0; i++) ;

    if (i == N) {
        printf("LD starts 1st wave of SIGUSR1\n");
        kill(getppid(), SIGUSR1);
    } else {
        sigsuspend(&empty_mask);
    }

    if (getppid() != 0) {
        sigsuspend(&empty_mask);
    }

    if (i != N) {
        sigsuspend(&empty_mask);
    }

    return 0;
}

#define _XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <head.h>
#include <sys/param.h>
#define NSIG 31

struct sigaction prev_action[NSIG];
unsigned int timer = 0;
sigset_t mask, prev_mask;
struct sigaction action;

void sig_handler(int sig) {
    if (sig == SIGALRM) {
        timer = 0;
    } else {
        int i;
        for (i = 0; i < NSIG; i++) {
            sigaction(i, &prev_action[i], NULL);
        }
        sigprocmask(SIG_SETMASK, &prev_mask, NULL);
        kill(getpid(), sig);
        timer = alarm(0);
    }
}

unsigned int mysleep(int sec) {

    sigfillset(&mask);
    sigprocmask (SIG_SETMASK, &mask, &prev_mask);

    action.sa_mask = mask;
    action.sa_handler = &sig_handler;

    int i;
    for (i = 0; i < NSIG; i++) {
        sigaction(i, &action, &prev_action[i]);
    }
    alarm(sec);
    sigsuspend(&prev_mask);
    for (i = 0; i < NSIG; i++) {
        sigaction(i, &prev_action[i], NULL);
    }
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    return timer;
}
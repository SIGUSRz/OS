#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
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
 
int main (int argc, char * argv []) {
    int i;
    pid_t pid = getpid(), pid_child [2];
    
    sigset_t mask, empty_mask;
    sigfillset(&mask);
    sigemptyset(&empty_mask);
    struct sigaction action;
    action.sa_handler = &handler;
    action.sa_mask = mask;
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
    
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    
    for (i = 0; i < 2 && (pid_child [i] = fork()) != 0; i++);
     
    calc1 ();
    
    if (pid_child[0] && pid_child[1]) {
        sigsuspend(&empty_mask);
        sigsuspend(&empty_mask);
        printf("Parent finish sync\n");
        kill(pid_child[0], SIGUSR1);
        kill(pid_child[1], SIGUSR2);
    } else if (pid_child[0] == 0) {
        kill(pid, SIGUSR1);
        printf("Child 1 receives clearance\n");
        sigsuspend(&empty_mask);
    } else if (pid_child[1] == 0) {
        kill(pid, SIGUSR2);
        printf("Child 2 receives clearance\n");
        sigsuspend(&empty_mask);
    }
    
    calc2 ();
     
    return 0;
}

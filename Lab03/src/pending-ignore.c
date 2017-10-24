#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv []) {

    sigset_t sig_rec;   /* list of blocked singals */
    sigset_t sig_pend;  /* list of pending signals */
    struct sigaction action;


    sigfillset(&sig_rec);
    sigprocmask (SIG_SETMASK, &sig_rec, NULL);

    sigemptyset (&sig_rec);
    sigaddset (&sig_rec,SIGINT);
    sigaddset (&sig_rec,SIGQUIT);

    /* mask SIGINT and SIGQUIT */
    sigprocmask (SIG_SETMASK, &sig_rec,NULL);

    action.sa_handler = SIG_IGN;
    action.sa_flags = 0;
    action.sa_mask = sig_rec;
    sigaction(SIGINT, &action, NULL);

    sleep(2);

    /* retrieve pending signals */
    sigpending (&sig_pend);
    if (sigismember ( &sig_pend,SIGINT) )
        printf("SIGINT received\n");
    if (sigismember ( &sig_pend,SIGQUIT) )
        printf("SIGQUIT received\n");

    sigdelset (&sig_rec,SIGINT);
    sigdelset (&sig_rec,SIGQUIT);
    sigprocmask (SIG_SETMASK, &sig_rec,NULL);

    printf ("End program \n");

    return EXIT_SUCCESS;
}

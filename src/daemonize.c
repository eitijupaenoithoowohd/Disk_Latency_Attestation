#include "disk_delay_of_writes.h"

/**
 *
 * Daemonize the process and listen the signals
 *
 */
void daemonize(){
    int pid;

    pid = fork();

    if(pid < 0){
        exit(1);
    }

    if(pid > 0){
        exit(0);
    }

    setsid();

    signal(SIGCHLD,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);

    signal(SIGHUP,signal_handler);
    signal(SIGTERM,signal_handler);
}

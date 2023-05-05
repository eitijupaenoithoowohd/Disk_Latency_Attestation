#include "disk_delay_of_writes.h"

void signal_handler(
    int sig
){
    switch(sig)
    {
        case SIGHUP:
            break;
        case SIGTERM:
            exit(0);
            break;
        default:
            return;
    }
}

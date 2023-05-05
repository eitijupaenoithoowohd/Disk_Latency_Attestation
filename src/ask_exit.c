#include "main.h"

/**
 *
 * Free allocated memmory before exit
 *
 */
void ask_exit(
    bool forced_exit,
    Return status,
    struct Event *event
){
    if(status == FAILURE || forced_exit)
    {
        // Free previously allocated memory of log
        for(int i = 0; i < RESEARCH_QUANTITY;i++)
        {
            if(event[i].error_string != NULL){
                free(event[i].error_string);
            }
        }

        if(status == FAILURE){
            exit(EXIT_FAILURE);
        } else {
            exit(EXIT_SUCCESS);
        }
    }
}

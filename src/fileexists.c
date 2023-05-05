#include "disk_delay_of_writes.h"

/**
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int fileexists(const char * filename){
    /* try to open file to read */
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

#include "main.h"

void save(
    const char *reportfile,
    struct Event *event,
    ssize_t position
)
{
    bool exists = false;

    if(fileexists(reportfile)){
        exists = true;
    }

    FILE *fp = fopen(reportfile, "a");

    // CSV file header
    if(exists == false){
        fprintf(fp,"Delay milliseconds between write start and its finish,Date and Time,Error Code,Error Message\n");
    }

    for(int i = 0; i < position;i++){

        // Convert timestamp to local date and time
        char buf[BUF_LEN] = {0};
        time_t rawtime = event[i].date;
        struct tm l_time;
        localtime_r(&rawtime,&l_time);
        strftime(buf, BUF_LEN, "%F %T", &l_time);

        // Concatenate a few strings in one: Error code, Error string and comas.
        char error_string[BUF_LEN] = {0};
        error_string[0] = '\0';
        if(event[i].error_code > 0)
        {
            snprintf(error_string, sizeof(error_string), ",%d,%s",event[i].error_code,event[i].error_string);
        }

        // Write every line to the log CSV file
        fprintf(fp,"%zu,%s%s\n",event[i].delay,buf,error_string);
    }

    fclose(fp);
}

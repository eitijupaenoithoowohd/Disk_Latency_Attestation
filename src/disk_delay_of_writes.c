/**
 * This is simple C program
 *
 */

#include "disk_delay_of_writes.h"

int main(const int argc, char ** const argv)
{
    /// Статус, который будет передан return() перед выходом.
    /// По умолчанию - функция отработала без ошибок.
    Return status = SUCCESS;

    ssize_t position = 0;
    typedef struct Event events[RESEARCH_QUANTITY];
    events event;
    // Initialize the structure with zeroes
    memset(&event,0,sizeof(events));

    // About this program and how to use it
    if(!intro(&argc,argv))
    {
        status = FAILURE;
    }

    ask_exit(false,status,event);

    const char *reportfile = argv[2];;
    const char *filename="sample.bin";
    const size_t path_len = strlen(argv[1]) + strlen(filename) + 2;

    // Path to the sample file on monitored storage
    char *path = NULL;
    if ((path = (char*)malloc(path_len * sizeof(char))) == NULL)
    {
        printf("Memory allocation error");
        return 0;
    }

    snprintf(path,path_len, "%s/%s",argv[1],filename);

    // Transform relative path to absolute
    char absolutepath[PATH_MAX] = {0};
    if(realpath(path,absolutepath) == NULL){
        // Yes, file does't exist yet and it is ok
        errno = 0;
    }
    free(path);

    ask_exit(false,status,event);


    // Turn to daemon
    daemonize();

    const size_t mega = 1024 * 1024;
    const size_t megabyte = mega * sizeof(uint8_t); // I know that
                                        // the sizeof(uint8_t) == 1,
                                        // don't warry ))) It would be important
                                        // if the type of variable will
                                        // be changed.
    while(position < RESEARCH_QUANTITY)
    {
        uint8_t *buffer;

        event[position].error_code = 0; // No error by default
        event[position].error_string = NULL; // Infill value. "if lvalue == NULL" will be later

        // Allocate 1 Megabyte
        if ((buffer = (uint8_t*)malloc(megabyte)) == NULL)
        {
            printf("Memory allocation error");
            status = FAILURE;
            break;
        }

        fill_1_MB_buffer_with_random_data(buffer,(ssize_t)mega,0,255);

        /* Timestamp in millisecond. */
        struct timeval time;
        gettimeofday(&time, NULL);
        time_t start_msec = (time.tv_sec * 1000) + (time.tv_usec / 1000);

        /* Current timestamp in second */
        event[position].date = time.tv_sec;

        int file_to_test_delay = open(absolutepath, O_WRONLY
                                                  | O_CREAT | O_TRUNC
                                               // | O_DIRECT // You can use it ony if your OS support it
                                                  | O_SYNC , S_IRUSR | S_IWUSR);
        if (file_to_test_delay < 0)
        {
            if (errno > 0)
            {
                event[position].error_code = errno;
                event[position].error_string = strdup(strerror(errno));
            }

        } else if (write(file_to_test_delay,buffer,mega) != (ssize_t)megabyte)
        {
            if (errno > 0)
            {
                event[position].error_code = errno;
                event[position].error_string = strdup(strerror(errno));
            }

        } else {
            if (errno > 0)
            {
                event[position].error_code = errno;
                event[position].error_string = strdup(strerror(errno));
            }
        }

        free(buffer);
        buffer = NULL;

        // Cache clean
        posix_fadvise(file_to_test_delay,0,0,POSIX_FADV_DONTNEED);

        // write operation has finished
        close(file_to_test_delay);

        // Modification time of file reset to currnet, like the "touch"
        utimensat(0,absolutepath, NULL, 0);

        struct stat info;
        stat(absolutepath, &info);
        time_t finish_msec = info.st_mtime * 1000 + info.st_mtim.tv_nsec / 1000000;

        event[position].delay = 0;
        if (finish_msec > start_msec)
        {
            event[position].delay = finish_msec - start_msec;
        }

        if (remove(absolutepath))
        {
            printf("The file %s is not Deleted\n",absolutepath);
            status = FAILURE;
            break;
        }

        position++;

        // Pause
        sleep(INTERVAL);
    }

    ask_exit(false,status,event);

    save(reportfile,event,position);

    ask_exit(true,status,event);
}

/**
 * Main .h file for main.c
 *
 */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>       /* random funcrions */
#include <sys/time.h>   /* gettimeofday() */
#include <sys/timeb.h>  /* ftime, timeb (for timestamp in millisecond) */
#include <errno.h>
#include <string.h>
#include <limits.h>     /* realpath() */

#define BUF_LEN 256
#define RESEARCH_QUANTITY 5
#define INTERVAL 10     // Interval in seconds between researche

/// Function exit status
/// Designed as an enum
typedef enum
{
    /// In case of fail
    FAILURE, // Actual value 0

    /// In case of success
    SUCCESS, // Actual value 1

} Return;

struct Event {
    time_t delay;
    time_t date;
    int error_code;
    char *error_string;
};

void fill_1_MB_buffer_with_random_data(
    uint8_t*,
    ssize_t,
    int,
    int
);

bool intro(
    const int * const,
    char ** const
);

int fileexists(
    const char*
);

void daemonize();

void signal_handler(
    int sig
);

void ask_exit(
    bool,
    Return,
    struct Event*
);

void save(
    const char*,
    struct Event*,
    ssize_t
);

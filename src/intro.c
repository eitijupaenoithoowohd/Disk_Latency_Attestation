#include "main.h"

/**
 * @brief About the program
 * @details Small intro and argument's check
 *
 */
bool intro(
    const int * const argc,
    char ** const argv
){
    if (*argc < 2) {
        printf("You were not passed any argument\n");
        printf("The best is to write report file to another place, than you are monitored\n");
        printf("Usage:\n");
        printf("%s /monitored_mountpoint /path/to/the/report_file_in.csv\n",basename(argv[0]));
        printf("For example:\n");
        printf("%s /data /var/log/report.csv\n\n",basename(argv[0]));
        printf(
            "Disk Latency Attestation Tool\n"
            "A simple yet effective tool to track storage latency during write operations.\n\n"
            "This utility operates as a daemon in the RAM, generates random data and writes it\n"
            "as a file to the specified storage for a certain intervals, measuring the time it\n"
            "took to write a file of a defined size.\n\n"
            "The data cannot be cached because each new writing test is generated as pseudo-random\n"
            " data.\n\n"
            "The application generates a log in the form of a CSV table, detailing the date, time\n"
            "of each event, duration of data set writing, error codes and error messages.\n"
        );
        return false;
    }

    return true;
}

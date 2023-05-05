#include "main.h"

void fill_1_MB_buffer_with_random_data(
    uint8_t *buffer,
    ssize_t size,
    int max,
    int min
)
{
    int i;
    for (i = 0; i < size; i++) {
        buffer[i] = (uint8_t)((rand() % (max - min + 1)) + min);
    }
}

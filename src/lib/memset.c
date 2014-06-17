#include <lib/memset.h>


void *memset(void *ptr, int value, size_t num)
{
    char *dst = ptr;
    while (num > 0)
    {
        dst[--num] = (char)value;
    }
    return ptr;
}


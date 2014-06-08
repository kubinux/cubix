#include "memcpy.h"

void *memcpy(void *dest, const void *src, size_t num)
{
    const char *src_it = (const char *)src;
    char *dest_it = (char *)dest;
    while (num--)
    {
        *(dest_it++) = *(src_it++);
    }
    return dest;
}


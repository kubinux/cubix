#include <lib/assert.h>
#include <lib/printf.h>

void _assert(const char *cond, const char *msg, const char *file, int line)
{
    printf("FATAL: ");
    printf("assertion %s failed ", cond);
    if (msg)
    {
        printf("(%s) ", msg);
    }
    printf("at %s:%d\n", file, line);
    while (1)
    {
        __asm__ __volatile__("cli\nhlt" ::: "memory");
    }
}


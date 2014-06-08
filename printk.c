#include "printk.h"
#include "vga.h"
#include <stdarg.h>
#include <stdint.h>

static void put_char(char c)
{
    vga_putc(c);
}

static int put_int32(int32_t val)
{
    // TODO
    return 0;
}

static int put_uint32(uint32_t val)
{
    // TODO
    return 0;
}

static int put_uint32_hex(uint32_t val)
{
    // TODO
    return 0;
}

static int put_str(const char *str)
{
    int num_written = 0;
    while (*str)
    {
        put_char(*str);
        ++num_written;
        ++str;
    }
    return num_written;
}

static int vprintk(const char *fmt, va_list va)
{
    int num_written = 0;
    char c = 0;
    while ((c = *(fmt++)))
    {
        if (c != '%')
        {
            put_char(c);
            ++num_written;
            continue;
        }

        c = *(fmt++);
        switch (c)
        {
        case '%':
            put_char(c);
            ++num_written;
            break;
        case 's':
            num_written += put_str(va_arg(va, const char*));
            break;
        case 'd':
            num_written += put_int32(va_arg(va, int32_t));
            break;
        case 'u':
            num_written += put_uint32(va_arg(va, uint32_t));
            break;
        case 'x':
            num_written += put_uint32_hex(va_arg(va, uint32_t));
        default:
            return -1;
        }
    }
    return num_written;
}

int printk(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int ret = vprintk(fmt, va);
    va_end(va);
    return ret;
}


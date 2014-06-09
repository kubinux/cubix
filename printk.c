#include "printk.h"
#include "vga.h"
#include <stdarg.h>
#include <stdint.h>

static void put_char(char c)
{
    vga_putc(c);
}

static int put_uint32(uint32_t val, int radix)
{
    char digits[32] = { 0 };
    int num_digits = 0;
    do
    {
        digits[num_digits++] = "0123456789abcdef"[val % radix];
        val /= radix;
    }
    while (val);
    for (int i = num_digits - 1; i >= 0; --i)
    {
        put_char(digits[i]);
    }
    return num_digits;
}

static int put_int32(int32_t val, int radix)
{
    int num_written = 0;
    if (val < 0)
    {
        put_char('-');
        ++num_written;
        val = -val;
    }
    num_written += put_uint32((uint32_t)val, radix);
    return num_written;
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
            num_written += put_int32(va_arg(va, int32_t), 10);
            break;
        case 'u':
            num_written += put_uint32(va_arg(va, uint32_t), 10);
            break;
        case 'x':
            num_written += put_uint32(va_arg(va, uint32_t), 16);
            break;
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


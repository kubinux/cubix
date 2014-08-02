// Copyright (C) 2014 Jakub Lewandowski <jakub.lewandowski@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <lib/printf.h>
#include <io/vga.h>
#include <stdarg.h>
#include <stdint.h>

static void put_char(char c)
{
    vga_putc(c);
}

static int put_unsigned_int(uintmax_t val, int radix)
{
    char digits[64] = {0};
    int num_digits = 0;
    do
    {
        digits[num_digits++] = "0123456789abcdef"[val % radix];
        val /= radix;
    } while (val);
    for (int i = num_digits - 1; i >= 0; --i)
    {
        put_char(digits[i]);
    }
    return num_digits;
}

static int put_signed_int(intmax_t val, int radix)
{
    int num_written = 0;
    if (val < 0)
    {
        put_char('-');
        ++num_written;
        val = -val;
    }
    num_written += put_unsigned_int((uint32_t)val, radix);
    return num_written;
}

static int put_str(const char* str)
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

enum format
{ 
    FMT_INVALID,
    FMT_MODULO,
    FMT_STRING,
    FMT_INT_DEC,
    FMT_UNSIGNED_INT_DEC,
    FMT_UNSIGNED_INT_HEX,
    FMT_LONG_INT_DEC,
    FMT_UNSIGNED_LONG_INT_DEC,
    FMT_UNSIGNED_LONG_INT_HEX,
    FMT_VOID_PTR
};

static enum format parse_format(const char** fmt)
{
    char c = **fmt;
    ++(*fmt);
    switch (c)
    {
    case '%':
        return FMT_MODULO;
    case 's':
        return FMT_STRING;
    case 'd':
        return FMT_INT_DEC;
    case 'u':
        return FMT_UNSIGNED_INT_DEC;
    case 'x':
        return FMT_UNSIGNED_INT_HEX;
    case 'p':
        return FMT_VOID_PTR;
    case 'l':
        c = **fmt;
        ++(*fmt);
        switch (c)
        {
        case 'd':
            return FMT_LONG_INT_DEC;
        case 'u':
            return FMT_UNSIGNED_LONG_INT_DEC;
        case 'x':
            return FMT_UNSIGNED_LONG_INT_HEX;
        default:
            return FMT_INVALID;
        }
    default:
        return FMT_INVALID;
    }
}

static int vprintf(const char* fmt_str, va_list va)
{
    int num_written = 0;
    char c = 0;
    while ((c = *(fmt_str++)))
    {
        if (c != '%')
        {
            put_char(c);
            ++num_written;
            continue;
        }

        enum format fmt = parse_format(&fmt_str);
        switch (fmt)
        {
        case FMT_MODULO:
            put_char('%');
            ++num_written;
            break;
        case FMT_STRING:
            num_written += put_str(va_arg(va, const char*));
            break;
        case FMT_INT_DEC:
            num_written += put_signed_int(va_arg(va, int), 10);
            break;
        case FMT_UNSIGNED_INT_DEC:
            num_written += put_unsigned_int(va_arg(va, unsigned int), 10);
            break;
        case FMT_UNSIGNED_INT_HEX:
            num_written += put_unsigned_int(va_arg(va, unsigned int), 16);
            break;
        case FMT_LONG_INT_DEC:
            num_written += put_signed_int(va_arg(va, long int), 10);
            break;
        case FMT_UNSIGNED_LONG_INT_DEC:
            num_written += put_unsigned_int(va_arg(va, unsigned long int), 10);
            break;
        case FMT_UNSIGNED_LONG_INT_HEX:
            num_written += put_unsigned_int(va_arg(va, unsigned long int), 16);
            break;
        case FMT_VOID_PTR:
            num_written += put_unsigned_int((uintmax_t)va_arg(va, void *), 16);
            break;
        default:
            return -1;
        }
    }
    return num_written;
}

int printf(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int ret = vprintf(fmt, va);
    va_end(va);
    return ret;
}


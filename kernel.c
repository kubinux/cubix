#include "vga.h"
#include "printk.h"
#include <stddef.h>
#include <stdint.h>

enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_RED = 4,
    COLOR_WHITE = 15
};

uint8_t vga_make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

uint16_t vga_make_entry(char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)color << 8;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len] != 0)
    {
        ++len;
    }
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t term_row;
size_t term_column;
uint8_t term_color;
uint16_t* term_buffer;

void term_initialize()
{
    term_row = 0;
    term_column = 0;
    term_color = vga_make_color(COLOR_WHITE, COLOR_BLACK);
    term_buffer = (uint16_t*)0xb8000;
    for (size_t y = 0; y < VGA_HEIGHT; ++y)
    {
        for (size_t x = 0; x < VGA_WIDTH; ++x)
        {
            const size_t index = y * VGA_WIDTH + x;
            term_buffer[index] = vga_make_entry(' ', term_color);
        }
    }
}

void term_set_color(uint8_t color)
{
    term_color = color;
}

void term_put_entry_at(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    term_buffer[index] = vga_make_entry(c, color);
}

void term_put_char(char c)
{
    term_put_entry_at(c, term_color, term_column, term_row);
    if (++term_column == VGA_WIDTH)
    {
        term_column = 0;
        if (++term_column == VGA_HEIGHT)
        {
            term_row = 0;
        }
    }
}

void term_write_string(const char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
    {
        term_put_char(str[i]);
    }
}

void sleep(void)
{
    for (int i = 0; i < 1000000; ++i)
    {
    }
}

void kernel_main(void)
{
    vga_clear();
    printk("Hello %s!\n", "Kuba");
}


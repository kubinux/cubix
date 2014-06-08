#include "vga.h"
#include "port_io.h"
#include "memcpy.h"
#include <stdint.h>

static uint16_t * const VIDEO_MEMORY = (uint16_t *)0xb8000;

static const uint16_t MAX_ROWS = 25;

static const uint16_t MAX_COLS = 80;

static const uint8_t WHITE_ON_BLACK = 0x0f;

static const port_t CTRL_PORT = 0x3d4;

static const port_t DATA_PORT = 0x3d5;

struct cursor
{
    uint16_t row;
    uint16_t column;
};

static uint16_t get_offset(struct cursor cur)
{
    return cur.row * MAX_COLS + cur.column;
}

static uint16_t *get_vga_address(struct cursor cur)
{
    return VIDEO_MEMORY + get_offset(cur);
}

static void set_cursor(struct cursor cur)
{
    uint16_t offset = get_offset(cur);
    port_byte_out(CTRL_PORT, 14);
    port_byte_out(DATA_PORT, (uint8_t)(offset >> 8));
    port_byte_out(CTRL_PORT, 15);
    port_byte_out(DATA_PORT, (uint8_t)(offset));
}

static struct cursor get_cursor(void)
{
    port_byte_out(CTRL_PORT, 14);
    uint8_t high_byte = port_byte_in(DATA_PORT);
    port_byte_out(CTRL_PORT, 15);
    uint8_t low_byte = port_byte_in(DATA_PORT);
    uint16_t offset = high_byte;
    offset <<= 8;
    offset += low_byte;
    struct cursor cur =
    { 
        (uint16_t)(offset / MAX_COLS),
        (uint16_t)(offset % MAX_COLS)
    };
    return cur;
}

static uint16_t make_cell(uint8_t ch, uint8_t attr)
{
    uint16_t cell = attr;
    cell <<= 8;
    cell += ch;
    return cell;
}

static void clear_row(int row)
{
    uint16_t cell = make_cell(' ', WHITE_ON_BLACK);
    uint16_t *mem = VIDEO_MEMORY + row * MAX_COLS;
    for (int i = 0; i < MAX_COLS; ++i)
    {
        mem[i] = cell;
    }
}

static void scroll(void)
{
    for (int row = 1; row < MAX_ROWS; ++row)
    {
        memcpy(VIDEO_MEMORY + (row - 1) * MAX_COLS,
               VIDEO_MEMORY + row * MAX_COLS,
               MAX_COLS * 2);
    }
    clear_row(MAX_ROWS - 1);
}

static void update_screen(struct cursor cur)
{
    if (cur.column == MAX_COLS)
    {
        ++cur.row;
        cur.column = 0;
    }
    if (cur.row == MAX_ROWS)
    {
        scroll();
        --cur.row;
    }
    set_cursor(cur);
}

void vga_clear(void)
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        clear_row(i);
    }
    struct cursor top_left = { 0, 0 };
    set_cursor(top_left);
}

void vga_putc(char c)
{
    struct cursor cur = get_cursor();
    if (c == '\n')
    {
        ++cur.row;
        cur.column = 0;
    }
    else
    {
        uint16_t cell = make_cell(c, WHITE_ON_BLACK);
        *get_vga_address(cur) = cell;
        ++cur.column;
        if (cur.column == MAX_COLS)
        {
            ++cur.row;
            cur.column = 0;
        }
    }
    update_screen(cur);
}


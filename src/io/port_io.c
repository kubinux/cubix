#include "port_io.h"

uint8_t port_byte_in(port_t port)
{
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void port_byte_out(port_t port, uint8_t data)
{
    __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

uint16_t port_word_in(port_t port)
{
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
}

void port_word_out(port_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx" : : "a"(data), "d"(port));
}


#ifndef PORT_IO_H_INCLUDED_SUESLCZD
#define PORT_IO_H_INCLUDED_SUESLCZD

#include <stdint.h>

typedef uint16_t port_t;

uint8_t port_byte_in(port_t port);

void port_byte_out(port_t port, uint8_t data);

uint16_t port_word_in(port_t port);

void port_word_out(port_t port, uint16_t data);

#endif // include guard


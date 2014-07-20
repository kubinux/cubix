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

#ifndef PORT_IO_H_INCLUDED_SUESLCZD
#define PORT_IO_H_INCLUDED_SUESLCZD

#include <stdint.h>

typedef uint16_t port_t;

uint8_t port_byte_in(port_t port);

void port_byte_out(port_t port, uint8_t data);

uint16_t port_word_in(port_t port);

void port_word_out(port_t port, uint16_t data);

#endif // include guard


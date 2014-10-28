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
#include <io/port_io.h>
#include <stdint.h>

static int count = 0;

void isr_dispatch(uint64_t num)
{
    if (num == 33)
    {
        uint8_t byte = port_byte_in(0x60);
        printf("got %u\n", byte);
    }
    ++count;
    if (num > 31)
    {
        if (num > 39)
        {
            port_byte_out(0xA0, 0x20);
        }
        port_byte_out(0x20, 0x20);
    }
    if (num != 32)
    {
        printf("Hello Interrupt %lu, %d!\n", num, count);
    }
}


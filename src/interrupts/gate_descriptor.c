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

#include <interrupts/gate_descriptor.h>
#include <stdbool.h>


static void init_gate(struct gate_descriptor *desc, uintptr_t entry_point,
                      uint8_t type, uint8_t ist)
{
    *desc = (struct gate_descriptor){ 0 };
    desc->offset_lo = (entry_point & 0xFFFF);
    desc->selector = 0x8; // kernel code segment
    desc->ist = ist;
    desc->type = type;
    desc->present = 1;
    desc->offset_hi = (entry_point >> 16) & 0xFFFFFFFFFFFF;
}


void init_trap_gate(struct gate_descriptor *desc, uintptr_t entry_point)
{
    init_gate(desc, entry_point, 0xF, 0);
}


void init_interrupt_gate(struct gate_descriptor *desc, uintptr_t entry_point)
{
    init_gate(desc, entry_point, 0xE, 0);
}


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

#ifndef GATE_DESCRIPTOR_H_INCLUDED_JN7M9XEV
#define GATE_DESCRIPTOR_H_INCLUDED_JN7M9XEV

#include <stdint.h>


struct gate_descriptor
{
    uint64_t offset_lo : 16;
    uint64_t selector : 16;
    uint64_t ist : 3;
    uint64_t : 5;
    uint64_t type : 5;
    uint64_t dpl : 2;
    uint64_t present : 1;
    uint64_t offset_hi : 48;
    uint64_t : 32;
} __attribute__((packed));


void init_trap_gate(struct gate_descriptor *desc, uintptr_t entry_point);


void init_interrupt_gate(struct gate_descriptor *desc, uintptr_t entry_point);


#endif // include guard


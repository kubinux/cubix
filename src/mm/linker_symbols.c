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

#ifndef LINKER_SYMBOLS_C_INCLUDED_NVSSJWRR
#define LINKER_SYMBOLS_C_INCLUDED_NVSSJWRR

#include <mm/linker_symbols.h>


extern char _kernel_virt_offset[];

extern char _kernel_phys_start[];

extern char _kernel_virt_start[];

extern char _kernel_phys_end[]; 

extern char _kernel_virt_end[];

const uintptr_t kernel_virt_offset = (uintptr_t)&_kernel_virt_offset;

const uintptr_t kernel_phys_start = (uintptr_t)&_kernel_phys_start;

const uintptr_t kernel_virt_start = (uintptr_t)&_kernel_virt_start;

const uintptr_t kernel_phys_end = (uintptr_t)&_kernel_phys_end; 

const uintptr_t kernel_virt_end = (uintptr_t)&_kernel_virt_end;

#endif // include guard


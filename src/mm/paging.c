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

#include <mm/paging.h>
#include <mm/linker_symbols.h>
#include <mm/phys_allocator.h>
#include <lib/printf.h>
#include <lib/memset.h>
#include <lib/assert.h>
#include <stdint.h>


extern size_t get_pt_index(uintptr_t virt_address);


extern size_t get_pd_index(uintptr_t virt_address);


extern size_t get_pdp_index(uintptr_t virt_address);


extern size_t get_pml4_index(uintptr_t virt_address);


static inline uint64_t *get_pt(size_t pml4i, size_t pdpi, size_t pdi)
{
    return (uint64_t *)(0xFFFFFF0000000000 + 0x40000000 * pml4i +
                        0x200000 * pdpi + 0x1000 * pdi);
}


static inline uint64_t *get_pd(size_t pml4i, size_t pdpi)
{
    return (uint64_t *)(0xFFFFFF7FC0000000 + 0x200000 * pml4i + 0x1000 * pdpi);
}


static inline uint64_t *get_pdp(size_t pml4i)
{
    return (uint64_t *)(0xFFFFFF7FFFE00000 + 0x1000 * pml4i);
}


static inline uint64_t *get_pml4(void)
{
    return (uint64_t *)(0xFFFFFF7FFFFFF000);
}


void invalidate_page(uintptr_t virt_address)
{
    __asm__ __volatile__("invlpg (%0)" ::"r"(virt_address) : "memory");
}


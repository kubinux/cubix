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

#ifndef PAGING_H_INCLUDED_LGNA9BIS
#define PAGING_H_INCLUDED_LGNA9BIS

#include <stdint.h>
#include <stddef.h>


#define PAGE_SIZE 0x1000


typedef uint64_t mmap_entry_t[512] __attribute__((aligned(0x1000)));


inline size_t get_pt_index(uintptr_t virt_address)
{
    return (virt_address >> 12) & 0x1FF;
}


inline size_t get_pd_index(uintptr_t virt_address)
{
    return (virt_address >> 21) & 0x1FF;
}


inline size_t get_pdp_index(uintptr_t virt_address)
{
    return (virt_address >> 30) & 0x1FF;
}


inline size_t get_pml4_index(uintptr_t virt_address)
{
    return (virt_address >> 39) & 0x1FF;
}


void invalidate_page(uintptr_t virt_address);


#endif // include guard


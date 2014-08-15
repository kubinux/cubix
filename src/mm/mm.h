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

#ifndef MM_H_INCLUDED_PYZC6EWG
#define MM_H_INCLUDED_PYZC6EWG

#include <mm/address_range.h>
#include <mm/paging.h>
#include <mm/page.h>
#include <stdint.h>


#define PAGE_OFFSET 0xFFFF880000000000


void mm_init(struct address_range *ranges, size_t num_ranges);


inline uintptr_t mm_va(uintptr_t phys_address)
{
    return phys_address + PAGE_OFFSET;
}


inline uintptr_t mm_pa(uintptr_t virt_address)
{
    return virt_address - PAGE_OFFSET;
}


inline struct page *mm_page_from_virt(uintptr_t virt_address)
{
    extern struct page *first_page_struct;
    return first_page_struct + (mm_pa(virt_address) >> 12);
}


inline uintptr_t mm_page_to_virt(struct page *page)
{
    extern struct page *first_page_struct;
    return mm_va((page - first_page_struct) << 12);
}


#endif // include guard


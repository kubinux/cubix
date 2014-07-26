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

#ifndef PHYS_ALLOCATOR_H_INCLUDED_VACHCL58
#define PHYS_ALLOCATOR_H_INCLUDED_VACHCL58

#include <mm/address_range.h>
#include <stdint.h>


void init_phys_allocator(const struct address_range *regions, int num_regions);


void print_phys_mem(void);


uintptr_t alloc_phys_page(void);


void free_phys_page(uintptr_t phys_address);


#endif // include guard


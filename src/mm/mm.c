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

#include <mm/mm.h>
#include <mm/paging.h>
#include <mm/linker_symbols.h>
#include <mm/phys_allocator.h>
#include <lib/printf.h>
#include <stdint.h>


static uintptr_t find_max_phys_address(const struct mmap_region *regions,
                                       int num_regions)
{
    uintptr_t max_address = 0;
    for (int i = 0; i < num_regions; ++i)
    {
        uintptr_t new_max = regions[i].addr + regions[i].size - 1;
        if (new_max > max_address)
        {
            max_address = new_max;
        }
    }
    return max_address;
}


void mm_init(const struct mmap_region *regions, int num_regions)
{
    init_kernel_pages();
    for (int i = 0; i < num_regions; ++i)
    {
        printf("addr = 0x%lx, size = %lu, usable = %d\n",
               regions[i].addr,
               regions[i].size,
               regions[i].usable);
    }
    printf("max addr: %lu\n", find_max_phys_address(regions, num_regions));
    printf("kernel end: %lx\n", kernel_phys_end);
    init_phys_allocator(regions, num_regions);
}


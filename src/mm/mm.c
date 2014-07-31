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
#include <mm/page.h>
#include <mm/linker_symbols.h>
#include <mm/phys_allocator.h>
#include <lib/printf.h>
#include <lib/memcpy.h>
#include <lib/memset.h>
#include <lib/assert.h>
#include <stdint.h>
#include <stddef.h>


static struct address_range *mem_regions_begin;
static struct address_range *mem_regions_end;
struct page *first_page_struct;


static mmap_entry_t pml4;


static uintptr_t alloc_phys_page_early(void)
{
    ASSERT_MSG(mem_regions_begin != mem_regions_end, "No memory left");
    ASSERT_MSG(mem_regions_begin->begin != mem_regions_begin->end,
               "Empty memory region");
    uintptr_t phys_address = mem_regions_begin->begin;
    mem_regions_begin->begin += PAGE_SIZE;
    if (mem_regions_begin->begin == mem_regions_begin->end)
    {
        ++mem_regions_begin;
    }
    void *page = (void *)phys_address;
    memset(page, 0, PAGE_SIZE);
    return phys_address;
}


static void init_memory_regions(struct address_range *ranges,
                                size_t num_ranges)
{
    ASSERT_MSG(num_ranges > 0, "No memory regions");
    mem_regions_begin = ranges;
    mem_regions_end = ranges + num_ranges;
}


static void map_address_range_early(uintptr_t beg_phys, uintptr_t end_phys,
                                    uintptr_t virt_offset, uint32_t flags)
{
    uint64_t *pd = NULL;
    for (uintptr_t phys = beg_phys; phys < end_phys; phys += 0x200000)
    {
        uintptr_t virt = phys + virt_offset;
        size_t pdi = get_pd_index(virt);
        if (pdi == 0)
        {
            pd = (uint64_t *)alloc_phys_page_early();
            size_t pml4i = get_pml4_index(virt);
            uint64_t *pdp = NULL;
            if (pml4[pml4i] != 0)
            {
                pdp = (uint64_t *)(pml4[pml4i] & (~0xFFF));
            }
            else
            {
                pdp = (uint64_t *)alloc_phys_page_early();
                pml4[pml4i] = (uintptr_t)pdp | 0x1 | flags;
            }
            size_t pdpi = get_pdp_index(virt);
            pdp[pdpi] = (uintptr_t)pd | 0x1 | flags;
        }
        pd[pdi] = phys | 0x81 | flags;
    }
}


static void init_kernel_pages(void)
{
    struct address_range *last_region = mem_regions_end - 1;
    map_address_range_early(0, 0x40000000, kernel_virt_offset, 0x2);
    map_address_range_early(0, last_region->end, PAGE_OFFSET, 0x2);
    __asm__ __volatile__(
        "movq %0, %%cr3" ::"r"((uintptr_t)pml4 - kernel_virt_offset)
        : "memory");
}


static void init_page_structs(void)
{
    struct address_range *last_region = mem_regions_end - 1;
    size_t num_phys_pages = last_region->end / PAGE_SIZE;
    size_t pages_needed = num_phys_pages * sizeof(struct page) / PAGE_SIZE + 1;
    first_page_struct = (struct page *)alloc_pages(pages_needed);
    ASSERT_MSG(first_page_struct, "failed to allocate page structs");
    memset(first_page_struct, 0, num_phys_pages * sizeof(struct page));
}


extern uintptr_t mm_va(uintptr_t phys_address);


extern uintptr_t mm_pa(uintptr_t virt_address);


extern struct page *mm_page(uintptr_t virt_address);


void mm_init(struct address_range *ranges, size_t num_ranges)
{
    init_memory_regions(ranges, num_ranges);
    init_kernel_pages();
    init_phys_allocator(mem_regions_begin,
                        mem_regions_end - mem_regions_begin);
    init_page_structs();
}


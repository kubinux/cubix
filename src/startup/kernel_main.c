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

#include <startup/multiboot.h>
#include <mm/mm.h>
#include <mm/paging.h>
#include <mm/phys_allocator.h>
#include <mm/linker_symbols.h>
#include <io/vga.h>
#include <lib/printf.h>
#include <lib/assert.h>

#include <stddef.h>
#include <stdint.h>


static struct address_range mem_regions[64];


static int extract_memory_regions(const multiboot_info_t *mbi,
                                  struct address_range *regions,
                                  int max_num_regions)
{
    uintptr_t mmap_addr = mbi->mmap_addr;
    int num_regions = 0;
    while ((mmap_addr < mbi->mmap_addr + mbi->mmap_length) &&
           (num_regions < max_num_regions))
    {
        typedef multiboot_memory_map_t mmap_t;
        const mmap_t *mmap = (const mmap_t *)mmap_addr;

        uintptr_t kernel_end = (kernel_phys_end + PAGE_SIZE) & (~0xFFF);

        if (mmap->type == 1)
        {
            uintptr_t beg = mmap->addr;
            if (beg & 0xFFF)
            {
                beg = (beg + PAGE_SIZE) & (~0xFFF);
            }

            uintptr_t end = ((mmap->addr + mmap->len - 1) & (~0xFFF)) + 1;

            if (beg < kernel_end)
            {
                beg = kernel_end;
            }

            if (beg < end)
            {
                regions[num_regions].begin = beg;
                regions[num_regions].end = end;
                ++num_regions;
            }
        }
        mmap_addr += mmap->size + sizeof(mmap->size);
    }
    return num_regions;
}


static void init_memory(const multiboot_info_t *mbi)
{
    int num_regions = extract_memory_regions(
        mbi, mem_regions, sizeof(mem_regions) / sizeof(*mem_regions));
    mm_init(mem_regions, num_regions);
}


static void check_multiboot(uint32_t magic, const multiboot_info_t *mbi)
{
    (void)magic;
    (void)mbi;
    // TODO:
    //   * check the cookie value
    //   * check all required flags are set
}


void main(uint32_t magic, const multiboot_info_t *mbi)
{
    vga_clear();
    check_multiboot(magic, mbi);
    init_memory(mbi);

    uintptr_t p1 = alloc_phys_page();
    uintptr_t p2 = alloc_phys_page();
    uintptr_t p3 = alloc_phys_page();

    printf("After 3 allocations:\n");
    print_phys_mem();

    free_phys_page(p2);
    printf("After free 2:\n");
    print_phys_mem();

    free_phys_page(p1);
    printf("After free 1:\n");
    print_phys_mem();

    free_phys_page(p3);
    printf("After free 3:\n");
    print_phys_mem();

    // TODO
    // put stuff here

    ASSERT_MSG(0, "control reached end of main");
}


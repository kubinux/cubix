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
#include <io/vga.h>
#include <lib/printf.h>
#include <lib/assert.h>

#include <stddef.h>
#include <stdint.h>


static int extract_memory_regions(const multiboot_info_t *mbi,
                                  struct mmap_region *regions,
                                  int max_num_regions)
{
    uintptr_t mmap_addr = mbi->mmap_addr;
    int num_regions = 0;
    while ((mmap_addr < mbi->mmap_addr + mbi->mmap_length) &&
           (num_regions < max_num_regions))
    {
        typedef multiboot_memory_map_t mmap_t;
        const mmap_t *mmap = (const mmap_t *)mmap_addr;

        regions[num_regions].addr = mmap->addr;
        regions[num_regions].size = mmap->len;
        regions[num_regions].usable = (mmap->type == 1);

        mmap_addr += mmap->size + sizeof(mmap->size);
        ++num_regions;
    }
    return num_regions;
}


static void init_memory(const multiboot_info_t *mbi)
{
    struct mmap_region regions[16];
    int num_regions = extract_memory_regions(
        mbi, regions, sizeof(regions) / sizeof(*regions));
    mm_init(regions, num_regions);
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
    print_phys_mem();

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
    ASSERT_MSG(0, "control reached end of main");
}


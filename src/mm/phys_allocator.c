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

#include <mm/phys_allocator.h>
#include <mm/linker_symbols.h>
#include <mm/paging.h>
#include <lib/memcpy.h>
#include <lib/printf.h>
#include <lib/assert.h>
#include <stdint.h>


static mmap_entry_t tmp_page;


static const uint64_t page_size = 0x1000;


static uintptr_t early_mem;


static uintptr_t first_node_addr;


static inline uintptr_t get_early_free_mem(void)
{
    return ((kernel_phys_end + page_size) & (~0xFFF)) + early_mem;
}


uintptr_t alloc_phys_page_early(void)
{
    uintptr_t mem = get_early_free_mem();
    early_mem += page_size;
    return mem;
}


struct node
{
    uintptr_t begin;
    uintptr_t end;
    uintptr_t next;
};


static inline void read_node(struct node *node, uintptr_t phys_address)
{
    remap_kernel_page((uintptr_t)&tmp_page, phys_address);
    memcpy(node, &tmp_page, sizeof(struct node));
    ASSERT_MSG(node->begin == phys_address, "corrupted memory node");
}


static inline void write_node(const struct node *node)
{
    remap_kernel_page((uintptr_t)&tmp_page, node->begin);
    memcpy(tmp_page, node, sizeof(struct node));
}


static void try_merge_with_next(struct node *node)
{
    if (node->end == node->next)
    {
        struct node next_node;
        read_node(&next_node, node->next);
        node->next = next_node.next;
        node->end = next_node.end;
    }
}


static inline void insert_node(struct node *node)
{
    if (first_node_addr == 0 || node->begin < first_node_addr)
    {
        node->next = first_node_addr;
        try_merge_with_next(node);
        write_node(node);
        first_node_addr = node->begin;
        return;
    }

    struct node current = { 0 };
    read_node(&current, first_node_addr);

    while (current.next != 0)
    {
        struct node next_node;
        read_node(&next_node, current.next);
        if (node->begin < next_node.begin)
        {
            break;
        }
        current = next_node;
    }

    node->next = current.next;
    current.next = node->begin;
    try_merge_with_next(node);
    write_node(node);
    try_merge_with_next(&current);
    write_node(&current);
}


void print_phys_mem(void)
{
    uintptr_t next = first_node_addr;
    while (next)
    {
        struct node node;
        read_node(&node, next);
        printf("Node [0x%lx, 0x%lx)\n", node.begin, node.end);
        next = node.next;
    }
}


void init_phys_allocator(const struct mmap_region *regions, int num_regions)
{
    uintptr_t free_mem_begin = get_early_free_mem();
    for (int i = 0; i < num_regions; ++i)
    {
        if (!regions[i].usable)
        {
            continue;
        }
        uintptr_t beg = regions[i].addr;
        if (beg & 0xFFF)
        {
            beg = (beg + page_size) & (~0xFFF);
        }
        uintptr_t end = beg + (regions[i].size / page_size) * page_size;
        if (beg == end)
        {
            continue;
        }
        if (end <= free_mem_begin + 1)
        {
            continue;
        }
        if (beg <= free_mem_begin)
        {
            beg = free_mem_begin;
        }
        struct node node = { .begin = beg, .end = end }; 
        insert_node(&node);
    }
}


uintptr_t alloc_phys_page(void)
{
    uintptr_t res = 0;
    if (first_node_addr != 0)
    {
        struct node first_node;
        res = first_node_addr;
        read_node(&first_node, first_node_addr);
        if (first_node.end - first_node.begin == page_size)
        {
            first_node_addr = first_node.next;
        }
        else
        {
            first_node.begin += page_size;
            write_node(&first_node);
            first_node_addr = first_node.begin;
        }
    }
    return res;
}


void free_phys_page(uintptr_t phys_address)
{
    ASSERT_MSG(!(phys_address & 0xFFF), "address needs to be page-aligned");
    struct node new_node =
    {
        .begin = phys_address,
        .end = phys_address + page_size
    };
    insert_node(&new_node);
}


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
#include <mm/mm.h>
#include <lib/memcpy.h>
#include <lib/printf.h>
#include <lib/assert.h>
#include <stdint.h>


static uintptr_t first_node_addr;


struct node
{
    uintptr_t begin;
    uintptr_t end;
    uintptr_t next;
};


static inline void read_node(struct node *node, uintptr_t phys_address)
{
    const void *addr = (void *)mm_va(phys_address);
    memcpy(node, addr, sizeof(struct node));
    ASSERT_MSG(node->begin == phys_address, "corrupted memory node");
}


static inline void write_node(const struct node *node)
{
    void *addr = (void *)mm_va(node->begin);
    memcpy(addr, node, sizeof(struct node));
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

    struct node current = {0};
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


void init_phys_allocator(const struct address_range *regions, int num_regions)
{
    for (int i = 0; i < num_regions; ++i)
    {
        uintptr_t beg = regions[i].begin;
        uintptr_t end = regions[i].end;

        ASSERT(!(beg & 0xFFF));
        ASSERT(!((end - 1) & 0xFFF));

        if (beg >= end)
        {
            continue;
        }
        struct node node = {.begin = beg, .end = end};
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
        if (first_node.end - first_node.begin == PAGE_SIZE)
        {
            first_node_addr = first_node.next;
        }
        else
        {
            first_node.begin += PAGE_SIZE;
            write_node(&first_node);
            first_node_addr = first_node.begin;
        }
    }
    return res;
}


void free_phys_page(uintptr_t phys_address)
{
    ASSERT_MSG(!(phys_address & 0xFFF), "address needs to be page-aligned");
    struct node new_node = {.begin = phys_address,
                            .end = phys_address + PAGE_SIZE};
    insert_node(&new_node);
}


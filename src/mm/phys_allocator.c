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


struct node
{
    void *begin;
    void *end;
    struct node *next;
};


static struct node *first_node;


static void try_merge_with_next(struct node *node)
{
    if (node->end == node->next)
    {
        struct node *next_node = node->next;
        node->next = next_node->next;
        node->end = next_node->end;
    }
}


static inline void insert_node(struct node *node)
{
    if (first_node == NULL || node->begin < (void *)first_node)
    {
        node->next = first_node;
        try_merge_with_next(node);
        first_node = node;
        return;
    }

    struct node *current = first_node;
    while (current->next != NULL)
    {
        struct node *next_node = current->next;
        if (node->begin < next_node->begin)
        {
            break;
        }
        current = next_node;
    }

    node->next = current->next;
    current->next = (struct node *)node->begin;
    try_merge_with_next(node);
    try_merge_with_next(current);
}


void print_phys_mem(void)
{
    const struct node *node = first_node;
    while (node)
    {
        printf("Node [0x%lx, 0x%lx, 0x%lx)\n", (uintptr_t)node->begin, (uintptr_t)node->end, (uintptr_t)node->next);
        node = node->next;
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
        struct node *node = (struct node *)mm_va(beg);
        node->begin = (void *)mm_va(beg);
        node->end = (void *)mm_va(end);
        node->next = NULL;
        insert_node(node);
    }
}


void *alloc_pages(size_t num_pages)
{
    void *res = NULL;
    if (first_node)
    {
        res = first_node;
        if (first_node->end - first_node->begin == PAGE_SIZE)
        {
            first_node = first_node->next;
        }
        else
        {
            void *new_begin = first_node->begin + PAGE_SIZE;
            void *new_end = first_node->end;
            struct node *new_next = first_node->next;
            first_node = (struct node *)new_begin;
            first_node->begin = new_begin;
            first_node->end = new_end;
            first_node->next = new_next;
        }
    }
    return res;
}


void free_pages(void *virt_address, size_t num_pages)
{
    struct node *new_node = (struct node *)virt_address;
    new_node->begin = virt_address;
    new_node->end = virt_address + PAGE_SIZE;
    new_node->next = NULL;
    insert_node(new_node);
}


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
    struct node *next;
    size_t num_pages;
};


static inline const void *node_end(const struct node *node)
{
    return (const void *)node + node->num_pages * PAGE_SIZE;
}


static inline void try_merge_with_next(struct node *node)
{
    if (node->next && node_end(node) == node->next)
    {
        node->num_pages += node->next->num_pages;
        node->next = node->next->next;
    }
}


static struct node *first_node;


static inline void insert_node(struct node *node)
{
    if (first_node == NULL || node < first_node)
    {
        node->next = first_node;
        try_merge_with_next(node);
        first_node = node;
        return;
    }

    struct node *current = first_node;
    while (current->next != NULL)
    {
        if (node < current->next)
        {
            break;
        }
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    try_merge_with_next(node);
    try_merge_with_next(current);
}


void print_phys_mem(void)
{
    const struct node *node = first_node;
    while (node)
    {
        printf("Node [0x%lx, 0x%lx), 0x%lx\n", (uintptr_t)node,
               (uintptr_t)(node_end(node)), (uintptr_t)node->next);
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
        node->num_pages = (end - beg) / PAGE_SIZE;
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
        if (first_node->num_pages == 1)
        {
            first_node = first_node->next;
        }
        else
        {
            void *new_begin = (void *)first_node + PAGE_SIZE;
            size_t new_num_pages = first_node->num_pages - 1;
            struct node *new_next = first_node->next;
            first_node = (struct node *)new_begin;
            first_node->num_pages = new_num_pages;
            first_node->next = new_next;
        }
    }
    return res;
}


void free_pages(void *virt_address, size_t num_pages)
{
    struct node *new_node = (struct node *)virt_address;
    new_node->num_pages = num_pages;
    new_node->next = NULL;
    insert_node(new_node);
}


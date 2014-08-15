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
#include <lib/memset.h>
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


struct page *first_page_struct;


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
        printf("Node [0x%p, 0x%p), 0x%p\n", node, node_end(node), node->next);
        node = node->next;
    }
}


static void *alloc_pages_impl(size_t num_pages)
{
    void *result = NULL;
    struct node *prev = NULL;
    struct node *node = first_node;
    while (node)
    {
        if (node->num_pages >= num_pages)
        {
            result = node;
            size_t new_num_pages = node->num_pages - num_pages;
            struct node *next = node->next;
            if (new_num_pages > 0)
            {
                struct node *new_node =
                    (struct node *)((void *)node + num_pages * PAGE_SIZE);
                new_node->num_pages = new_num_pages;
                new_node->next = node->next;
                next = new_node;
            }
            if (prev)
            {
                prev->next = next;
            }
            else
            {
                first_node = next;
            }
            break;
        }
        prev = node;
        node = node->next;
    }
    return result;
}


static void init_page_structs(size_t num_phys_pages)
{
    size_t pages_needed = num_phys_pages * sizeof(struct page) / PAGE_SIZE + 1;
    first_page_struct = (struct page *)alloc_pages_impl(pages_needed);
    ASSERT_MSG(first_page_struct, "failed to allocate page structs");
    memset(first_page_struct, 0, num_phys_pages * sizeof(struct page));
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
    const struct address_range *last_region = regions + num_regions - 1;
    size_t num_phys_pages = last_region->end / PAGE_SIZE;
    init_page_structs(num_phys_pages);
}


struct page *alloc_pages(size_t num_pages)
{
    void *res = alloc_pages_impl(num_pages);
    return mm_page_from_virt((uintptr_t)res);
}


static void free_pages_impl(void *virt_address, size_t num_pages)
{
    struct node *node = (struct node *)virt_address;
    node->num_pages = num_pages;
    node->next = NULL;
    insert_node(node);
}


void free_pages(struct page *first_page, size_t num_pages)
{
    void *virt_address = (void *)mm_page_to_virt(first_page);
    free_pages_impl(virt_address, num_pages);
}


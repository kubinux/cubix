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

#ifndef PAGE_H_INCLUDED_EETA6YDN
#define PAGE_H_INCLUDED_EETA6YDN

#include <lib/list.h>
#include <stdint.h>


struct kmem_cache;


#define PG_HEAD (1 << 1)
#define PG_CONT (1 << 2)


struct page
{
    uint64_t flags;
    struct list_node list;
    union
    {
        // head page
        struct
        {
            struct kmem_cache *slab_cache;
            void *free_objects;
            size_t num_allocated;
        };

        // continuation page
        struct page *head_page;
    };
};


#endif // include guard


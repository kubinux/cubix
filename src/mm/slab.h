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

#ifndef SLAB_H_INCLUDED_LAFCXXUQ
#define SLAB_H_INCLUDED_LAFCXXUQ

#include <stddef.h>
#include <stdint.h>


struct kmem_cache;


void kmem_cache_init(void);


struct kmem_cache *kmem_cache_create(const char *name, size_t size,
                                     size_t alignment);


void kmem_cache_destroy(struct kmem_cache *cache);


void *kmem_cache_alloc(struct kmem_cache *cache, uint32_t flags);


void kmem_cache_free(struct kmem_cache *cache, void *object);


void kmem_cache_print_info(struct kmem_cache *cache);


#endif // include guard


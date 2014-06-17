#ifndef PHYS_ALLOCATOR_H_INCLUDED_VACHCL58
#define PHYS_ALLOCATOR_H_INCLUDED_VACHCL58

#include <mm/mmap_region.h>
#include <stdint.h>


uintptr_t alloc_phys_page_early(void);


void init_phys_allocator(const struct mmap_region *regions, int num_regions);


void print_phys_mem(void);


uintptr_t alloc_phys_page(void);


void free_phys_page(uintptr_t phys_address);


#endif // include guard


#ifndef PAGING_H_INCLUDED_LGNA9BIS
#define PAGING_H_INCLUDED_LGNA9BIS

#include <stdint.h>
#include <stddef.h>


typedef uint64_t mmap_entry_t[512] __attribute__((aligned(0x1000)));


void init_kernel_pages(void);


void invalidate_page(uintptr_t virt_address);


void remap_kernel_page(uintptr_t virt_address, uintptr_t phys_address);


#endif // include guard


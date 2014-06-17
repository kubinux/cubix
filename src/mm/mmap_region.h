#ifndef MMAP_REGION_H_INCLUDED_DNSV8CM7
#define MMAP_REGION_H_INCLUDED_DNSV8CM7

#include <stdint.h>

struct mmap_region
{
    uintptr_t addr;
    uintmax_t size;
    int usable;
};

#endif // include guard


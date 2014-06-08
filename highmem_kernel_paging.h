#ifndef HIGHMEM_KERNEL_PAGING_H_INCLUDED_RLVHGLHP
#define HIGHMEM_KERNEL_PAGING_H_INCLUDED_RLVHGLHP

void init_highmem_kernel_paging(void)
    __attribute__((section("low-memory-text")));

#endif // include guard


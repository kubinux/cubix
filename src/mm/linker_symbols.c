#ifndef LINKER_SYMBOLS_C_INCLUDED_NVSSJWRR
#define LINKER_SYMBOLS_C_INCLUDED_NVSSJWRR

#include <mm/linker_symbols.h>


extern char _kernel_virt_offset[];

extern char _kernel_phys_start[];

extern char _kernel_virt_start[];

extern char _kernel_phys_end[]; 

extern char _kernel_virt_end[];

const uintptr_t kernel_virt_offset = (uintptr_t)&_kernel_virt_offset;

const uintptr_t kernel_phys_start = (uintptr_t)&_kernel_phys_start;

const uintptr_t kernel_virt_start = (uintptr_t)&_kernel_virt_start;

const uintptr_t kernel_phys_end = (uintptr_t)&_kernel_phys_end; 

const uintptr_t kernel_virt_end = (uintptr_t)&_kernel_virt_end;


#endif // include guard


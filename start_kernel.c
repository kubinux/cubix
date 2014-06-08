#include "kernel.h"
#include "highmem_kernel_paging.h"

static void hang(void) __attribute__((noreturn));

static void hang(void)
{
    while (1)
    {
    }
}

static void invoke_global_ctors(void)
{
    extern void _init(void);
    _init();
}

void start_kernel(void) __attribute__((noreturn));

void start_kernel(void)
{
    init_highmem_kernel_paging();
    invoke_global_ctors();
    kernel_main();
    hang();
}


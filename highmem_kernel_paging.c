#include "highmem_kernel_paging.h"
#include <stddef.h>
#include <stdint.h>

// see linker.ld for definitions of external symbols

static void *get_low_mem_start(void)
{
    extern int LOW_MEM_START;
    return (void *)&LOW_MEM_START;
}

static void *get_low_mem_end(void)
{
    extern int LOW_MEM_END;
    return (void *)&LOW_MEM_END;
}

static void *get_high_mem_start(void)
{
    extern int HIGH_MEM_START;
    return (void *)&HIGH_MEM_START;
}

static void *get_high_mem_end(void)
{
    extern int HIGH_MEM_END;
    return (void *)&HIGH_MEM_END;
}

static size_t get_low_mem_size(void)
{
    return (size_t)get_low_mem_end() - (size_t)get_low_mem_start();
}

static size_t get_high_mem_size(void)
{
    return (size_t)get_high_mem_end() - (size_t)get_high_mem_size();
}

static uint32_t page_table[1024] __attribute__((aligned(4096)));

static uint32_t page_directory[1024] __attribute__((aligned(4096)));

static void init_page_table(void)
{
    uint32_t phys_address = 0x0;
    // P (Present) = 1 (page present)             -->   1
    // R (Read/Write) = 1 (read + write)          -->  1
    // U (User/Supervisor) = 0 (only supervisor)  --> 0
    // --------------------------------------------------
    //                                                011b = 3
    uint32_t flags = 3;
    for (size_t i = 0; i < sizeof(page_table)/sizeof(*page_table); ++i)
    {
        page_table[i] = phys_address | flags;
        phys_address += 4096;
    }
}

static void init_page_directory(void)
{
    page_directory[0] = (uint32_t)&page_table;
    page_directory[0] |= 1; // (present)
    for (size_t i = 1; i < sizeof(page_directory)/sizeof(*page_directory); ++i)
    {
        // P (Present) = 0 (page not present)         -->   0
        // R (Read/Write) = 1 (read + write)          -->  1
        // U (User/Supervisor) = 0 (only supervisor)  --> 0
        // --------------------------------------------------
        //                                                010b = 2
        page_directory[i] = 2;
    }
}

static void install_page_directory(void)
{
    __asm__ __volatile__("movl %0, %%cr3": : "b"(&page_directory));
}

static void enable_paging(void)
{
    uint32_t cr0;
    __asm__ __volatile__("movl %%cr0, %0": "=b"(cr0));
    cr0 |= (1 << 31);
    __asm__ __volatile__("movl %0, %%cr0": : "b"(cr0));
}

void init_highmem_kernel_paging(void)
{
    init_page_table();
    init_page_directory();
    install_page_directory();
    enable_paging();
}


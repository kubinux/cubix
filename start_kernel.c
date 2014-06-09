#include "kernel.h"
#include <stddef.h>
#include <stdint.h>

static uint32_t get_kernel_virtual_mem_offset(void)
{
    // see linker.ld for definitions of external symbols
    extern char HIGH_MEM_START[];
    extern char LOW_MEM_END[];
    return (uint32_t)HIGH_MEM_START - (uint32_t)LOW_MEM_END;
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
    for (size_t i = 0; i < sizeof(page_table) / sizeof(*page_table); ++i)
    {
        page_table[i] = phys_address | flags;
        phys_address += 4096;
    }
}

static void init_page_directory(void)
{
    for (size_t i = 0; i < sizeof(page_directory) / sizeof(*page_directory);
         ++i)
    {
        // P (Present) = 0 (page not present)         -->   0
        // R (Read/Write) = 1 (read + write)          -->  1
        // U (User/Supervisor) = 0 (only supervisor)  --> 0
        // --------------------------------------------------
        //                                                010b = 2
        page_directory[i] = 2;
    }
    page_directory[0] = (uint32_t)&page_table;
    page_directory[0] |= 1; // (present)
    uint32_t high_mem_idx = get_kernel_virtual_mem_offset();
    high_mem_idx = high_mem_idx >> 22;
    page_directory[high_mem_idx] = (uint32_t)&page_table;
    page_directory[high_mem_idx] |= 1; // (present)
}

static void install_page_directory(void)
{
    __asm__ __volatile__("movl %0, %%cr3" : : "r"(&page_directory));
}

static void enable_paging(void)
{
    uint32_t cr0;
    __asm__ __volatile__("movl %%cr0, %0" : "=r"(cr0));
    cr0 |= (1 << 31);
    __asm__ __volatile__("movl %0, %%cr0" : : "r"(cr0));
}

static void init_highmem_kernel_paging(void)
{
    init_page_table();
    init_page_directory();
    install_page_directory();
    enable_paging();
}

static void invoke_global_ctors(void)
{
    extern void _init(void);
    _init();
}

void start_kernel(uint32_t magic, uint32_t boot_info_addr)
{
    init_highmem_kernel_paging();
    invoke_global_ctors();
    kernel_main(magic, boot_info_addr);
}


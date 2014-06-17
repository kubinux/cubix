#include <mm/paging.h>
#include <mm/linker_symbols.h>
#include <mm/phys_allocator.h>
#include <lib/printf.h>
#include <lib/memset.h>
#include <lib/assert.h>
#include <stdint.h>


static mmap_entry_t pml4;
static mmap_entry_t pdp;
static mmap_entry_t pd;


static inline size_t get_pt_index(uintptr_t virt_address)
{
    return (virt_address >> 12) & 0x1FF;
}


static inline size_t get_pd_index(uintptr_t virt_address)
{
    return (virt_address >> 21) & 0x1FF;
}


static inline size_t get_pdp_index(uintptr_t virt_address)
{
    return (virt_address >> 30) & 0x1FF;
}


static inline size_t get_pml4_index(uintptr_t virt_address)
{
    return (virt_address >> 39) & 0x1FF;
}


static inline uint64_t *get_pt(size_t pml4i, size_t pdpi, size_t pdi)
{
    return (uint64_t *)(0xFFFFFF0000000000 + 0x40000000 * pml4i +
                        0x200000 * pdpi + 0x1000 * pdi);
}


static inline uint64_t *get_pd(size_t pml4i, size_t pdpi)
{
    return (uint64_t *)(0xFFFFFF7FC0000000 + 0x200000 * pml4i + 0x1000 * pdpi);
}


static inline uint64_t *get_pdp(size_t pml4i)
{
    return (uint64_t *)(0xFFFFFF7FFFE00000 + 0x1000 * pml4i);
}


static inline uint64_t *get_pml4(void)
{
    return (uint64_t *)(0xFFFFFF7FFFFFF000);
}


void init_kernel_pages(void)
{
    int num_pts = (kernel_phys_end / 0x200000) + 1;
    uintptr_t phys_addr = 0;
    for (int pd_index = 0; pd_index < num_pts; ++pd_index)
    {
        uintptr_t pt_phys_addr = alloc_phys_page_early();
        uint64_t *pt = (uint64_t *)(pt_phys_addr + kernel_virt_offset);
        memset(pt, 0, 0x1000);
        for (int i = 0; i < 512 && phys_addr < kernel_phys_end;
             ++i, phys_addr += 0x1000)
        {
            pt[i] = phys_addr | 3;
        }
        pd[pd_index] = pt_phys_addr | 3;
    }
    pdp[510] = ((uintptr_t)pd - kernel_virt_offset) | 3;
    pml4[511] = ((uintptr_t)pdp - kernel_virt_offset) | 3;
    uintptr_t pml4_phys_addr = (uintptr_t)pml4 - kernel_virt_offset;
    pml4[510] = pml4_phys_addr | 3;
    __asm__ __volatile__("movq %0, %%cr3" ::"r"(pml4_phys_addr) : "memory");
}


void invalidate_page(uintptr_t virt_address)
{
    __asm__ __volatile__("invlpg (%0)" ::"r"(virt_address) : "memory");
}


void remap_kernel_page(uintptr_t virt_address, uintptr_t phys_address)
{
    ASSERT_MSG(virt_address < kernel_virt_end, "page outside kernel image");

    size_t pml4i = get_pml4_index(virt_address);
    size_t pdpi = get_pdp_index(virt_address);
    size_t pdi = get_pd_index(virt_address);
    size_t pti = get_pt_index(virt_address);

    uint64_t *pt = get_pt(pml4i, pdpi, pdi);
    pt[pti] = phys_address | 3;
    invalidate_page(virt_address);
}


#include "kernel.h"
#include "multiboot.h"
#include "vga.h"
#include "printk.h"
#include <stddef.h>
#include <stdint.h>

int foo;

__attribute__((constructor)) void init_foo(void)
{
    foo = 123;
}

void kernel_main(uint32_t magic, uint32_t boot_info_addr)
{
    vga_clear();
    printk("Hello %s!\n", "Kuba");
    printk("Boot magic: 0x%x\n", magic);

    multiboot_info_t* mbi = (multiboot_info_t*)boot_info_addr;
    printk("flags = 0x%x\n", (mbi->flags));
    printk("mem_lower = %uKB, mem_upper = %uKB\n",
           mbi->mem_lower,
           mbi->mem_upper);
    printk("boot_device = 0x%x\n", mbi->boot_device);
    {
        multiboot_memory_map_t* mmap;

        printk("mmap_addr = 0x%x, mmap_length = 0x%x\n",
               (unsigned)mbi->mmap_addr,
               (unsigned)mbi->mmap_length);
        for (mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
             (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
             mmap = (multiboot_memory_map_t*)((unsigned long)mmap +
                                              mmap->size + sizeof(mmap->size)))
/*            printk(" size = 0x%x, base_addr = 0x%x%x,"
                   " length = 0x%x%x, type = 0x%x\n",
                   (unsigned)mmap->size,
                   (unsigned)(mmap->addr >> 32),
                   (unsigned)(mmap->addr & 0xffffffff),
                   (unsigned)(mmap->len >> 32),
                   (unsigned)(mmap->len & 0xffffffff),
                   (unsigned)(mmap->type)); */
            printk(" size = %u, base_addr = %u,"
                   " length = %u, type = %u\n",
                   (unsigned)mmap->size,
                   (unsigned)(mmap->addr),
                   (unsigned)(mmap->len),
                   (unsigned)(mmap->type));
    }
}


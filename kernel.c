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
}


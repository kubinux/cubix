#include "kernel.h"
#include "vga.h"
#include "printk.h"
#include <stddef.h>
#include <stdint.h>

int foo;

void init_foo(void) __attribute__((constructor));

void init_foo(void)
{
    foo = 123;
}

void kernel_main(void)
{
    vga_clear();
    printk("Hello %s!\n", "Kuba");
    if (foo == 123)
    {
        printk("Constructors run!\n");
    }
    else
    {
        printk("Constructors did not run\n");
    }
}


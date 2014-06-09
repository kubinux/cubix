OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

KERNEL_LOADED_AT = 0x00100000;
KERNEL_VIRTUAL_MEM_OFFSET = 0xc0000000;

SECTIONS
{
    . = KERNEL_LOADED_AT;

    LOW_MEM_START = .;
    .low-memory : ALIGN(4K)
    {
        multiboot.o(.text .data .rodata .bss)
        boot.o(.text .data .rodata .bss .bootstrap_stack)
        start_kernel.o(.text .data .rodata .bss)
    }
    LOW_MEM_END = .;

    . += KERNEL_VIRTUAL_MEM_OFFSET;

    HIGH_MEM_START = .;
    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL_MEM_OFFSET)
    {
        *(.text)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL_MEM_OFFSET)
    {
        *(.data)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_MEM_OFFSET)
    {
        *(.rodata)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL_MEM_OFFSET)
    {
        *(COMMON)
        *(.bss)
    }
    HIGH_MEM_END = .;
}

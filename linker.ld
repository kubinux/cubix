OUTPUT_FORMAT(elf32-i386)
ENTRY(start_kernel)

KERNEL_LOADED_AT = 0x00100000;
KERNEL_VIRTUAL_MEM_OFFSET = 0xc0000000;

SECTIONS
{
    . = KERNEL_LOADED_AT;

    LOW_MEM_START = .;
    .low-memory : ALIGN(4K)
    {
        *(multiboot)
    }
    LOW_MEM_END = .;

    HIGH_MEM_START = .;
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
        *(.early_paging)
    }
    HIGH_MEM_END = .;
}

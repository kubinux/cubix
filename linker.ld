ENTRY(_start)

_kernel_virt_offset = 0xFFFFFFFF80000000;
_kernel_phys_start = 0x00100000;
_kernel_virt_start = _kernel_phys_start + _kernel_virt_offset;

SECTIONS
{
    . = _kernel_phys_start;

    .low-memory ALIGN(4K):
    {
        src/startup/multiboot.o(*)
        src/startup/startup.o(*)
        src/startup/enter_long_mode.o(*)
    }

    . += _kernel_virt_offset;

    .text ALIGN(0x1000) : AT(ADDR(.text) - _kernel_virt_offset)
    {
        *(.text)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - _kernel_virt_offset)
    {
        *(.rodata)
    }

    .data ALIGN(0x1000) : AT(ADDR(.data) - _kernel_virt_offset)
    {
        *(.data)
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - _kernel_virt_offset)
    {
        *(COMMON)
        *(.bss)
    }

    _kernel_virt_end = .;
    _kernel_phys_end = _kernel_virt_end - _kernel_virt_offset;

    /DISCARD/ :
    {
        *(.comment)
    }

}

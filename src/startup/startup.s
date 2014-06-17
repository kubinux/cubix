.code32


.section .text
.extern enter_long_mode
.extern main
.global _start
.type _start, @function
_start:
    movl %eax, (multiboot_magic)
    movl %ebx, (multiboot_info)
    movl $stack_top, %esp
    call enter_long_mode
    movl $gdt_descriptor, %eax
    lgdt (%eax)
    jmpl $0x8, $long_mode


.code64
long_mode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    movq $stack_top, %rsp
    addq $_kernel_virt_offset, %rsp

    movq $gdt_descriptor, %rax
    addq $_kernel_virt_offset, 2(%rax)
    lgdt (%rax)

    xorq %rdi, %rdi
    movl (multiboot_magic), %edi
    addq $_kernel_virt_offset, %rdi
    xorq %rsi, %rsi
    movl (multiboot_info), %esi
    addq $_kernel_virt_offset, %rsi
    call main

    cli
    hlt
    jmp .


.section .bootstrap_stack, "aw", @nobits


stack_bottom:
.skip 0x4000
stack_top:


.section .bss


.lcomm multiboot_magic, 4
.lcomm multiboot_info, 4


.section .data


.align 4
gdt:
    .quad 0x0000000000000000
    .quad 0x00A09A0000000000
    .quad 0x00A0920000000000
gdt_descriptor:
.length:
    .word . - gdt - 1
.address:
    .quad gdt


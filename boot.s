.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1badb002
.set CHECKSUM,  -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp
    call _init
    call kernel_main
    # hang just in case kernel_main returns
    cli
    hlt
.hang:
    jmp .hang

.size _start, . - _start


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
.skip 0x4000 # 16 KB
stack_top:

.section .early_paging
.align 0x1000
early_page_dir:
.skip 0x1000 # 4 KB
early_kernel_page_tbl:
.skip 0x1000
early_low_page_tbl:
.skip 0x1000

.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp
    call init_early_paging
    call _init
    call kernel_main
    # hang just in case kernel_main returns
    cli
    hlt
.hang:
    jmp .hang

.size _start, . - _start

.type init_early_paging, @function
init_early_paging:
    pusha

    popa
    ret


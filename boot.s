.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 0x4000
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp
    pushl %ebx
    pushl %eax
    call start_kernel
    cli
    hlt
.hang:
    jmp .hang
.size _start, . - _start


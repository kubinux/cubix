# Copyright (C) 2014 Jakub Lewandowski <jakub.lewandowski@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


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


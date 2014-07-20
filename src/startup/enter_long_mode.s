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


.global enter_long_mode
.type enter_long_mode, @function
enter_long_mode:
    pusha
    call setup_page_map
    call enable_pae
    call enable_long_mode
    call enable_paging
    popa
    ret


.local setup_page_map
.type setup_page_map, @function
setup_page_map:
    pusha
    call init_pd
    call init_pdp
    call init_pml4
    movl $pml4, %eax
    movl %eax, %cr3
    popa
    ret


.local enable_pae
.type enable_pae, @function
enable_pae:
    pusha
    movl %cr4, %eax
    btsl $5, %eax
    movl %eax, %cr4
    popa
    ret


.local enable_long_mode
.type enable_long_mode, @function
enable_long_mode:
    pusha
    movl $0xc0000080, %ecx
    rdmsr
    btsl $8, %eax
    wrmsr
    popa
    ret


.local enable_paging
.type enable_paging, @function
enable_paging:
    pusha
    movl %cr0, %eax
    btsl $31, %eax
    movl %eax, %cr0
    popa
    ret


.code32
.local init_pd
.type init_pd, @function
init_pd:
    pusha
    xorl %eax, %eax  # current physical address
    xorl %ebx, %ebx  # PDE flags:
    btsl $0, %ebx    #   present   (P)
    btsl $1, %ebx    #   writable  (R/W)
    btsl $7, %ebx    #   2MB pages (PS)
    xorl %edi, %edi  # index of the current PDE
    movl $pd, %edx   # address of the PD
.loop:
    cmpl $512, %edi
    jge .loop_end
    movl %eax, %ecx
    orl  %ebx, %ecx
    movl %ecx, (%edx, %edi, 8)
    incl %edi
    addl $0x200000, %eax
    jmp .loop
.loop_end:
    popa
    ret


.code32
.local init_pdp
.type init_pdp, @function
init_pdp:
    pusha
    movl $pdp_low, %edx
    movl $pd, %ecx
    xorl %ebx, %ebx  # PDPE flags:
    btsl $0, %ebx    #   present   (P)
    btsl $1, %ebx    #   writable  (R/W)
    orl  %ebx, %ecx
    movl $0, %edi
    movl %ecx, (%edx, %edi, 8)
    movl $pdp_high, %edx
    movl $510, %edi
    movl %ecx, (%edx, %edi, 8)
    popa
    ret


.code32
.local init_pml4
.type init_pml4, @function
init_pml4:
    pusha
    movl $pml4, %edx
    movl $pdp_low, %ecx
    xorl %ebx, %ebx  # PML4E flags:
    btsl $0, %ebx    #   present   (P)
    btsl $1, %ebx    #   writable  (R/W)
    orl  %ebx, %ecx
    movl $0, %edi
    movl %ecx, (%edx, %edi, 8)
    movl $pdp_high, %ecx
    orl $3, %ecx
    movl $511, %edi
    movl %ecx, (%edx, %edi, 8)
    popa
    ret


.section .bss


.local pml4
.comm pml4, 4096, 4096


.local pdp_low
.comm pdp_low, 4096, 4096


.local pdp_high
.comm pdp_high, 4096, 4096


.local pd
.comm pd, 4096, 4096


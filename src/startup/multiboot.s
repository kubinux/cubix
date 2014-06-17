.code32


.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1badb002
.set CHECKSUM,  -(MAGIC + FLAGS)


.section .data
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


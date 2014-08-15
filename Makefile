TARGET := x86_64-elf
CC     := $(TARGET)-gcc
CXX    := $(TARGET)-g++
AS     := $(TARGET)-as


WARNING_FLAGS := -Wall -Wextra


CFLAGS := -std=gnu11         \
          -mno-red-zone      \
          -mno-mmx           \
          -mno-sse           \
          -mno-sse2          \
          -mno-sse3          \
          -mno-3dnow         \
          -ffreestanding     \
          -mcmodel=kernel    \
          -g                 \
          $(WARNING_FLAGS)


CPPFLAGS := -I src


LDFLAGS := -T linker.ld            \
           -ffreestanding          \
           -z max-page-size=0x1000 \
           -nostdlib               \
           -g


OBJS := src/startup/startup.o         \
        src/startup/multiboot.o       \
        src/startup/enter_long_mode.o \
        src/startup/kernel_main.o     \
        src/lib/memcpy.o              \
        src/lib/memset.o              \
        src/lib/printf.o              \
        src/lib/string.o              \
        src/lib/assert.o              \
        src/lib/list.o                \
        src/mm/paging.o               \
        src/mm/linker_symbols.o       \
        src/mm/phys_allocator.o       \
        src/mm/slab.o                 \
        src/mm/mm.o                   \
        src/io/vga.o                  \
        src/io/port_io.o              \


cubix.iso: cubix.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp cubix.bin isodir/boot
	cp grub.cfg isodir/boot/grub 
	grub-mkrescue -o $@ isodir
	rm -rf isodir


cubix.bin: $(OBJS)
	$(CC) $(LDFLAGS) -lgcc -o $@ $^


cubix.bin: linker.ld


isodir/boot/cubix.bin: cubix.bin
	cp $^ $@


.PHONY: run
run: cubix.iso
	qemu-system-x86_64 -monitor stdio -m 2000 -cdrom $<


.PHONY: run-dbg
run-dbg: cubix.iso
	qemu-system-x86_64 -s -cdrom $< &
	sleep 5
	gdb cubix.bin -ex 'break main' -ex 'target remote localhost:1234'


.PHONY: clean
clean:
	find -name '*.o' -delete


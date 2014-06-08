TARGET := i686-elf
CC     := $(TARGET)-gcc
CXX    := $(TARGET)-g++
AS     := $(TARGET)-as



WARNING_FLAGS := -Wall -Wextra


CFLAGS := -std=c11           \
          -ffreestanding     \
          $(WARNING_FLAGS)


CXXFLAGS := -std=c++11       \
            -ffreestanding   \
            -fno-exceptions  \
            -fno-rtti        \
            $(WARNING_FLAGS)


OBJS := highmem_kernel_paging.o \
        kernel.o                \
        vga.o                   \
        port_io.o               \
        memcpy.o                \
        printk.o                \
        cxa_guard.o             \
        multiboot.o             \
        start_kernel.o


CRTI_OBJ      := crti.o
CRTBEGIN_OBJ  := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ    := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_OBJ      := crtn.o
KERNEL_OBJS   := $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)


cubix.iso: cubix.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp cubix.bin isodir/boot
	cp grub.cfg isodir/boot/grub 
	grub-mkrescue -o $@ isodir
	rm -rf isodir


cubix.bin: $(KERNEL_OBJS) linker.ld
	$(CXX) -T linker.ld -ffreestanding -nostdlib -lgcc -o $@ $(KERNEL_OBJS)


isodir/boot/cubix.bin: cubix.bin
	cp $< $@


.PHONY: run
run: cubix.iso
	qemu-system-i386 -cdrom cubix.iso


.PHONT: clean
clean:
	rm -f *.o

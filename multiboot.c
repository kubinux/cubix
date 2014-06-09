#include "multiboot.h"

#define FLAGS (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)

static struct multiboot_header header
    __attribute__((aligned(MULTIBOOT_HEADER_ALIGN))) = {
        .magic = MULTIBOOT_HEADER_MAGIC,
        .flags = FLAGS,
        .checksum = (multiboot_uint32_t)(-MULTIBOOT_HEADER_MAGIC - FLAGS)};


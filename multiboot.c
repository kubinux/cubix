struct
{
    int magic;
    int flags;
    int checksum;
} __attribute__((__packed__)) multiboot_header
    __attribute__((aligned(4), section("multiboot")))
    = { 0x1badb002, ((1<<0) | (1<<1)), -(0x1badb002 + ((1<<0) | (1<<1))) };

/* LightOS Kernel Linker Script */

ENTRY(_start)

SECTIONS {
    /* The kernel will be loaded at 1MB */
    . = 0x100000;

    /* First put the multiboot header, as it's required to be put very early
       in the image or the bootloader won't recognize the file format.
       Next we'll put the .text section. */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
}

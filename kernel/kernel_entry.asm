; LightOS Kernel Entry Point
; This is the entry point for our kernel, called by the bootloader

[BITS 32]           ; We're in 32-bit protected mode

[GLOBAL _start]     ; Make the entry point visible to the linker

_start:
    ; Call the C kernel main function
    [EXTERN kernel_main]
    call kernel_main
    
    ; If kernel_main returns, hang the CPU
    jmp $

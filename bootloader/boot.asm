; LightOS Bootloader
; A simple bootloader for our custom operating system

[BITS 16]       ; We're working in 16-bit mode initially
[ORG 0x7C00]    ; BIOS loads the bootloader at this memory address

; Constants
KERNEL_OFFSET equ 0x1000   ; Memory offset where we'll load our kernel

; Boot entry point
start:
    ; Set up segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00     ; Set up stack pointer just below where we're loaded

    ; Display boot message
    mov si, MSG_BOOT
    call print_string

    ; Load kernel from disk
    call load_kernel

    ; Switch to protected mode
    call switch_to_pm

    ; We should never reach here
    jmp $

; Function to print a string
; Input: SI points to null-terminated string
print_string:
    push ax
    push bx
    mov ah, 0x0E       ; BIOS teletype function
    mov bh, 0          ; Page number
    mov bl, 0x07       ; Text attribute (light gray on black)

.loop:
    lodsb              ; Load byte from SI into AL and increment SI
    or al, al          ; Check if we've reached the end of the string
    jz .done           ; If AL is zero, we're done
    int 0x10           ; Call BIOS interrupt to print character
    jmp .loop          ; Repeat for next character

.done:
    pop bx
    pop ax
    ret

; Function to load the kernel from disk
load_kernel:
    mov si, MSG_LOAD_KERNEL
    call print_string

    ; Set up disk read
    mov ah, 0x02       ; BIOS read sector function
    mov al, 15         ; Number of sectors to read
    mov ch, 0          ; Cylinder number
    mov cl, 2          ; Sector number (1-based, sector 1 is the boot sector)
    mov dh, 0          ; Head number
    mov dl, [BOOT_DRIVE] ; Drive number

    ; Set up memory location to read to
    mov bx, KERNEL_OFFSET

    ; Perform the read
    int 0x13

    ; Check for errors
    jc disk_error      ; Jump if carry flag is set (error occurred)

    ; Compare number of sectors read
    cmp al, 15
    jne disk_error     ; Jump if not all sectors were read

    ret

; Handle disk read errors
disk_error:
    mov si, MSG_DISK_ERROR
    call print_string
    jmp $              ; Hang the system

; Function to switch to 32-bit protected mode
switch_to_pm:
    cli                ; Disable interrupts
    
    ; Load GDT
    lgdt [gdt_descriptor]
    
    ; Set PE bit in CR0 to enter protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    ; Far jump to 32-bit code
    jmp CODE_SEG:init_pm

[BITS 32]
; Initialize protected mode
init_pm:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Set up stack
    mov ebp, 0x90000
    mov esp, ebp
    
    ; Jump to the kernel
    call KERNEL_OFFSET
    
    ; We should never reach here
    jmp $

; Global Descriptor Table (GDT)
gdt_start:

; Null descriptor (required)
gdt_null:
    dd 0x0
    dd 0x0

; Code segment descriptor
gdt_code:
    dw 0xFFFF          ; Limit (bits 0-15)
    dw 0x0             ; Base (bits 0-15)
    db 0x0             ; Base (bits 16-23)
    db 10011010b       ; Access byte
    db 11001111b       ; Flags and Limit (bits 16-19)
    db 0x0             ; Base (bits 24-31)

; Data segment descriptor
gdt_data:
    dw 0xFFFF          ; Limit (bits 0-15)
    dw 0x0             ; Base (bits 0-15)
    db 0x0             ; Base (bits 16-23)
    db 10010010b       ; Access byte
    db 11001111b       ; Flags and Limit (bits 16-19)
    db 0x0             ; Base (bits 24-31)

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start               ; Address of GDT

; Define segment selectors
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Data
BOOT_DRIVE db 0
MSG_BOOT db 'LightOS Bootloader starting...', 0x0D, 0x0A, 0
MSG_LOAD_KERNEL db 'Loading kernel into memory...', 0x0D, 0x0A, 0
MSG_DISK_ERROR db 'Error reading disk!', 0x0D, 0x0A, 0

; Padding and boot signature
times 510-($-$$) db 0  ; Pad the rest of the sector with zeros
dw 0xAA55              ; Boot signature

# LightOS Makefile
# Main build system for the LightOS operating system

# Compiler settings
CC = gcc
ASM = nasm
LD = ld

# Directories
BOOTLOADER_DIR = bootloader
KERNEL_DIR = kernel
LIBC_DIR = libc
BUILD_DIR = build
ISO_DIR = $(BUILD_DIR)/iso

# Flags
CFLAGS = -Wall -Wextra -ffreestanding -O2 -nostdlib -nostdinc -fno-builtin
ASMFLAGS = -f elf64
LDFLAGS = -T $(KERNEL_DIR)/linker.ld -nostdlib

# Source files
BOOTLOADER_SRC = $(wildcard $(BOOTLOADER_DIR)/*.asm)
KERNEL_C_SRC = $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_ASM_SRC = $(wildcard $(KERNEL_DIR)/*.asm)
LIBC_SRC = $(wildcard $(LIBC_DIR)/*.c)

# Object files
BOOTLOADER_OBJ = $(BOOTLOADER_SRC:.asm=.o)
KERNEL_C_OBJ = $(KERNEL_C_SRC:.c=.o)
KERNEL_ASM_OBJ = $(KERNEL_ASM_SRC:.asm=.o)
LIBC_OBJ = $(LIBC_SRC:.c=.o)

# Output files
BOOTLOADER_BIN = $(BUILD_DIR)/bootloader.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
ISO_FILE = $(BUILD_DIR)/lightos.iso

# Default target
all: prepare bootloader kernel libc iso

# Prepare build directories
prepare:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(ISO_DIR)/boot/grub

# Bootloader compilation
bootloader: $(BOOTLOADER_BIN)

$(BOOTLOADER_BIN): $(BOOTLOADER_OBJ)
	@echo "Building bootloader..."
	@mkdir -p $(BUILD_DIR)
	@cat $(BOOTLOADER_OBJ) > $(BOOTLOADER_BIN)

%.o: %.asm
	@echo "Assembling $<..."
	@$(ASM) $(ASMFLAGS) $< -o $@

# Kernel compilation
kernel: $(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_C_OBJ) $(KERNEL_ASM_OBJ)
	@echo "Linking kernel..."
	@mkdir -p $(BUILD_DIR)
	@$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(KERNEL_C_OBJ) $(KERNEL_ASM_OBJ)

$(KERNEL_DIR)/%.o: $(KERNEL_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# LibC compilation
libc: $(LIBC_OBJ)

$(LIBC_DIR)/%.o: $(LIBC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Create bootable ISO
iso: $(ISO_FILE)

$(ISO_FILE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo "Creating bootable ISO..."
	@cp $(BOOTLOADER_BIN) $(ISO_DIR)/boot/
	@cp $(KERNEL_BIN) $(ISO_DIR)/boot/
	@echo "set timeout=0" > $(ISO_DIR)/boot/grub/grub.cfg
	@echo "set default=0" >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo "menuentry \"LightOS\" {" >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo "  multiboot /boot/kernel.bin" >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo "  boot" >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo "}" >> $(ISO_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO_FILE) $(ISO_DIR) 2>/dev/null
	@echo "ISO created at $(ISO_FILE)"

# Run in QEMU
run: $(ISO_FILE)
	@echo "Running LightOS in QEMU..."
	@qemu-system-x86_64 -cdrom $(ISO_FILE)

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(BOOTLOADER_DIR)/*.o
	@rm -f $(KERNEL_DIR)/*.o
	@rm -f $(LIBC_DIR)/*.o

.PHONY: all prepare bootloader kernel libc iso run clean

#!/bin/bash
# LightOS Build Script
# This script builds the LightOS operating system

# Exit on error
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print a colored message
print_message() {
    echo -e "${BLUE}[LightOS]${NC} $1"
}

# Print an error message
print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Print a success message
print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# Print a warning message
print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check for required tools
check_dependencies() {
    print_message "Checking dependencies..."
    
    # Check for GCC
    if ! command -v gcc &> /dev/null; then
        print_error "GCC not found. Please install GCC."
        exit 1
    fi
    
    # Check for NASM
    if ! command -v nasm &> /dev/null; then
        print_error "NASM not found. Please install NASM."
        exit 1
    fi
    
    # Check for LD
    if ! command -v ld &> /dev/null; then
        print_error "LD not found. Please install binutils."
        exit 1
    fi
    
    # Check for QEMU
    if ! command -v qemu-system-x86_64 &> /dev/null; then
        print_warning "QEMU not found. You won't be able to run the OS in emulation."
    fi
    
    print_success "All required dependencies found."
}

# Clean the build directory
clean_build() {
    print_message "Cleaning build directory..."
    
    # Remove the build directory if it exists
    if [ -d "build" ]; then
        rm -rf build
    fi
    
    # Create a new build directory
    mkdir -p build/iso/boot/grub
    
    print_success "Build directory cleaned."
}

# Build the bootloader
build_bootloader() {
    print_message "Building bootloader..."
    
    # Assemble the bootloader
    nasm -f bin bootloader/boot.asm -o build/bootloader.bin
    
    print_success "Bootloader built successfully."
}

# Build the kernel
build_kernel() {
    print_message "Building kernel..."
    
    # Assemble the kernel entry point
    nasm -f elf64 kernel/kernel_entry.asm -o build/kernel_entry.o
    
    # Compile the kernel C files
    gcc -c kernel/kernel.c -o build/kernel.o -ffreestanding -O2 -Wall -Wextra
    gcc -c kernel/memory.c -o build/memory.o -ffreestanding -O2 -Wall -Wextra
    gcc -c kernel/process.c -o build/process.o -ffreestanding -O2 -Wall -Wextra
    gcc -c kernel/filesystem.c -o build/filesystem.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the init system
    gcc -c init/init.c -o build/init.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the drivers
    gcc -c drivers/keyboard.c -o build/keyboard.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the networking
    gcc -c networking/network.c -o build/network.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the server
    gcc -c server/server.c -o build/server.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the GUI
    gcc -c gui/gui.c -o build/gui.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the CLI
    gcc -c cli/cli.c -o build/cli.o -ffreestanding -O2 -Wall -Wextra
    
    # Compile the C library
    gcc -c libc/string.c -o build/string.o -ffreestanding -O2 -Wall -Wextra
    
    # Link the kernel
    ld -o build/kernel.bin -T kernel/linker.ld build/kernel_entry.o build/kernel.o build/memory.o build/process.o build/filesystem.o build/init.o build/keyboard.o build/network.o build/server.o build/gui.o build/cli.o build/string.o --oformat binary
    
    print_success "Kernel built successfully."
}

# Create a bootable ISO
create_iso() {
    print_message "Creating bootable ISO..."
    
    # Copy the bootloader and kernel to the ISO directory
    cp build/bootloader.bin build/iso/boot/
    cp build/kernel.bin build/iso/boot/
    
    # Create the GRUB configuration file
    echo "set timeout=0" > build/iso/boot/grub/grub.cfg
    echo "set default=0" >> build/iso/boot/grub/grub.cfg
    echo "menuentry \"LightOS\" {" >> build/iso/boot/grub/grub.cfg
    echo "  multiboot /boot/kernel.bin" >> build/iso/boot/grub/grub.cfg
    echo "  boot" >> build/iso/boot/grub/grub.cfg
    echo "}" >> build/iso/boot/grub/grub.cfg
    
    # Create the ISO
    if command -v grub-mkrescue &> /dev/null; then
        grub-mkrescue -o build/lightos.iso build/iso
    else
        print_error "grub-mkrescue not found. Cannot create ISO."
        exit 1
    fi
    
    print_success "Bootable ISO created successfully."
}

# Run the OS in QEMU
run_os() {
    print_message "Running LightOS in QEMU..."
    
    # Check if QEMU is installed
    if ! command -v qemu-system-x86_64 &> /dev/null; then
        print_error "QEMU not found. Cannot run the OS."
        exit 1
    fi
    
    # Run the OS
    qemu-system-x86_64 -cdrom build/lightos.iso
    
    print_success "QEMU emulation completed."
}

# Main function
main() {
    print_message "Starting LightOS build process..."
    
    # Check dependencies
    check_dependencies
    
    # Clean the build directory
    clean_build
    
    # Build the bootloader
    build_bootloader
    
    # Build the kernel
    build_kernel
    
    # Create a bootable ISO
    create_iso
    
    print_success "LightOS built successfully!"
    
    # Ask if the user wants to run the OS
    read -p "Do you want to run LightOS in QEMU? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        run_os
    fi
}

# Run the main function
main

# Building and Running LightOS

This document provides instructions for building and running LightOS from source.

## Prerequisites

Before you can build LightOS, you need to have the following tools installed:

- **GCC** - The GNU Compiler Collection
- **NASM** - The Netwide Assembler
- **LD** - The GNU Linker (part of binutils)
- **GRUB** - The GNU GRand Unified Bootloader
- **QEMU** - A generic machine emulator and virtualizer (for testing)

### Installing Prerequisites on Debian/Ubuntu

```bash
sudo apt-get update
sudo apt-get install build-essential nasm qemu-system-x86 xorriso grub-pc-bin grub-common
```

### Installing Prerequisites on macOS (using Homebrew)

```bash
brew install x86_64-elf-gcc nasm qemu xorriso
```

Note: On macOS, you might need to use a cross-compiler for building.

### Installing Prerequisites on Windows

On Windows, it's recommended to use WSL (Windows Subsystem for Linux) with Ubuntu and follow the Debian/Ubuntu instructions above.

## Building LightOS

1. Clone the repository:

```bash
git clone https://github.com/yourusername/LightOS.git
cd LightOS
```

2. Run the build script:

```bash
./tools/build.sh
```

This will:
- Clean the build directory
- Build the bootloader
- Build the kernel and all components
- Create a bootable ISO

## Running LightOS

### Running in QEMU

The build script will ask if you want to run LightOS in QEMU after building. If you choose yes, it will automatically launch QEMU with the built ISO.

You can also manually run LightOS in QEMU:

```bash
qemu-system-x86_64 -cdrom build/lightos.iso
```

### Running on Real Hardware

To run LightOS on real hardware:

1. Burn the ISO to a CD/DVD:

```bash
wodim -v -dao dev=/dev/cdrom build/lightos.iso
```

2. Or create a bootable USB drive (be careful with this command, it will erase all data on the USB drive):

```bash
sudo dd if=build/lightos.iso of=/dev/sdX bs=4M status=progress
```

Replace `/dev/sdX` with your USB drive device (e.g., `/dev/sdb`).

3. Boot your computer from the CD/DVD or USB drive.

## Troubleshooting

### Build Errors

- **Missing dependencies**: Make sure all required tools are installed.
- **Compilation errors**: Check the error messages for details on what went wrong.

### Runtime Errors

- **Boot failures**: Ensure your BIOS/UEFI is configured to boot from CD/DVD or USB.
- **Kernel panics**: These indicate a problem with the kernel code.

## Development

For development, it's recommended to use QEMU for testing. You can also use GDB for debugging:

```bash
qemu-system-x86_64 -cdrom build/lightos.iso -s -S
```

In another terminal:

```bash
gdb
(gdb) target remote localhost:1234
(gdb) symbol-file build/kernel.bin
(gdb) break kernel_main
(gdb) continue
```

## Contributing

Contributions to LightOS are welcome! Please read the main README.md file for contribution guidelines.

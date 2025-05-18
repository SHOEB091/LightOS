# LightOS Developer Guide

## Introduction

This developer guide provides comprehensive information for developers who want to build applications for LightOS or contribute to the LightOS operating system itself. It covers the architecture, development environment setup, coding standards, and APIs.

## System Architecture

LightOS follows a modular architecture with the following main components:

### Kernel

The kernel is the core of the operating system and provides:
- Memory management
- Process scheduling
- Device drivers
- System calls
- Interrupt handling

### System Libraries

LightOS provides a set of system libraries:
- libc: Standard C library
- libm: Math library
- libnet: Networking library
- libgui: GUI library
- libsecurity: Security library

### Subsystems

LightOS is organized into several subsystems:
- Filesystem
- Networking
- Security
- Package Management
- Update Management
- Backup Management
- Monitoring
- Containerization
- Accessibility
- Localization
- IoT
- Mobile Integration
- Cloud Integration

## Development Environment Setup

### Prerequisites

- GCC compiler (version 9.0 or higher)
- Make build system
- Git version control
- CMake (version 3.10 or higher)
- Python (version 3.6 or higher)

### Getting the Source Code

```bash
git clone https://github.com/lightos/lightos.git
cd lightos
```

### Building LightOS

```bash
mkdir build
cd build
cmake ..
make
```

### Running LightOS in a Virtual Machine

```bash
make run
```

### Debugging LightOS

```bash
make debug
```

## Coding Standards

### C Code Style

- Use 4 spaces for indentation (no tabs)
- Maximum line length of 80 characters
- Use camelCase for function names and variables
- Use PascalCase for struct and enum names
- Use UPPER_CASE for constants and macros
- Always use braces for control structures, even for single-line blocks

### Documentation

- Document all public functions, structs, and enums
- Use Doxygen-style comments
- Include parameter descriptions, return values, and examples

### Error Handling

- Always check return values
- Use error codes consistently
- Provide meaningful error messages
- Clean up resources in error cases

### Memory Management

- Always free allocated memory
- Use appropriate allocation functions
- Check for allocation failures
- Avoid memory leaks

## Building Applications for LightOS

### Application Structure

A typical LightOS application has the following structure:

```
myapp/
├── CMakeLists.txt
├── include/
│   └── myapp.h
├── src/
│   ├── main.c
│   └── utils.c
├── tests/
│   └── test_myapp.c
└── README.md
```

### Building an Application

```bash
mkdir build
cd build
cmake ..
make
```

### Packaging an Application

```bash
make package
```

### Installing an Application

```bash
sudo package install myapp.pkg
```

## Contributing to LightOS

### Getting Started

1. Fork the LightOS repository
2. Create a branch for your feature or bug fix
3. Make your changes
4. Write tests for your changes
5. Submit a pull request

### Pull Request Process

1. Ensure your code follows the coding standards
2. Update the documentation
3. Add tests for your changes
4. Make sure all tests pass
5. Submit a pull request with a clear description of the changes

### Code Review Process

1. All code changes require review
2. At least one maintainer must approve the changes
3. CI tests must pass
4. Documentation must be updated

## Kernel Development

### Kernel Architecture

The LightOS kernel is a monolithic kernel with modular design:
- Core kernel: Memory management, process scheduling, system calls
- Device drivers: Hardware abstraction
- Filesystem: File operations, VFS
- Networking: TCP/IP stack, sockets

### Building the Kernel

```bash
cd kernel
make
```

### Kernel Modules

LightOS supports loadable kernel modules:
- Device drivers
- Filesystem drivers
- Network protocols

### Writing a Kernel Module

```c
#include <kernel/module.h>

int init_module(void) {
    // Initialization code
    return 0;
}

void cleanup_module(void) {
    // Cleanup code
}

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("My Kernel Module");
```

### Building a Kernel Module

```bash
cd modules/mymodule
make
```

### Loading a Kernel Module

```bash
insmod mymodule.ko
```

## Device Driver Development

### Driver Architecture

LightOS uses a unified driver model:
- Character devices
- Block devices
- Network devices
- Display devices (VGA and HDMI)

### Writing a Device Driver

```c
#include <kernel/device.h>

static int mydriver_probe(struct device *dev) {
    // Initialization code
    return 0;
}

static void mydriver_remove(struct device *dev) {
    // Cleanup code
}

static struct driver mydriver = {
    .name = "mydriver",
    .probe = mydriver_probe,
    .remove = mydriver_remove,
};

int init_module(void) {
    return register_driver(&mydriver);
}

void cleanup_module(void) {
    unregister_driver(&mydriver);
}
```

### Building a Device Driver

```bash
cd drivers/mydriver
make
```

### Display Drivers

LightOS provides comprehensive support for display devices:

#### VGA Support
- Text mode (80x25, 80x50)
- Graphics modes (up to 1280x1024)
- Hardware cursor
- Font customization

#### HDMI Support
- Full HD resolution (1920x1080p @ 60Hz) - Primary supported resolution
- 4K resolution (3840x2160)
- HDCP support
- Audio over HDMI
- CEC (Consumer Electronics Control)
- EDID (Extended Display Identification Data)
- Dedicated API for 1080p mode setting

#### Writing a Display Driver

```c
#include <kernel/display.h>

static int mydisplay_init(struct display_device *dev) {
    // Initialize the display hardware
    return 0;
}

static void mydisplay_set_mode(struct display_device *dev, int width, int height, int bpp) {
    // Set the display mode

    // Example of setting 1080p mode
    if (width == 1920 && height == 1080) {
        // Configure hardware for 1920x1080p @ 60Hz
        // This is the primary supported resolution
    }
}

static void mydisplay_set_pixel(struct display_device *dev, int x, int y, uint32_t color) {
    // Set a pixel color
}

static struct display_driver mydisplay = {
    .name = "mydisplay",
    .init = mydisplay_init,
    .set_mode = mydisplay_set_mode,
    .set_pixel = mydisplay_set_pixel,
};

int init_module(void) {
    return register_display_driver(&mydisplay);
}

void cleanup_module(void) {
    unregister_display_driver(&mydisplay);
}
```

## Filesystem Development

### Filesystem Architecture

LightOS supports multiple filesystems through a VFS layer:
- LightFS (native filesystem)
- ext2/ext3/ext4
- FAT32
- NTFS (read-only)

### Writing a Filesystem Driver

```c
#include <kernel/fs.h>

static struct file_operations myfs_file_ops = {
    .read = myfs_read,
    .write = myfs_write,
    .open = myfs_open,
    .close = myfs_close,
};

static struct filesystem_type myfs_fs_type = {
    .name = "myfs",
    .mount = myfs_mount,
    .unmount = myfs_unmount,
    .file_ops = &myfs_file_ops,
};

int init_module(void) {
    return register_filesystem(&myfs_fs_type);
}

void cleanup_module(void) {
    unregister_filesystem(&myfs_fs_type);
}
```

## Networking Development

### Networking Architecture

LightOS provides a complete networking stack:
- Socket API
- TCP/IP protocols
- Network device drivers
- Firewall

### Writing a Network Protocol

```c
#include <kernel/net.h>

static struct proto_ops myproto_ops = {
    .connect = myproto_connect,
    .disconnect = myproto_disconnect,
    .send = myproto_send,
    .recv = myproto_recv,
};

static struct protocol myproto = {
    .name = "myproto",
    .ops = &myproto_ops,
};

int init_module(void) {
    return register_protocol(&myproto);
}

void cleanup_module(void) {
    unregister_protocol(&myproto);
}
```

## GUI Development

### GUI Architecture

LightOS provides a lightweight GUI framework:
- Window manager
- Widget toolkit
- Event system
- Rendering engine

### Writing a GUI Application

```c
#include <gui/window.h>
#include <gui/button.h>

int main(int argc, char **argv) {
    Window *window = window_create("My App", 800, 600);
    Button *button = button_create(window, "Click Me", 10, 10, 100, 30);

    button_set_click_handler(button, on_button_click);

    window_show(window);
    gui_main_loop();

    button_destroy(button);
    window_destroy(window);

    return 0;
}
```

## Testing

### Unit Testing

LightOS uses a custom unit testing framework:
- Test cases
- Test suites
- Assertions
- Mocking

### Writing Unit Tests

```c
#include <test/test.h>

TEST_CASE(test_my_function) {
    // Test code
    ASSERT_EQ(my_function(1, 2), 3);
    ASSERT_NE(my_function(2, 2), 5);
}

TEST_SUITE(my_test_suite) {
    RUN_TEST(test_my_function);
}

int main(int argc, char **argv) {
    RUN_SUITE(my_test_suite);
    return 0;
}
```

### Running Tests

```bash
make test
```

## Debugging

### Debugging Tools

LightOS provides several debugging tools:
- GDB support
- System logs
- Kernel debugger
- Performance profiler

### Using GDB

```bash
gdb --args myapp arg1 arg2
```

### System Logs

```bash
log show
```

### Kernel Debugger

```bash
kdb
```

## Performance Optimization

### Profiling

LightOS includes a performance profiler:
- CPU usage
- Memory usage
- I/O operations
- Network traffic

### Using the Profiler

```bash
profile start myapp
# Run your application
profile stop
profile report
```

## Security

### Security Model

LightOS implements a comprehensive security model:
- User and group permissions
- Mandatory access control
- Capability-based security
- Secure boot

### Writing Secure Code

- Validate all input
- Use secure APIs
- Avoid buffer overflows
- Check return values
- Use least privilege principle

## Resources

- [LightOS Website](https://lightos.org)
- [API Reference](https://docs.lightos.org/api)
- [GitHub Repository](https://github.com/lightos/lightos)
- [Developer Forum](https://community.lightos.org/dev)
- [Bug Tracker](https://bugs.lightos.org)

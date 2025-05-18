# LightOS Hardware Drivers

## Overview

LightOS provides a comprehensive driver framework for supporting various hardware devices. The driver system is designed to be modular, allowing for easy addition of new drivers and devices.

## Driver Manager

The Driver Manager is the central component of the hardware driver system. It manages the detection, loading, and unloading of device drivers.

### Key Features

- **Automatic Hardware Detection**: Automatically detects hardware devices during system initialization.
- **Dynamic Driver Loading**: Loads appropriate drivers for detected devices.
- **Hot-Plugging Support**: Handles devices that are connected or disconnected while the system is running.
- **Driver Installation Interface**: Allows for installation of new drivers.

### API Reference

#### Initialization

```c
void driver_manager_init();
```

Initializes the driver manager and detects hardware devices.

#### Driver Registration

```c
int driver_manager_register_driver(Driver* driver);
```

Registers a driver with the driver manager.

#### Driver Unregistration

```c
int driver_manager_unregister_driver(Driver* driver);
```

Unregisters a driver from the driver manager.

#### Device Detection

```c
Device* driver_manager_detect_devices();
```

Detects hardware devices and returns an array of detected devices.

#### Driver Loading

```c
int driver_manager_load_driver(Device* device);
```

Loads a driver for a device.

#### Driver Unloading

```c
int driver_manager_unload_driver(Device* device);
```

Unloads a driver from a device.

#### Device Retrieval

```c
Device* driver_manager_get_device_by_type(DeviceType type, int index);
Device* driver_manager_get_device_by_name(const char* name);
```

Retrieves a device by type or name.

#### Information Display

```c
void driver_manager_print_devices();
void driver_manager_print_drivers();
```

Prints information about detected devices and registered drivers.

## Supported Device Types

### Input Devices

#### Keyboard

The keyboard driver provides support for PS/2 and USB keyboards.

##### Key Features

- **Key Mapping**: Maps scan codes to characters.
- **Modifier Key Support**: Handles Shift, Ctrl, Alt, and other modifier keys.
- **LED Control**: Controls keyboard LEDs (Caps Lock, Num Lock, Scroll Lock).
- **Key Repeat**: Handles key repeat for held keys.

##### API Reference

```c
void keyboard_init();
void keyboard_handler(unsigned char scancode);
void keyboard_buffer_put(char c);
char keyboard_buffer_get();
int keyboard_buffer_available();
char keyboard_read();
void keyboard_read_line(char* buffer, int max_length);
```

#### Mouse

The mouse driver provides support for PS/2 and USB mice.

##### Key Features

- **Movement Tracking**: Tracks mouse movement.
- **Button Handling**: Handles mouse button clicks.
- **Wheel Support**: Supports mouse wheel scrolling.
- **Event Callbacks**: Provides callbacks for mouse events.

##### API Reference

```c
void mouse_init();
void mouse_handler(unsigned char data);
void mouse_register_callback(mouse_callback_t callback);
void mouse_unregister_callback(mouse_callback_t callback);
int mouse_get_x();
int mouse_get_y();
int mouse_get_buttons();
void mouse_set_position(int x, int y);
void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y);
```

### Storage Devices

The storage driver provides support for various storage devices including HDD, SSD, NVMe, USB storage, and SD cards.

#### Key Features

- **Block Device Interface**: Provides a block device interface for file systems.
- **Read/Write Operations**: Supports reading and writing sectors.
- **Device Information**: Provides information about storage devices.
- **Hot-Plugging Support**: Handles devices that are connected or disconnected while the system is running.

#### API Reference

```c
void storage_init();
int storage_register_device(storage_device_t* device);
int storage_unregister_device(const char* name);
storage_device_t* storage_get_device(const char* name);
int storage_read_sectors(const char* device_name, unsigned int start_sector, unsigned int sector_count, void* buffer);
int storage_write_sectors(const char* device_name, unsigned int start_sector, unsigned int sector_count, const void* buffer);
int storage_flush(const char* device_name);
void storage_list_devices();
```

### Network Devices

The network driver provides support for Ethernet and WiFi network interfaces.

#### Key Features

- **Packet Transmission**: Sends network packets.
- **Packet Reception**: Receives network packets.
- **Link Status**: Monitors link status.
- **MAC Address Management**: Manages MAC addresses.

#### API Reference

```c
void network_driver_init();
int network_driver_register(network_driver_t* driver);
int network_driver_unregister(const char* name);
network_driver_t* network_driver_get(const char* name);
int network_driver_send_packet(const char* driver_name, const network_packet_t* packet);
int network_driver_receive_packet(const char* driver_name, network_packet_t* packet);
void network_driver_list();
```

### Display Devices

The display driver provides support for graphics cards and monitors.

#### Key Features

- **Mode Setting**: Sets display mode (resolution, color depth).
- **Framebuffer Access**: Provides access to the framebuffer.
- **Hardware Acceleration**: Utilizes hardware acceleration when available.
- **Multi-Monitor Support**: Supports multiple monitors.

### Audio Devices

The audio driver provides support for sound cards, speakers, and microphones.

#### Key Features

- **Playback**: Plays audio.
- **Recording**: Records audio.
- **Volume Control**: Controls volume.
- **Format Conversion**: Converts between audio formats.

### Peripheral Devices

The peripheral driver provides support for printers, scanners, webcams, and other peripheral devices.

#### Key Features

- **Device-Specific Operations**: Provides device-specific operations.
- **Data Transfer**: Transfers data to and from devices.
- **Device Control**: Controls device settings.

## Writing Custom Drivers

### Driver Structure

```c
typedef struct {
    char name[64];
    char description[128];
    char version[32];
    DeviceType supported_type;
    unsigned int supported_vendor_id;
    unsigned int supported_device_id;
    unsigned int supported_class_id;
    unsigned int supported_subclass_id;
    int (*probe)(Device* device);
    int (*init)(Device* device);
    int (*remove)(Device* device);
    int (*suspend)(Device* device);
    int (*resume)(Device* device);
    void* private_data;
} Driver;
```

### Device Structure

```c
typedef struct {
    char name[64];
    char description[128];
    char manufacturer[64];
    char model[64];
    char serial[32];
    DeviceType type;
    BusType bus;
    DeviceStatus status;
    unsigned int vendor_id;
    unsigned int device_id;
    unsigned int class_id;
    unsigned int subclass_id;
    void* driver;
    void* private_data;
} Device;
```

### Driver Implementation Steps

1. **Create a Driver Structure**: Define a driver structure with the necessary information.
2. **Implement Driver Functions**: Implement the required driver functions (probe, init, remove, suspend, resume).
3. **Register the Driver**: Register the driver with the driver manager.
4. **Handle Device Events**: Handle device events such as connection, disconnection, and errors.

### Example Driver

```c
// Example driver for a fictional device
Driver example_driver = {
    .name = "example",
    .description = "Example Driver",
    .version = "1.0.0",
    .supported_type = DEVICE_OTHER,
    .supported_vendor_id = 0x1234,
    .supported_device_id = 0x5678,
    .supported_class_id = 0,
    .supported_subclass_id = 0,
    .probe = example_probe,
    .init = example_init,
    .remove = example_remove,
    .suspend = example_suspend,
    .resume = example_resume,
    .private_data = NULL
};

int example_probe(Device* device) {
    // Check if the device is supported
    if (device->vendor_id == 0x1234 && device->device_id == 0x5678) {
        return 1; // Device is supported
    }
    
    return 0; // Device is not supported
}

int example_init(Device* device) {
    // Initialize the device
    terminal_write("Initializing example device\n");
    
    // Allocate private data
    device->private_data = allocate_block();
    
    if (!device->private_data) {
        terminal_write("Failed to allocate memory for example device\n");
        return -1;
    }
    
    // Initialize private data
    memset(device->private_data, 0, MEMORY_BLOCK_SIZE);
    
    return 0;
}

int example_remove(Device* device) {
    // Clean up the device
    terminal_write("Removing example device\n");
    
    // Free private data
    if (device->private_data) {
        free_block(device->private_data);
        device->private_data = NULL;
    }
    
    return 0;
}

int example_suspend(Device* device) {
    // Suspend the device
    terminal_write("Suspending example device\n");
    
    return 0;
}

int example_resume(Device* device) {
    // Resume the device
    terminal_write("Resuming example device\n");
    
    return 0;
}

// Register the driver
void example_driver_init() {
    driver_manager_register_driver(&example_driver);
}
```

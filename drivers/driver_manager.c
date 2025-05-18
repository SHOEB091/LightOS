/**
 * LightOS Driver Manager
 * Implementation of the driver manager
 */

#include "driver_manager.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// Maximum number of devices and drivers
#define MAX_DEVICES 256
#define MAX_DRIVERS 128

// Device and driver arrays
static Device devices[MAX_DEVICES];
static int device_count = 0;

static Driver* drivers[MAX_DRIVERS];
static int driver_count = 0;

// Initialize the driver manager
void driver_manager_init() {
    terminal_write("Initializing driver manager...\n");
    
    // Clear device and driver arrays
    memset(devices, 0, sizeof(devices));
    memset(drivers, 0, sizeof(drivers));
    
    device_count = 0;
    driver_count = 0;
    
    // Detect devices
    terminal_write("Detecting hardware devices...\n");
    
    // Detect different types of devices
    detect_keyboard_devices();
    detect_mouse_devices();
    detect_storage_devices();
    detect_display_devices();
    detect_network_devices();
    detect_audio_devices();
    detect_peripheral_devices();
    
    terminal_write("Driver manager initialized\n");
    terminal_write_color("Found ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert device_count to string
    char count_str[16];
    int temp = device_count;
    int i = 0;
    
    // Handle zero case
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        // Convert number to string (reversed)
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    // Null terminate
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = temp;
    }
    
    terminal_write_color(count_str, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_write_color(" devices\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

// Register a driver
int driver_manager_register_driver(Driver* driver) {
    if (driver_count >= MAX_DRIVERS) {
        terminal_write("Error: Maximum number of drivers reached\n");
        return -1;
    }
    
    // Add the driver to the array
    drivers[driver_count++] = driver;
    
    // Try to match the driver with existing devices
    for (int i = 0; i < device_count; i++) {
        Device* device = &devices[i];
        
        // Check if the driver supports this device
        if ((driver->supported_type == device->type || driver->supported_type == DEVICE_UNKNOWN) &&
            (driver->supported_vendor_id == device->vendor_id || driver->supported_vendor_id == 0) &&
            (driver->supported_device_id == device->device_id || driver->supported_device_id == 0) &&
            (driver->supported_class_id == device->class_id || driver->supported_class_id == 0) &&
            (driver->supported_subclass_id == device->subclass_id || driver->supported_subclass_id == 0)) {
            
            // Load the driver for this device
            driver_manager_load_driver(device);
        }
    }
    
    return 0;
}

// Unregister a driver
int driver_manager_unregister_driver(Driver* driver) {
    // Find the driver in the array
    int index = -1;
    
    for (int i = 0; i < driver_count; i++) {
        if (drivers[i] == driver) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: Driver not found\n");
        return -1;
    }
    
    // Unload the driver from all devices using it
    for (int i = 0; i < device_count; i++) {
        Device* device = &devices[i];
        
        if (device->driver == driver) {
            driver_manager_unload_driver(device);
        }
    }
    
    // Remove the driver from the array
    for (int i = index; i < driver_count - 1; i++) {
        drivers[i] = drivers[i + 1];
    }
    
    driver_count--;
    
    return 0;
}

// Detect devices (placeholder implementation)
Device* driver_manager_detect_devices() {
    // In a real system, this would scan the hardware buses
    // For now, we'll just return the devices array
    return devices;
}

// Load a driver for a device
int driver_manager_load_driver(Device* device) {
    if (!device) {
        terminal_write("Error: Invalid device\n");
        return -1;
    }
    
    // If the device already has a driver, unload it first
    if (device->driver) {
        driver_manager_unload_driver(device);
    }
    
    // Find a suitable driver
    Driver* suitable_driver = NULL;
    
    for (int i = 0; i < driver_count; i++) {
        Driver* driver = drivers[i];
        
        // Check if the driver supports this device
        if ((driver->supported_type == device->type || driver->supported_type == DEVICE_UNKNOWN) &&
            (driver->supported_vendor_id == device->vendor_id || driver->supported_vendor_id == 0) &&
            (driver->supported_device_id == device->device_id || driver->supported_device_id == 0) &&
            (driver->supported_class_id == device->class_id || driver->supported_class_id == 0) &&
            (driver->supported_subclass_id == device->subclass_id || driver->supported_subclass_id == 0)) {
            
            suitable_driver = driver;
            break;
        }
    }
    
    if (!suitable_driver) {
        terminal_write("Error: No suitable driver found for device: ");
        terminal_write(device->name);
        terminal_write("\n");
        return -1;
    }
    
    // Probe the device
    if (suitable_driver->probe && suitable_driver->probe(device) != 0) {
        terminal_write("Error: Device probe failed: ");
        terminal_write(device->name);
        terminal_write("\n");
        return -1;
    }
    
    // Initialize the driver
    if (suitable_driver->init && suitable_driver->init(device) != 0) {
        terminal_write("Error: Driver initialization failed: ");
        terminal_write(device->name);
        terminal_write("\n");
        return -1;
    }
    
    // Set the device driver
    device->driver = suitable_driver;
    device->status = STATUS_DRIVER_LOADED;
    
    terminal_write("Driver loaded for device: ");
    terminal_write(device->name);
    terminal_write("\n");
    
    return 0;
}

// Unload a driver from a device
int driver_manager_unload_driver(Device* device) {
    if (!device) {
        terminal_write("Error: Invalid device\n");
        return -1;
    }
    
    if (!device->driver) {
        terminal_write("Error: Device has no driver: ");
        terminal_write(device->name);
        terminal_write("\n");
        return -1;
    }
    
    Driver* driver = (Driver*)device->driver;
    
    // Remove the driver
    if (driver->remove && driver->remove(device) != 0) {
        terminal_write("Error: Driver removal failed: ");
        terminal_write(device->name);
        terminal_write("\n");
        return -1;
    }
    
    // Clear the device driver
    device->driver = NULL;
    device->status = STATUS_DETECTED;
    
    terminal_write("Driver unloaded from device: ");
    terminal_write(device->name);
    terminal_write("\n");
    
    return 0;
}

// Get a device by type and index
Device* driver_manager_get_device_by_type(DeviceType type, int index) {
    int count = 0;
    
    for (int i = 0; i < device_count; i++) {
        if (devices[i].type == type) {
            if (count == index) {
                return &devices[i];
            }
            count++;
        }
    }
    
    return NULL;
}

// Get a device by name
Device* driver_manager_get_device_by_name(const char* name) {
    for (int i = 0; i < device_count; i++) {
        if (strcmp(devices[i].name, name) == 0) {
            return &devices[i];
        }
    }
    
    return NULL;
}

// Print all detected devices
void driver_manager_print_devices() {
    terminal_write("Detected Devices:\n");
    terminal_write("----------------\n");
    
    for (int i = 0; i < device_count; i++) {
        Device* device = &devices[i];
        
        // Print device information
        terminal_write(device->name);
        terminal_write(" - ");
        terminal_write(device->description);
        terminal_write(" (");
        
        // Print device status
        switch (device->status) {
            case STATUS_DETECTED:
                terminal_write_color("DETECTED", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                break;
            case STATUS_DRIVER_LOADED:
                terminal_write_color("DRIVER LOADED", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
                break;
            case STATUS_ACTIVE:
                terminal_write_color("ACTIVE", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
                break;
            case STATUS_ERROR:
                terminal_write_color("ERROR", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
                break;
            case STATUS_DISABLED:
                terminal_write_color("DISABLED", VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
                break;
            default:
                terminal_write_color("UNKNOWN", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
                break;
        }
        
        terminal_write(")\n");
    }
}

// Print all registered drivers
void driver_manager_print_drivers() {
    terminal_write("Registered Drivers:\n");
    terminal_write("------------------\n");
    
    for (int i = 0; i < driver_count; i++) {
        Driver* driver = drivers[i];
        
        // Print driver information
        terminal_write(driver->name);
        terminal_write(" - ");
        terminal_write(driver->description);
        terminal_write(" (");
        terminal_write(driver->version);
        terminal_write(")\n");
    }
}

// Device detection functions (placeholder implementations)
// In a real system, these would scan the hardware buses

int detect_keyboard_devices() {
    // Simulate detecting a keyboard
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "keyboard0");
        strcpy(device->description, "Standard PS/2 Keyboard");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "PS/2 Keyboard");
        device->type = DEVICE_KEYBOARD;
        device->bus = BUS_PS2;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0001;
        device->device_id = 0x0001;
        device->class_id = 0x0009;
        device->subclass_id = 0x0001;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

int detect_mouse_devices() {
    // Simulate detecting a mouse
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "mouse0");
        strcpy(device->description, "USB Optical Mouse");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "USB Mouse");
        device->type = DEVICE_MOUSE;
        device->bus = BUS_USB;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0002;
        device->device_id = 0x0001;
        device->class_id = 0x0009;
        device->subclass_id = 0x0002;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

// Other device detection functions would be implemented similarly
int detect_storage_devices() {
    // Simulate detecting a storage device
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "sda");
        strcpy(device->description, "SATA Hard Disk Drive");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "SATA HDD 500GB");
        device->type = DEVICE_STORAGE;
        device->bus = BUS_SATA;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0003;
        device->device_id = 0x0001;
        device->class_id = 0x0001;
        device->subclass_id = 0x0001;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

int detect_display_devices() {
    // Simulate detecting a display device
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "card0");
        strcpy(device->description, "PCI Graphics Card");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "Graphics Adapter");
        device->type = DEVICE_DISPLAY;
        device->bus = BUS_PCI;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0004;
        device->device_id = 0x0001;
        device->class_id = 0x0003;
        device->subclass_id = 0x0000;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

int detect_network_devices() {
    // Simulate detecting a network device
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "eth0");
        strcpy(device->description, "Ethernet Controller");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "Ethernet Adapter");
        device->type = DEVICE_NETWORK;
        device->bus = BUS_PCI;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0005;
        device->device_id = 0x0001;
        device->class_id = 0x0002;
        device->subclass_id = 0x0000;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

int detect_audio_devices() {
    // Simulate detecting an audio device
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "audio0");
        strcpy(device->description, "Audio Controller");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "Sound Card");
        device->type = DEVICE_AUDIO;
        device->bus = BUS_PCI;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0006;
        device->device_id = 0x0001;
        device->class_id = 0x0004;
        device->subclass_id = 0x0001;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

int detect_peripheral_devices() {
    // Simulate detecting a webcam
    if (device_count < MAX_DEVICES) {
        Device* device = &devices[device_count++];
        
        strcpy(device->name, "video0");
        strcpy(device->description, "USB Webcam");
        strcpy(device->manufacturer, "Generic");
        strcpy(device->model, "HD Webcam");
        device->type = DEVICE_WEBCAM;
        device->bus = BUS_USB;
        device->status = STATUS_DETECTED;
        device->vendor_id = 0x0007;
        device->device_id = 0x0001;
        device->class_id = 0x000E;
        device->subclass_id = 0x0001;
        device->driver = NULL;
        
        return 1;
    }
    
    return 0;
}

// Hot-plug support
void driver_manager_handle_device_added(Device* device) {
    if (!device) {
        return;
    }
    
    // Add the device to the array
    if (device_count < MAX_DEVICES) {
        devices[device_count++] = *device;
        
        terminal_write("Device added: ");
        terminal_write(device->name);
        terminal_write("\n");
        
        // Try to load a driver for the device
        driver_manager_load_driver(&devices[device_count - 1]);
    }
}

void driver_manager_handle_device_removed(Device* device) {
    if (!device) {
        return;
    }
    
    // Find the device in the array
    int index = -1;
    
    for (int i = 0; i < device_count; i++) {
        if (strcmp(devices[i].name, device->name) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return;
    }
    
    // Unload the driver if needed
    if (devices[index].driver) {
        driver_manager_unload_driver(&devices[index]);
    }
    
    terminal_write("Device removed: ");
    terminal_write(devices[index].name);
    terminal_write("\n");
    
    // Remove the device from the array
    for (int i = index; i < device_count - 1; i++) {
        devices[i] = devices[i + 1];
    }
    
    device_count--;
}

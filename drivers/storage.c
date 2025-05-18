/**
 * LightOS Drivers
 * Storage driver implementation
 */

#include "storage.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// Maximum number of storage devices
#define MAX_STORAGE_DEVICES 16

// Storage device array
static storage_device_t* storage_devices[MAX_STORAGE_DEVICES];
static int storage_device_count = 0;

// Initialize storage subsystem
void storage_init() {
    terminal_write("Initializing storage subsystem...\n");
    
    // Clear the device array
    for (int i = 0; i < MAX_STORAGE_DEVICES; i++) {
        storage_devices[i] = NULL;
    }
    
    storage_device_count = 0;
    
    // Detect storage devices
    detect_ata_devices();
    detect_nvme_devices();
    detect_usb_storage_devices();
    detect_sd_devices();
    
    terminal_write("Storage subsystem initialized\n");
    terminal_write_color("Found ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert device_count to string
    char count_str[16];
    int temp = storage_device_count;
    int i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write(count_str);
    terminal_write(" storage devices\n");
}

// Register a storage device
int storage_register_device(storage_device_t* device) {
    if (!device) {
        return -1;
    }
    
    // Check if we have room for another device
    if (storage_device_count >= MAX_STORAGE_DEVICES) {
        terminal_write("Error: Maximum number of storage devices reached\n");
        return -1;
    }
    
    // Check if a device with the same name already exists
    for (int i = 0; i < storage_device_count; i++) {
        if (strcmp(storage_devices[i]->name, device->name) == 0) {
            terminal_write("Error: Storage device with name '");
            terminal_write(device->name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Allocate memory for the device
    storage_device_t* new_device = (storage_device_t*)allocate_block();
    
    if (!new_device) {
        terminal_write("Error: Failed to allocate memory for storage device\n");
        return -1;
    }
    
    // Copy the device data
    memcpy(new_device, device, sizeof(storage_device_t));
    
    // Add the device to the array
    storage_devices[storage_device_count++] = new_device;
    
    terminal_write("Registered storage device: ");
    terminal_write(device->name);
    terminal_write("\n");
    
    return 0;
}

// Unregister a storage device
int storage_unregister_device(const char* name) {
    if (!name) {
        return -1;
    }
    
    // Find the device
    for (int i = 0; i < storage_device_count; i++) {
        if (strcmp(storage_devices[i]->name, name) == 0) {
            // Free the device memory
            free_block(storage_devices[i]);
            
            // Remove the device from the array by shifting all subsequent devices
            for (int j = i; j < storage_device_count - 1; j++) {
                storage_devices[j] = storage_devices[j + 1];
            }
            
            storage_devices[--storage_device_count] = NULL;
            
            terminal_write("Unregistered storage device: ");
            terminal_write(name);
            terminal_write("\n");
            
            return 0;
        }
    }
    
    terminal_write("Error: Storage device '");
    terminal_write(name);
    terminal_write("' not found\n");
    
    return -1;
}

// Get a storage device by name
storage_device_t* storage_get_device(const char* name) {
    if (!name) {
        return NULL;
    }
    
    // Find the device
    for (int i = 0; i < storage_device_count; i++) {
        if (strcmp(storage_devices[i]->name, name) == 0) {
            return storage_devices[i];
        }
    }
    
    return NULL;
}

// Read sectors from a storage device
int storage_read_sectors(const char* device_name, unsigned int start_sector, unsigned int sector_count, void* buffer) {
    storage_device_t* device = storage_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Storage device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->read_sectors) {
        terminal_write("Error: Storage device '");
        terminal_write(device_name);
        terminal_write("' does not support reading\n");
        return -1;
    }
    
    return device->read_sectors(device, start_sector, sector_count, buffer);
}

// Write sectors to a storage device
int storage_write_sectors(const char* device_name, unsigned int start_sector, unsigned int sector_count, const void* buffer) {
    storage_device_t* device = storage_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Storage device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (device->read_only) {
        terminal_write("Error: Storage device '");
        terminal_write(device_name);
        terminal_write("' is read-only\n");
        return -1;
    }
    
    if (!device->write_sectors) {
        terminal_write("Error: Storage device '");
        terminal_write(device_name);
        terminal_write("' does not support writing\n");
        return -1;
    }
    
    return device->write_sectors(device, start_sector, sector_count, buffer);
}

// Flush a storage device's cache
int storage_flush(const char* device_name) {
    storage_device_t* device = storage_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Storage device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->flush) {
        // If the device doesn't support flushing, just return success
        return 0;
    }
    
    return device->flush(device);
}

// List all storage devices
void storage_list_devices() {
    terminal_write("Storage Devices:\n");
    terminal_write("----------------\n");
    
    if (storage_device_count == 0) {
        terminal_write("No storage devices found\n");
        return;
    }
    
    for (int i = 0; i < storage_device_count; i++) {
        storage_device_t* device = storage_devices[i];
        
        terminal_write(device->name);
        terminal_write(": ");
        
        // Print device type
        switch (device->type) {
            case STORAGE_TYPE_HDD:
                terminal_write("HDD");
                break;
            case STORAGE_TYPE_SSD:
                terminal_write("SSD");
                break;
            case STORAGE_TYPE_NVME:
                terminal_write("NVMe");
                break;
            case STORAGE_TYPE_USB:
                terminal_write("USB");
                break;
            case STORAGE_TYPE_SD:
                terminal_write("SD");
                break;
            case STORAGE_TYPE_CDROM:
                terminal_write("CD-ROM");
                break;
            case STORAGE_TYPE_FLOPPY:
                terminal_write("Floppy");
                break;
            default:
                terminal_write("Unknown");
                break;
        }
        
        terminal_write(", ");
        
        // Print size (in GB, MB, or KB as appropriate)
        unsigned long long size_kb = device->size / 1024;
        unsigned long long size_mb = size_kb / 1024;
        unsigned long long size_gb = size_mb / 1024;
        
        char size_str[32];
        int idx = 0;
        
        if (size_gb > 0) {
            // Convert size_gb to string
            unsigned long long temp = size_gb;
            if (temp == 0) {
                size_str[idx++] = '0';
            } else {
                while (temp > 0) {
                    size_str[idx++] = '0' + (temp % 10);
                    temp /= 10;
                }
            }
            
            // Reverse the string
            for (int j = 0; j < idx / 2; j++) {
                char tmp = size_str[j];
                size_str[j] = size_str[idx - j - 1];
                size_str[idx - j - 1] = tmp;
            }
            
            size_str[idx] = '\0';
            terminal_write(size_str);
            terminal_write(" GB");
        } else if (size_mb > 0) {
            // Convert size_mb to string
            unsigned long long temp = size_mb;
            if (temp == 0) {
                size_str[idx++] = '0';
            } else {
                while (temp > 0) {
                    size_str[idx++] = '0' + (temp % 10);
                    temp /= 10;
                }
            }
            
            // Reverse the string
            for (int j = 0; j < idx / 2; j++) {
                char tmp = size_str[j];
                size_str[j] = size_str[idx - j - 1];
                size_str[idx - j - 1] = tmp;
            }
            
            size_str[idx] = '\0';
            terminal_write(size_str);
            terminal_write(" MB");
        } else {
            // Convert size_kb to string
            unsigned long long temp = size_kb;
            if (temp == 0) {
                size_str[idx++] = '0';
            } else {
                while (temp > 0) {
                    size_str[idx++] = '0' + (temp % 10);
                    temp /= 10;
                }
            }
            
            // Reverse the string
            for (int j = 0; j < idx / 2; j++) {
                char tmp = size_str[j];
                size_str[j] = size_str[idx - j - 1];
                size_str[idx - j - 1] = tmp;
            }
            
            size_str[idx] = '\0';
            terminal_write(size_str);
            terminal_write(" KB");
        }
        
        terminal_write("\n");
    }
}

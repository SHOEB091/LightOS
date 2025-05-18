/**
 * LightOS Drivers
 * Display driver implementation
 */

#include "display.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// Maximum number of display devices
#define MAX_DISPLAY_DEVICES 4

// Display device array
static display_device_t* display_devices[MAX_DISPLAY_DEVICES];
static int display_device_count = 0;

// Initialize display subsystem
void display_init() {
    terminal_write("Initializing display subsystem...\n");
    
    // Clear the device array
    for (int i = 0; i < MAX_DISPLAY_DEVICES; i++) {
        display_devices[i] = NULL;
    }
    
    display_device_count = 0;
    
    // Detect display devices
    detect_display_devices();
    
    terminal_write("Display subsystem initialized\n");
    terminal_write_color("Found ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert device_count to string
    char count_str[16];
    int temp = display_device_count;
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
    terminal_write(" display devices\n");
}

// Register a display device
int display_register_device(display_device_t* device) {
    if (!device) {
        return -1;
    }
    
    // Check if we have room for another device
    if (display_device_count >= MAX_DISPLAY_DEVICES) {
        terminal_write("Error: Maximum number of display devices reached\n");
        return -1;
    }
    
    // Check if a device with the same name already exists
    for (int i = 0; i < display_device_count; i++) {
        if (strcmp(display_devices[i]->name, device->name) == 0) {
            terminal_write("Error: Display device with name '");
            terminal_write(device->name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Allocate memory for the device
    display_device_t* new_device = (display_device_t*)allocate_block();
    
    if (!new_device) {
        terminal_write("Error: Failed to allocate memory for display device\n");
        return -1;
    }
    
    // Copy the device data
    memcpy(new_device, device, sizeof(display_device_t));
    
    // Add the device to the array
    display_devices[display_device_count++] = new_device;
    
    terminal_write("Registered display device: ");
    terminal_write(device->name);
    terminal_write("\n");
    
    // Initialize the device if it has an init function
    if (new_device->init) {
        if (new_device->init(new_device) != 0) {
            terminal_write("Warning: Failed to initialize display device '");
            terminal_write(new_device->name);
            terminal_write("'\n");
        }
    }
    
    return 0;
}

// Unregister a display device
int display_unregister_device(const char* name) {
    if (!name) {
        return -1;
    }
    
    // Find the device
    for (int i = 0; i < display_device_count; i++) {
        if (strcmp(display_devices[i]->name, name) == 0) {
            // Close the device if it has a close function
            if (display_devices[i]->close) {
                display_devices[i]->close(display_devices[i]);
            }
            
            // Free the device memory
            free_block(display_devices[i]);
            
            // Remove the device from the array by shifting all subsequent devices
            for (int j = i; j < display_device_count - 1; j++) {
                display_devices[j] = display_devices[j + 1];
            }
            
            display_devices[--display_device_count] = NULL;
            
            terminal_write("Unregistered display device: ");
            terminal_write(name);
            terminal_write("\n");
            
            return 0;
        }
    }
    
    terminal_write("Error: Display device '");
    terminal_write(name);
    terminal_write("' not found\n");
    
    return -1;
}

// Get a display device by name
display_device_t* display_get_device(const char* name) {
    if (!name) {
        return NULL;
    }
    
    // Find the device
    for (int i = 0; i < display_device_count; i++) {
        if (strcmp(display_devices[i]->name, name) == 0) {
            return display_devices[i];
        }
    }
    
    return NULL;
}

// Set the mode of a display device
int display_set_mode(const char* device_name, display_mode_t* mode) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->set_mode) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support setting mode\n");
        return -1;
    }
    
    return device->set_mode(device, mode);
}

// Get the mode of a display device
int display_get_mode(const char* device_name, display_mode_t* mode) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->get_mode) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support getting mode\n");
        return -1;
    }
    
    return device->get_mode(device, mode);
}

// Clear a display device
int display_clear(const char* device_name, unsigned int color) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->clear) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support clearing\n");
        return -1;
    }
    
    return device->clear(device, color);
}

// Update a region of a display device
int display_update(const char* device_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->update) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support updating\n");
        return -1;
    }
    
    return device->update(device, x, y, width, height);
}

// Set the brightness of a display device
int display_set_brightness(const char* device_name, unsigned int brightness) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->set_brightness) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support setting brightness\n");
        return -1;
    }
    
    return device->set_brightness(device, brightness);
}

// Get the brightness of a display device
int display_get_brightness(const char* device_name) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->get_brightness) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support getting brightness\n");
        return -1;
    }
    
    return device->get_brightness(device);
}

// Set the contrast of a display device
int display_set_contrast(const char* device_name, unsigned int contrast) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->set_contrast) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support setting contrast\n");
        return -1;
    }
    
    return device->set_contrast(device, contrast);
}

// Get the contrast of a display device
int display_get_contrast(const char* device_name) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->get_contrast) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' does not support getting contrast\n");
        return -1;
    }
    
    return device->get_contrast(device);
}

// List all display devices
void display_list_devices() {
    terminal_write("Display Devices:\n");
    terminal_write("----------------\n");
    
    if (display_device_count == 0) {
        terminal_write("No display devices found\n");
        return;
    }
    
    for (int i = 0; i < display_device_count; i++) {
        terminal_write(display_devices[i]->name);
        terminal_write(": ");
        
        // Print current mode
        terminal_write("Resolution: ");
        
        // Convert width to string
        char width_str[16];
        int temp = display_devices[i]->current_mode.width;
        int idx = 0;
        
        if (temp == 0) {
            width_str[idx++] = '0';
        } else {
            while (temp > 0) {
                width_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        width_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = width_str[j];
            width_str[j] = width_str[idx - j - 1];
            width_str[idx - j - 1] = tmp;
        }
        
        terminal_write(width_str);
        terminal_write("x");
        
        // Convert height to string
        char height_str[16];
        temp = display_devices[i]->current_mode.height;
        idx = 0;
        
        if (temp == 0) {
            height_str[idx++] = '0';
        } else {
            while (temp > 0) {
                height_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        height_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = height_str[j];
            height_str[j] = height_str[idx - j - 1];
            height_str[idx - j - 1] = tmp;
        }
        
        terminal_write(height_str);
        terminal_write(", ");
        
        // Print color depth
        terminal_write("Depth: ");
        
        // Convert bpp to string
        char bpp_str[16];
        temp = display_devices[i]->current_mode.bpp;
        idx = 0;
        
        if (temp == 0) {
            bpp_str[idx++] = '0';
        } else {
            while (temp > 0) {
                bpp_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        bpp_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = bpp_str[j];
            bpp_str[j] = bpp_str[idx - j - 1];
            bpp_str[idx - j - 1] = tmp;
        }
        
        terminal_write(bpp_str);
        terminal_write(" bpp, ");
        
        // Print refresh rate
        terminal_write("Refresh: ");
        
        // Convert refresh rate to string
        char refresh_str[16];
        temp = display_devices[i]->current_mode.refresh_rate;
        idx = 0;
        
        if (temp == 0) {
            refresh_str[idx++] = '0';
        } else {
            while (temp > 0) {
                refresh_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        refresh_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = refresh_str[j];
            refresh_str[j] = refresh_str[idx - j - 1];
            refresh_str[idx - j - 1] = tmp;
        }
        
        terminal_write(refresh_str);
        terminal_write(" Hz");
        
        terminal_write("\n");
    }
}

// List all supported modes of a display device
void display_list_modes(const char* device_name) {
    display_device_t* device = display_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Display device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return;
    }
    
    terminal_write("Supported Modes for '");
    terminal_write(device_name);
    terminal_write("':\n");
    terminal_write("------------------------\n");
    
    if (device->supported_mode_count == 0 || !device->supported_modes) {
        terminal_write("No supported modes found\n");
        return;
    }
    
    for (unsigned int i = 0; i < device->supported_mode_count; i++) {
        // Convert mode index to string
        char index_str[16];
        int temp = i + 1;
        int idx = 0;
        
        if (temp == 0) {
            index_str[idx++] = '0';
        } else {
            while (temp > 0) {
                index_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        index_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = index_str[j];
            index_str[j] = index_str[idx - j - 1];
            index_str[idx - j - 1] = tmp;
        }
        
        terminal_write(index_str);
        terminal_write(": ");
        
        // Print resolution
        // Convert width to string
        char width_str[16];
        temp = device->supported_modes[i].width;
        idx = 0;
        
        if (temp == 0) {
            width_str[idx++] = '0';
        } else {
            while (temp > 0) {
                width_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        width_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = width_str[j];
            width_str[j] = width_str[idx - j - 1];
            width_str[idx - j - 1] = tmp;
        }
        
        terminal_write(width_str);
        terminal_write("x");
        
        // Convert height to string
        char height_str[16];
        temp = device->supported_modes[i].height;
        idx = 0;
        
        if (temp == 0) {
            height_str[idx++] = '0';
        } else {
            while (temp > 0) {
                height_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        height_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = height_str[j];
            height_str[j] = height_str[idx - j - 1];
            height_str[idx - j - 1] = tmp;
        }
        
        terminal_write(height_str);
        terminal_write(", ");
        
        // Print color depth
        // Convert bpp to string
        char bpp_str[16];
        temp = device->supported_modes[i].bpp;
        idx = 0;
        
        if (temp == 0) {
            bpp_str[idx++] = '0';
        } else {
            while (temp > 0) {
                bpp_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        bpp_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = bpp_str[j];
            bpp_str[j] = bpp_str[idx - j - 1];
            bpp_str[idx - j - 1] = tmp;
        }
        
        terminal_write(bpp_str);
        terminal_write(" bpp, ");
        
        // Print refresh rate
        // Convert refresh rate to string
        char refresh_str[16];
        temp = device->supported_modes[i].refresh_rate;
        idx = 0;
        
        if (temp == 0) {
            refresh_str[idx++] = '0';
        } else {
            while (temp > 0) {
                refresh_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        refresh_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = refresh_str[j];
            refresh_str[j] = refresh_str[idx - j - 1];
            refresh_str[idx - j - 1] = tmp;
        }
        
        terminal_write(refresh_str);
        terminal_write(" Hz");
        
        terminal_write("\n");
    }
}

// Detect display devices
int detect_display_devices() {
    // In a real system, this would scan for display devices
    // For now, just simulate finding a display
    
    display_device_t device;
    
    // VGA display
    strcpy(device.name, "vga0");
    device.current_mode.width = 640;
    device.current_mode.height = 480;
    device.current_mode.bpp = 32;
    device.current_mode.refresh_rate = 60;
    
    // Allocate memory for supported modes
    device.supported_modes = (display_mode_t*)allocate_block();
    
    if (!device.supported_modes) {
        terminal_write("Error: Failed to allocate memory for supported modes\n");
        return 0;
    }
    
    // Add some supported modes
    device.supported_modes[0].width = 640;
    device.supported_modes[0].height = 480;
    device.supported_modes[0].bpp = 32;
    device.supported_modes[0].refresh_rate = 60;
    
    device.supported_modes[1].width = 800;
    device.supported_modes[1].height = 600;
    device.supported_modes[1].bpp = 32;
    device.supported_modes[1].refresh_rate = 60;
    
    device.supported_modes[2].width = 1024;
    device.supported_modes[2].height = 768;
    device.supported_modes[2].bpp = 32;
    device.supported_modes[2].refresh_rate = 60;
    
    device.supported_mode_count = 3;
    
    // Allocate a framebuffer
    device.framebuffer_size = device.current_mode.width * device.current_mode.height * (device.current_mode.bpp / 8);
    device.framebuffer = allocate_blocks((device.framebuffer_size + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!device.framebuffer) {
        terminal_write("Error: Failed to allocate framebuffer\n");
        free_block(device.supported_modes);
        return 0;
    }
    
    device.pitch = device.current_mode.width * (device.current_mode.bpp / 8);
    
    // Function pointers would be set to actual functions in a real system
    device.init = NULL;
    device.close = NULL;
    device.set_mode = NULL;
    device.get_mode = NULL;
    device.clear = NULL;
    device.update = NULL;
    device.set_brightness = NULL;
    device.get_brightness = NULL;
    device.set_contrast = NULL;
    device.get_contrast = NULL;
    
    device.private_data = NULL;
    
    display_register_device(&device);
    
    return 1; // Found 1 device
}

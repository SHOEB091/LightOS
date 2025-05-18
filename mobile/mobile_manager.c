/**
 * LightOS Mobile
 * Mobile Manager implementation
 */

#include "mobile_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "protocols/adb_support.h"
#include "protocols/mtp_support.h"
#include "sync/file_sync.h"
#include "sync/contact_sync.h"

// Maximum number of mobile devices
#define MAX_MOBILE_DEVICES 10

// Mobile device array
static mobile_device_t* mobile_devices[MAX_MOBILE_DEVICES];
static unsigned int mobile_device_count = 0;

// Initialize the mobile manager
void mobile_manager_init() {
    terminal_write("Initializing mobile manager...\n");
    
    // Clear the device array
    for (int i = 0; i < MAX_MOBILE_DEVICES; i++) {
        mobile_devices[i] = NULL;
    }
    
    mobile_device_count = 0;
    
    // Initialize mobile protocols
    adb_init();
    mtp_init();
    
    // Initialize sync modules
    file_sync_init();
    contact_sync_init();
    
    terminal_write("Mobile manager initialized\n");
}

// Detect connected mobile devices
int mobile_detect_devices(mobile_device_t** devices, unsigned int* count) {
    if (!devices || !count) {
        terminal_write("Error: Devices and count cannot be NULL\n");
        return -1;
    }
    
    terminal_write("Detecting mobile devices...\n");
    
    // Clear the device array
    for (int i = 0; i < mobile_device_count; i++) {
        if (mobile_devices[i]) {
            free_block(mobile_devices[i]);
            mobile_devices[i] = NULL;
        }
    }
    
    mobile_device_count = 0;
    
    // Detect Android devices using ADB
    mobile_device_t* android_devices[MAX_MOBILE_DEVICES];
    unsigned int android_count = 0;
    
    if (adb_detect_devices(android_devices, &android_count) != 0) {
        terminal_write("Error: Failed to detect Android devices\n");
    } else {
        // Add Android devices to the device array
        for (unsigned int i = 0; i < android_count && mobile_device_count < MAX_MOBILE_DEVICES; i++) {
            mobile_devices[mobile_device_count++] = android_devices[i];
        }
    }
    
    // Detect iOS devices
    // In a real system, we would detect iOS devices using libimobiledevice
    
    // Detect MTP devices
    mobile_device_t* mtp_devices[MAX_MOBILE_DEVICES];
    unsigned int mtp_count = 0;
    
    if (mtp_detect_devices(mtp_devices, &mtp_count) != 0) {
        terminal_write("Error: Failed to detect MTP devices\n");
    } else {
        // Add MTP devices to the device array
        for (unsigned int i = 0; i < mtp_count && mobile_device_count < MAX_MOBILE_DEVICES; i++) {
            mobile_devices[mobile_device_count++] = mtp_devices[i];
        }
    }
    
    // Return the detected devices
    *devices = mobile_devices;
    *count = mobile_device_count;
    
    terminal_write("Detected ");
    
    // Convert count to string
    char count_str[16];
    sprintf(count_str, "%u", mobile_device_count);
    terminal_write(count_str);
    terminal_write(" mobile device(s)\n");
    
    return 0;
}

// Connect to a mobile device
int mobile_connect_device(const char* id, mobile_connection_type_t connection_type) {
    if (!id) {
        terminal_write("Error: Device ID cannot be NULL\n");
        return -1;
    }
    
    // Find the device
    mobile_device_t* device = mobile_get_device(id);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the device is already connected
    if (device->connection_state == MOBILE_CONNECTION_STATE_CONNECTED) {
        terminal_write("Device '");
        terminal_write(id);
        terminal_write("' is already connected\n");
        return 0;
    }
    
    terminal_write("Connecting to mobile device '");
    terminal_write(id);
    terminal_write("'...\n");
    
    // Set the connection type
    device->connection_type = connection_type;
    
    // Connect to the device based on its type
    int result = -1;
    
    switch (device->type) {
        case MOBILE_DEVICE_ANDROID:
            result = android_adb_connect(id);
            break;
        
        case MOBILE_DEVICE_IOS:
            result = ios_connect(id);
            break;
        
        default:
            terminal_write("Error: Unsupported device type\n");
            break;
    }
    
    if (result == 0) {
        device->connection_state = MOBILE_CONNECTION_STATE_CONNECTED;
        terminal_write("Connected to mobile device '");
        terminal_write(id);
        terminal_write("'\n");
    } else {
        device->connection_state = MOBILE_CONNECTION_STATE_ERROR;
        terminal_write("Error: Failed to connect to mobile device '");
        terminal_write(id);
        terminal_write("'\n");
    }
    
    return result;
}

// Disconnect from a mobile device
int mobile_disconnect_device(const char* id) {
    if (!id) {
        terminal_write("Error: Device ID cannot be NULL\n");
        return -1;
    }
    
    // Find the device
    mobile_device_t* device = mobile_get_device(id);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the device is already disconnected
    if (device->connection_state == MOBILE_CONNECTION_STATE_DISCONNECTED) {
        terminal_write("Device '");
        terminal_write(id);
        terminal_write("' is already disconnected\n");
        return 0;
    }
    
    terminal_write("Disconnecting from mobile device '");
    terminal_write(id);
    terminal_write("'...\n");
    
    // Disconnect from the device based on its type
    int result = -1;
    
    switch (device->type) {
        case MOBILE_DEVICE_ANDROID:
            result = android_adb_disconnect(id);
            break;
        
        case MOBILE_DEVICE_IOS:
            result = ios_disconnect(id);
            break;
        
        default:
            terminal_write("Error: Unsupported device type\n");
            break;
    }
    
    if (result == 0) {
        device->connection_state = MOBILE_CONNECTION_STATE_DISCONNECTED;
        terminal_write("Disconnected from mobile device '");
        terminal_write(id);
        terminal_write("'\n");
    } else {
        terminal_write("Error: Failed to disconnect from mobile device '");
        terminal_write(id);
        terminal_write("'\n");
    }
    
    return result;
}

// Get a mobile device by ID
mobile_device_t* mobile_get_device(const char* id) {
    if (!id) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < mobile_device_count; i++) {
        if (strcmp(mobile_devices[i]->id, id) == 0) {
            return mobile_devices[i];
        }
    }
    
    return NULL;
}

// Get information about a mobile device
int mobile_get_device_info(const char* id, mobile_device_t* device) {
    if (!id || !device) {
        terminal_write("Error: Device ID and device cannot be NULL\n");
        return -1;
    }
    
    // Find the device
    mobile_device_t* found_device = mobile_get_device(id);
    
    if (!found_device) {
        terminal_write("Error: Device '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Copy the device information
    *device = *found_device;
    
    return 0;
}

// List all mobile devices
int mobile_list_devices(mobile_device_t** devices, unsigned int* count) {
    if (!devices || !count) {
        terminal_write("Error: Devices and count cannot be NULL\n");
        return -1;
    }
    
    // Return the devices
    *devices = mobile_devices;
    *count = mobile_device_count;
    
    return 0;
}

// Start a sync operation
int mobile_start_sync(const char* device_id, mobile_sync_type_t type, mobile_sync_direction_t direction, const char* source_path, const char* destination_path) {
    if (!device_id || !source_path || !destination_path) {
        terminal_write("Error: Device ID, source path, and destination path cannot be NULL\n");
        return -1;
    }
    
    // Find the device
    mobile_device_t* device = mobile_get_device(device_id);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(device_id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the device is connected
    if (device->connection_state != MOBILE_CONNECTION_STATE_CONNECTED) {
        terminal_write("Error: Device '");
        terminal_write(device_id);
        terminal_write("' is not connected\n");
        return -1;
    }
    
    terminal_write("Starting sync operation for device '");
    terminal_write(device_id);
    terminal_write("'...\n");
    
    // Start the sync operation based on its type
    int result = -1;
    
    switch (type) {
        case MOBILE_SYNC_FILES:
            result = file_sync_start(device, direction, source_path, destination_path);
            break;
        
        case MOBILE_SYNC_CONTACTS:
            result = contact_sync_start(device, direction, source_path, destination_path);
            break;
        
        default:
            terminal_write("Error: Unsupported sync type\n");
            break;
    }
    
    if (result == 0) {
        terminal_write("Sync operation started\n");
    } else {
        terminal_write("Error: Failed to start sync operation\n");
    }
    
    return result;
}

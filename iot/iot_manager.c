/**
 * LightOS IoT
 * IoT Manager implementation
 */

#include "iot_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "protocols/mqtt_support.h"
#include "protocols/coap_support.h"
#include "protocols/zigbee_support.h"

// Maximum number of IoT devices
#define MAX_IOT_DEVICES 100

// IoT device array
static iot_device_t* iot_devices[MAX_IOT_DEVICES];
static unsigned int iot_device_count = 0;

// Initialize the IoT manager
void iot_manager_init() {
    terminal_write("Initializing IoT manager...\n");
    
    // Clear the device array
    for (int i = 0; i < MAX_IOT_DEVICES; i++) {
        iot_devices[i] = NULL;
    }
    
    iot_device_count = 0;
    
    // Initialize IoT protocols
    mqtt_init();
    coap_init();
    zigbee_init();
    
    terminal_write("IoT manager initialized\n");
}

// Add an IoT device
int iot_add_device(const char* name, iot_device_type_t type, iot_protocol_t protocol, const char* address, unsigned int port) {
    if (!name || !address) {
        terminal_write("Error: Device name and address cannot be NULL\n");
        return -1;
    }
    
    // Check if a device with the same name already exists
    for (unsigned int i = 0; i < iot_device_count; i++) {
        if (strcmp(iot_devices[i]->name, name) == 0) {
            terminal_write("Error: Device with name '");
            terminal_write(name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Check if we have room for another device
    if (iot_device_count >= MAX_IOT_DEVICES) {
        terminal_write("Error: Maximum number of IoT devices reached\n");
        return -1;
    }
    
    // Allocate memory for the device
    iot_device_t* device = (iot_device_t*)allocate_block();
    
    if (!device) {
        terminal_write("Error: Failed to allocate memory for device\n");
        return -1;
    }
    
    // Initialize the device
    // In a real system, we would generate a unique ID
    strcpy(device->id, "iot-");
    for (int i = 4; i < 12; i++) {
        device->id[i] = '0' + (i % 10);
    }
    device->id[12] = '\0';
    
    strcpy(device->name, name);
    device->type = type;
    device->protocol = protocol;
    device->state = IOT_CONNECTION_DISCONNECTED;
    strcpy(device->address, address);
    device->port = port;
    device->username[0] = '\0';
    device->password[0] = '\0';
    device->client_id[0] = '\0';
    device->secure = 0;
    device->topics = NULL;
    device->topic_count = 0;
    device->private_data = NULL;
    
    // Add the device to the array
    iot_devices[iot_device_count++] = device;
    
    terminal_write("Added IoT device '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Remove an IoT device
int iot_remove_device(const char* name) {
    if (!name) {
        terminal_write("Error: Device name cannot be NULL\n");
        return -1;
    }
    
    // Find the device
    int index = -1;
    for (unsigned int i = 0; i < iot_device_count; i++) {
        if (strcmp(iot_devices[i]->name, name) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: Device '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    iot_device_t* device = iot_devices[index];
    
    // Disconnect the device if it's connected
    if (device->state == IOT_CONNECTION_CONNECTED || device->state == IOT_CONNECTION_CONNECTING) {
        iot_disconnect_device(name);
    }
    
    // Free the device's resources
    if (device->topics) {
        free_blocks(device->topics, (device->topic_count * sizeof(char*) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    // Remove the device from the array
    free_block(device);
    
    for (unsigned int i = index; i < iot_device_count - 1; i++) {
        iot_devices[i] = iot_devices[i + 1];
    }
    
    iot_devices[--iot_device_count] = NULL;
    
    terminal_write("Removed IoT device '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Connect an IoT device
int iot_connect_device(const char* name) {
    iot_device_t* device = iot_get_device(name);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the device is already connected
    if (device->state == IOT_CONNECTION_CONNECTED) {
        terminal_write("Device '");
        terminal_write(name);
        terminal_write("' is already connected\n");
        return 0;
    }
    
    // Connect the device based on its protocol
    int result = -1;
    
    switch (device->protocol) {
        case IOT_PROTOCOL_MQTT:
            result = mqtt_connect(device);
            break;
        
        case IOT_PROTOCOL_COAP:
            result = coap_connect(device);
            break;
        
        case IOT_PROTOCOL_ZIGBEE:
            result = zigbee_connect(device);
            break;
        
        default:
            terminal_write("Error: Unsupported protocol\n");
            break;
    }
    
    if (result == 0) {
        device->state = IOT_CONNECTION_CONNECTED;
        terminal_write("Connected IoT device '");
        terminal_write(name);
        terminal_write("'\n");
    } else {
        device->state = IOT_CONNECTION_ERROR;
        terminal_write("Error: Failed to connect IoT device '");
        terminal_write(name);
        terminal_write("'\n");
    }
    
    return result;
}

// Disconnect an IoT device
int iot_disconnect_device(const char* name) {
    iot_device_t* device = iot_get_device(name);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the device is already disconnected
    if (device->state == IOT_CONNECTION_DISCONNECTED) {
        terminal_write("Device '");
        terminal_write(name);
        terminal_write("' is already disconnected\n");
        return 0;
    }
    
    // Disconnect the device based on its protocol
    int result = -1;
    
    switch (device->protocol) {
        case IOT_PROTOCOL_MQTT:
            result = mqtt_disconnect(device);
            break;
        
        case IOT_PROTOCOL_COAP:
            result = coap_disconnect(device);
            break;
        
        case IOT_PROTOCOL_ZIGBEE:
            result = zigbee_disconnect(device);
            break;
        
        default:
            terminal_write("Error: Unsupported protocol\n");
            break;
    }
    
    if (result == 0) {
        device->state = IOT_CONNECTION_DISCONNECTED;
        terminal_write("Disconnected IoT device '");
        terminal_write(name);
        terminal_write("'\n");
    } else {
        terminal_write("Error: Failed to disconnect IoT device '");
        terminal_write(name);
        terminal_write("'\n");
    }
    
    return result;
}

// Get an IoT device by name
iot_device_t* iot_get_device(const char* name) {
    if (!name) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < iot_device_count; i++) {
        if (strcmp(iot_devices[i]->name, name) == 0) {
            return iot_devices[i];
        }
    }
    
    return NULL;
}

// Get the connection state of an IoT device
iot_connection_state_t iot_get_device_state(const char* name) {
    iot_device_t* device = iot_get_device(name);
    
    if (!device) {
        return IOT_CONNECTION_ERROR;
    }
    
    return device->state;
}

// Set the credentials for an IoT device
int iot_set_device_credentials(const char* name, const char* username, const char* password) {
    iot_device_t* device = iot_get_device(name);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (username) {
        strncpy(device->username, username, sizeof(device->username) - 1);
        device->username[sizeof(device->username) - 1] = '\0';
    } else {
        device->username[0] = '\0';
    }
    
    if (password) {
        strncpy(device->password, password, sizeof(device->password) - 1);
        device->password[sizeof(device->password) - 1] = '\0';
    } else {
        device->password[0] = '\0';
    }
    
    return 0;
}

// Set the client ID for an IoT device
int iot_set_device_client_id(const char* name, const char* client_id) {
    iot_device_t* device = iot_get_device(name);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (client_id) {
        strncpy(device->client_id, client_id, sizeof(device->client_id) - 1);
        device->client_id[sizeof(device->client_id) - 1] = '\0';
    } else {
        device->client_id[0] = '\0';
    }
    
    return 0;
}

// Set the secure flag for an IoT device
int iot_set_device_secure(const char* name, unsigned int secure) {
    iot_device_t* device = iot_get_device(name);
    
    if (!device) {
        terminal_write("Error: Device '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    device->secure = secure ? 1 : 0;
    
    return 0;
}

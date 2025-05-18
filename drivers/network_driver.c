/**
 * LightOS Drivers
 * Network driver implementation
 */

#include "network_driver.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// Maximum number of network drivers
#define MAX_NETWORK_DRIVERS 8

// Network driver array
static network_driver_t* network_drivers[MAX_NETWORK_DRIVERS];
static int network_driver_count = 0;

// Initialize network drivers
void network_driver_init() {
    terminal_write("Initializing network drivers...\n");
    
    // Clear the driver array
    for (int i = 0; i < MAX_NETWORK_DRIVERS; i++) {
        network_drivers[i] = NULL;
    }
    
    network_driver_count = 0;
    
    // Detect network drivers
    detect_ethernet_drivers();
    detect_wifi_drivers();
    
    terminal_write("Network drivers initialized\n");
    terminal_write_color("Found ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert driver_count to string
    char count_str[16];
    int temp = network_driver_count;
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
    terminal_write(" network drivers\n");
}

// Register a network driver
int network_driver_register(network_driver_t* driver) {
    if (!driver) {
        return -1;
    }
    
    // Check if we have room for another driver
    if (network_driver_count >= MAX_NETWORK_DRIVERS) {
        terminal_write("Error: Maximum number of network drivers reached\n");
        return -1;
    }
    
    // Check if a driver with the same name already exists
    for (int i = 0; i < network_driver_count; i++) {
        if (strcmp(network_drivers[i]->name, driver->name) == 0) {
            terminal_write("Error: Network driver with name '");
            terminal_write(driver->name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Allocate memory for the driver
    network_driver_t* new_driver = (network_driver_t*)allocate_block();
    
    if (!new_driver) {
        terminal_write("Error: Failed to allocate memory for network driver\n");
        return -1;
    }
    
    // Copy the driver data
    memcpy(new_driver, driver, sizeof(network_driver_t));
    
    // Add the driver to the array
    network_drivers[network_driver_count++] = new_driver;
    
    terminal_write("Registered network driver: ");
    terminal_write(driver->name);
    terminal_write("\n");
    
    // Initialize the driver if it has an init function
    if (new_driver->init) {
        if (new_driver->init(new_driver) != 0) {
            terminal_write("Warning: Failed to initialize network driver '");
            terminal_write(new_driver->name);
            terminal_write("'\n");
        }
    }
    
    return 0;
}

// Unregister a network driver
int network_driver_unregister(const char* name) {
    if (!name) {
        return -1;
    }
    
    // Find the driver
    for (int i = 0; i < network_driver_count; i++) {
        if (strcmp(network_drivers[i]->name, name) == 0) {
            // Free the driver memory
            free_block(network_drivers[i]);
            
            // Remove the driver from the array by shifting all subsequent drivers
            for (int j = i; j < network_driver_count - 1; j++) {
                network_drivers[j] = network_drivers[j + 1];
            }
            
            network_drivers[--network_driver_count] = NULL;
            
            terminal_write("Unregistered network driver: ");
            terminal_write(name);
            terminal_write("\n");
            
            return 0;
        }
    }
    
    terminal_write("Error: Network driver '");
    terminal_write(name);
    terminal_write("' not found\n");
    
    return -1;
}

// Get a network driver by name
network_driver_t* network_driver_get(const char* name) {
    if (!name) {
        return NULL;
    }
    
    // Find the driver
    for (int i = 0; i < network_driver_count; i++) {
        if (strcmp(network_drivers[i]->name, name) == 0) {
            return network_drivers[i];
        }
    }
    
    return NULL;
}

// Send a packet using a network driver
int network_driver_send_packet(const char* driver_name, const network_packet_t* packet) {
    network_driver_t* driver = network_driver_get(driver_name);
    
    if (!driver) {
        terminal_write("Error: Network driver '");
        terminal_write(driver_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!driver->send_packet) {
        terminal_write("Error: Network driver '");
        terminal_write(driver_name);
        terminal_write("' does not support sending packets\n");
        return -1;
    }
    
    if (driver->link_status == 0) {
        terminal_write("Error: Network interface '");
        terminal_write(driver_name);
        terminal_write("' is down\n");
        return -1;
    }
    
    return driver->send_packet(driver, packet);
}

// Receive a packet using a network driver
int network_driver_receive_packet(const char* driver_name, network_packet_t* packet) {
    network_driver_t* driver = network_driver_get(driver_name);
    
    if (!driver) {
        terminal_write("Error: Network driver '");
        terminal_write(driver_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!driver->receive_packet) {
        terminal_write("Error: Network driver '");
        terminal_write(driver_name);
        terminal_write("' does not support receiving packets\n");
        return -1;
    }
    
    if (driver->link_status == 0) {
        terminal_write("Error: Network interface '");
        terminal_write(driver_name);
        terminal_write("' is down\n");
        return -1;
    }
    
    return driver->receive_packet(driver, packet);
}

// List all network drivers
void network_driver_list() {
    terminal_write("Network Drivers:\n");
    terminal_write("----------------\n");
    
    if (network_driver_count == 0) {
        terminal_write("No network drivers found\n");
        return;
    }
    
    for (int i = 0; i < network_driver_count; i++) {
        network_driver_t* driver = network_drivers[i];
        
        terminal_write(driver->name);
        terminal_write(": ");
        
        // Print link status
        if (driver->link_status) {
            terminal_write("UP");
        } else {
            terminal_write("DOWN");
        }
        
        terminal_write(", ");
        
        // Print MAC address
        terminal_write("MAC: ");
        for (int j = 0; j < 6; j++) {
            // Convert byte to hex
            char hex[3];
            unsigned char byte = driver->mac_address[j];
            
            hex[0] = (byte >> 4) < 10 ? '0' + (byte >> 4) : 'A' + (byte >> 4) - 10;
            hex[1] = (byte & 0xF) < 10 ? '0' + (byte & 0xF) : 'A' + (byte & 0xF) - 10;
            hex[2] = '\0';
            
            terminal_write(hex);
            
            if (j < 5) {
                terminal_write(":");
            }
        }
        
        terminal_write(", ");
        
        // Print link speed
        char speed_str[16];
        int idx = 0;
        unsigned int speed = driver->link_speed;
        
        if (speed == 0) {
            speed_str[idx++] = '0';
        } else {
            while (speed > 0) {
                speed_str[idx++] = '0' + (speed % 10);
                speed /= 10;
            }
        }
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = speed_str[j];
            speed_str[j] = speed_str[idx - j - 1];
            speed_str[idx - j - 1] = tmp;
        }
        
        speed_str[idx] = '\0';
        terminal_write(speed_str);
        terminal_write(" Mbps");
        
        terminal_write("\n");
    }
}

// Allocate a network packet
network_packet_t* network_packet_allocate(unsigned int length) {
    // Allocate memory for the packet structure
    network_packet_t* packet = (network_packet_t*)allocate_block();
    
    if (!packet) {
        return NULL;
    }
    
    // Allocate memory for the packet data
    unsigned int blocks_needed = (length + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE;
    packet->data = (unsigned char*)allocate_blocks(blocks_needed);
    
    if (!packet->data) {
        free_block(packet);
        return NULL;
    }
    
    packet->length = length;
    packet->allocated_length = blocks_needed * MEMORY_BLOCK_SIZE;
    
    return packet;
}

// Free a network packet
void network_packet_free(network_packet_t* packet) {
    if (!packet) {
        return;
    }
    
    if (packet->data) {
        unsigned int blocks_used = (packet->allocated_length + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE;
        free_blocks(packet->data, blocks_used);
    }
    
    free_block(packet);
}

// Resize a network packet
int network_packet_resize(network_packet_t* packet, unsigned int new_length) {
    if (!packet) {
        return -1;
    }
    
    // If the new length fits in the already allocated memory, just update the length
    if (new_length <= packet->allocated_length) {
        packet->length = new_length;
        return 0;
    }
    
    // Allocate a new buffer
    unsigned int blocks_needed = (new_length + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE;
    unsigned char* new_data = (unsigned char*)allocate_blocks(blocks_needed);
    
    if (!new_data) {
        return -1;
    }
    
    // Copy the old data
    if (packet->data) {
        memcpy(new_data, packet->data, packet->length);
        
        // Free the old buffer
        unsigned int blocks_used = (packet->allocated_length + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE;
        free_blocks(packet->data, blocks_used);
    }
    
    // Update the packet
    packet->data = new_data;
    packet->length = new_length;
    packet->allocated_length = blocks_needed * MEMORY_BLOCK_SIZE;
    
    return 0;
}

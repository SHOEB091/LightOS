/**
 * LightOS Networking
 * Basic networking implementation
 */

#include "network.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"

// Maximum number of network interfaces
#define MAX_NETWORK_INTERFACES 8

// Network interface table
static network_interface_t network_interfaces[MAX_NETWORK_INTERFACES];
static int num_interfaces = 0;

// Initialize networking
void network_init() {
    // Clear the network interface table
    for (int i = 0; i < MAX_NETWORK_INTERFACES; i++) {
        network_interfaces[i].flags = 0;
        network_interfaces[i].name[0] = '\0';
        network_interfaces[i].ip_addr = 0;
        network_interfaces[i].netmask = 0;
        network_interfaces[i].gateway = 0;
        network_interfaces[i].mac_addr[0] = 0;
        network_interfaces[i].mac_addr[1] = 0;
        network_interfaces[i].mac_addr[2] = 0;
        network_interfaces[i].mac_addr[3] = 0;
        network_interfaces[i].mac_addr[4] = 0;
        network_interfaces[i].mac_addr[5] = 0;
        network_interfaces[i].send = NULL;
        network_interfaces[i].recv = NULL;
    }
    
    num_interfaces = 0;
}

// Register a network interface
int network_register_interface(const char* name, unsigned char* mac_addr, 
                              network_send_func_t send_func, 
                              network_recv_func_t recv_func) {
    if (num_interfaces >= MAX_NETWORK_INTERFACES) {
        return -1; // Too many interfaces
    }
    
    int idx = num_interfaces++;
    
    // Copy the interface name
    int i;
    for (i = 0; name[i] && i < 15; i++) {
        network_interfaces[idx].name[i] = name[i];
    }
    network_interfaces[idx].name[i] = '\0';
    
    // Copy the MAC address
    for (i = 0; i < 6; i++) {
        network_interfaces[idx].mac_addr[i] = mac_addr[i];
    }
    
    // Set the function pointers
    network_interfaces[idx].send = send_func;
    network_interfaces[idx].recv = recv_func;
    
    // Initialize other fields
    network_interfaces[idx].flags = NETWORK_INTERFACE_DOWN;
    network_interfaces[idx].ip_addr = 0;
    network_interfaces[idx].netmask = 0;
    network_interfaces[idx].gateway = 0;
    
    return idx;
}

// Configure a network interface
int network_configure_interface(int interface_idx, unsigned int ip_addr, 
                               unsigned int netmask, unsigned int gateway) {
    if (interface_idx < 0 || interface_idx >= num_interfaces) {
        return -1; // Invalid interface
    }
    
    network_interfaces[interface_idx].ip_addr = ip_addr;
    network_interfaces[interface_idx].netmask = netmask;
    network_interfaces[interface_idx].gateway = gateway;
    
    return 0;
}

// Bring up a network interface
int network_interface_up(int interface_idx) {
    if (interface_idx < 0 || interface_idx >= num_interfaces) {
        return -1; // Invalid interface
    }
    
    network_interfaces[interface_idx].flags |= NETWORK_INTERFACE_UP;
    network_interfaces[interface_idx].flags &= ~NETWORK_INTERFACE_DOWN;
    
    return 0;
}

// Bring down a network interface
int network_interface_down(int interface_idx) {
    if (interface_idx < 0 || interface_idx >= num_interfaces) {
        return -1; // Invalid interface
    }
    
    network_interfaces[interface_idx].flags &= ~NETWORK_INTERFACE_UP;
    network_interfaces[interface_idx].flags |= NETWORK_INTERFACE_DOWN;
    
    return 0;
}

// Send a packet through a network interface
int network_send_packet(int interface_idx, unsigned char* data, unsigned int length) {
    if (interface_idx < 0 || interface_idx >= num_interfaces) {
        return -1; // Invalid interface
    }
    
    if (!(network_interfaces[interface_idx].flags & NETWORK_INTERFACE_UP)) {
        return -1; // Interface is down
    }
    
    if (!network_interfaces[interface_idx].send) {
        return -1; // No send function
    }
    
    return network_interfaces[interface_idx].send(data, length);
}

// Receive a packet from a network interface
int network_recv_packet(int interface_idx, unsigned char* buffer, unsigned int max_length) {
    if (interface_idx < 0 || interface_idx >= num_interfaces) {
        return -1; // Invalid interface
    }
    
    if (!(network_interfaces[interface_idx].flags & NETWORK_INTERFACE_UP)) {
        return -1; // Interface is down
    }
    
    if (!network_interfaces[interface_idx].recv) {
        return -1; // No receive function
    }
    
    return network_interfaces[interface_idx].recv(buffer, max_length);
}

// Find a network interface by name
int network_find_interface(const char* name) {
    for (int i = 0; i < num_interfaces; i++) {
        int j;
        for (j = 0; name[j] && network_interfaces[i].name[j]; j++) {
            if (name[j] != network_interfaces[i].name[j]) {
                break;
            }
        }
        
        if (name[j] == '\0' && network_interfaces[i].name[j] == '\0') {
            return i; // Found
        }
    }
    
    return -1; // Not found
}

// Print information about all network interfaces
void network_print_interfaces() {
    terminal_write("Network Interfaces:\n");
    terminal_write("-------------------\n");
    
    for (int i = 0; i < num_interfaces; i++) {
        terminal_write(network_interfaces[i].name);
        terminal_write(": ");
        
        // Print status
        if (network_interfaces[i].flags & NETWORK_INTERFACE_UP) {
            terminal_write("UP");
        } else {
            terminal_write("DOWN");
        }
        
        terminal_write("\n");
        
        // Print MAC address
        terminal_write("  MAC: ");
        // In a real system, we would format the MAC address here
        terminal_write("\n");
        
        // Print IP configuration
        terminal_write("  IP: ");
        // In a real system, we would format the IP address here
        terminal_write("\n");
        
        terminal_write("  Netmask: ");
        // In a real system, we would format the netmask here
        terminal_write("\n");
        
        terminal_write("  Gateway: ");
        // In a real system, we would format the gateway here
        terminal_write("\n");
        
        terminal_write("\n");
    }
}

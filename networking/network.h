/**
 * LightOS Networking
 * Networking header
 */

#ifndef NETWORK_H
#define NETWORK_H

// Network interface flags
#define NETWORK_INTERFACE_UP    0x01
#define NETWORK_INTERFACE_DOWN  0x02
#define NETWORK_INTERFACE_DHCP  0x04

// Function types for network operations
typedef int (*network_send_func_t)(unsigned char* data, unsigned int length);
typedef int (*network_recv_func_t)(unsigned char* buffer, unsigned int max_length);

// Network interface structure
typedef struct {
    char name[16];
    unsigned int flags;
    unsigned int ip_addr;
    unsigned int netmask;
    unsigned int gateway;
    unsigned char mac_addr[6];
    network_send_func_t send;
    network_recv_func_t recv;
} network_interface_t;

// Networking functions
void network_init();
int network_register_interface(const char* name, unsigned char* mac_addr, 
                              network_send_func_t send_func, 
                              network_recv_func_t recv_func);
int network_configure_interface(int interface_idx, unsigned int ip_addr, 
                               unsigned int netmask, unsigned int gateway);
int network_interface_up(int interface_idx);
int network_interface_down(int interface_idx);
int network_send_packet(int interface_idx, unsigned char* data, unsigned int length);
int network_recv_packet(int interface_idx, unsigned char* buffer, unsigned int max_length);
int network_find_interface(const char* name);
void network_print_interfaces();

#endif /* NETWORK_H */

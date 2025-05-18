/**
 * LightOS Drivers
 * Network driver header
 */

#ifndef NETWORK_DRIVER_H
#define NETWORK_DRIVER_H

// Network packet structure
typedef struct {
    unsigned char* data;
    unsigned int length;
    unsigned int allocated_length;
} network_packet_t;

// Network driver structure
typedef struct {
    char name[32];
    unsigned char mac_address[6];
    unsigned int ip_address;
    unsigned int subnet_mask;
    unsigned int gateway;
    unsigned int mtu;
    unsigned int link_speed;      // In Mbps
    unsigned int link_status;     // 0 = down, 1 = up
    
    // Function pointers for driver operations
    int (*init)(struct network_driver* driver);
    int (*send_packet)(struct network_driver* driver, const network_packet_t* packet);
    int (*receive_packet)(struct network_driver* driver, network_packet_t* packet);
    int (*set_mac_address)(struct network_driver* driver, const unsigned char* mac_address);
    int (*get_link_status)(struct network_driver* driver);
    
    // Private data for the driver
    void* private_data;
} network_driver_t;

// Network driver functions
void network_driver_init();
int network_driver_register(network_driver_t* driver);
int network_driver_unregister(const char* name);
network_driver_t* network_driver_get(const char* name);
int network_driver_send_packet(const char* driver_name, const network_packet_t* packet);
int network_driver_receive_packet(const char* driver_name, network_packet_t* packet);
void network_driver_list();

// Packet management functions
network_packet_t* network_packet_allocate(unsigned int length);
void network_packet_free(network_packet_t* packet);
int network_packet_resize(network_packet_t* packet, unsigned int new_length);

// Specific network driver detection
int detect_ethernet_drivers();
int detect_wifi_drivers();

#endif /* NETWORK_DRIVER_H */

# LightOS Networking

## Overview

LightOS provides a comprehensive networking stack that supports various network protocols and interfaces. The networking system is designed to be modular, allowing for easy addition of new protocols and interfaces.

## Network Stack Architecture

The LightOS network stack follows a layered architecture similar to the OSI model:

1. **Physical Layer**: Handled by network device drivers.
2. **Data Link Layer**: Ethernet, WiFi, and other link layer protocols.
3. **Network Layer**: IP (IPv4 and IPv6), ICMP, and other network layer protocols.
4. **Transport Layer**: TCP, UDP, and other transport layer protocols.
5. **Session Layer**: Implemented by applications.
6. **Presentation Layer**: Implemented by applications.
7. **Application Layer**: HTTP, FTP, SMTP, and other application layer protocols.

## Network Interface Management

### Key Features

- **Interface Registration**: Registers network interfaces with the network stack.
- **Interface Configuration**: Configures network interfaces with IP addresses, netmasks, and gateways.
- **Interface Status**: Monitors and controls interface status (up/down).
- **Packet Transmission**: Sends packets through network interfaces.
- **Packet Reception**: Receives packets from network interfaces.

### API Reference

#### Initialization

```c
void network_init();
```

Initializes the network stack.

#### Interface Registration

```c
int network_register_interface(const char* name, unsigned char* mac_addr, 
                              network_send_func_t send_func, 
                              network_recv_func_t recv_func);
```

Registers a network interface with the network stack.

#### Interface Configuration

```c
int network_configure_interface(int interface_idx, unsigned int ip_addr, 
                               unsigned int netmask, unsigned int gateway);
```

Configures a network interface with an IP address, netmask, and gateway.

#### Interface Control

```c
int network_interface_up(int interface_idx);
int network_interface_down(int interface_idx);
```

Brings a network interface up or down.

#### Packet Transmission and Reception

```c
int network_send_packet(int interface_idx, unsigned char* data, unsigned int length);
int network_recv_packet(int interface_idx, unsigned char* buffer, unsigned int max_length);
```

Sends or receives a packet through a network interface.

#### Interface Information

```c
int network_find_interface(const char* name);
void network_print_interfaces();
```

Finds a network interface by name or prints information about all interfaces.

## TCP Protocol Implementation

### Key Features

- **Connection Management**: Establishes, maintains, and terminates TCP connections.
- **Flow Control**: Controls the flow of data to prevent overwhelming receivers.
- **Congestion Control**: Prevents network congestion.
- **Reliable Delivery**: Ensures reliable delivery of data.
- **Ordered Delivery**: Ensures ordered delivery of data.

### API Reference

#### Initialization

```c
void tcp_init();
```

Initializes the TCP protocol.

#### Socket Management

```c
int tcp_socket_create();
int tcp_socket_bind(int socket_id, unsigned int ip, unsigned short port);
int tcp_socket_listen(int socket_id, int backlog);
int tcp_socket_accept(int socket_id, unsigned int* remote_ip, unsigned short* remote_port);
int tcp_socket_connect(int socket_id, unsigned int ip, unsigned short port);
int tcp_socket_close(int socket_id);
```

Creates, binds, listens, accepts, connects, and closes TCP sockets.

#### Data Transmission and Reception

```c
int tcp_socket_send(int socket_id, const void* data, unsigned int length);
int tcp_socket_recv(int socket_id, void* buffer, unsigned int length);
```

Sends or receives data through a TCP socket.

#### Socket Options

```c
int tcp_socket_set_option(int socket_id, int option, const void* value, unsigned int length);
int tcp_socket_get_option(int socket_id, int option, void* value, unsigned int* length);
```

Sets or gets socket options.

#### Event Callbacks

```c
int tcp_socket_set_callbacks(int socket_id, int (*on_connect)(tcp_socket_t*), 
                            int (*on_data)(tcp_socket_t*, const void*, unsigned int), 
                            int (*on_close)(tcp_socket_t*));
```

Sets callbacks for socket events.

#### Packet Handling

```c
void tcp_handle_packet(const unsigned char* packet, unsigned int length, 
                      unsigned int src_ip, unsigned int dst_ip);
```

Handles a TCP packet.

## UDP Protocol Implementation

### Key Features

- **Connectionless Communication**: Provides connectionless communication.
- **Unreliable Delivery**: Does not guarantee delivery of data.
- **No Flow Control**: Does not provide flow control.
- **No Congestion Control**: Does not provide congestion control.
- **No Ordered Delivery**: Does not guarantee ordered delivery of data.

### API Reference

Similar to the TCP API, but with connectionless semantics.

## HTTP Server Implementation

### Key Features

- **Request Handling**: Handles HTTP requests.
- **Response Generation**: Generates HTTP responses.
- **Static File Serving**: Serves static files.
- **Dynamic Content Generation**: Generates dynamic content.
- **Virtual Hosting**: Supports multiple virtual hosts.

### API Reference

#### Initialization

```c
void http_server_init();
```

Initializes the HTTP server.

#### Server Configuration

```c
void http_server_configure(http_server_config_t* config);
```

Configures the HTTP server.

#### Server Control

```c
int http_server_start();
void http_server_stop();
int http_server_is_running();
```

Starts, stops, or checks if the HTTP server is running.

#### Request Handling

```c
void http_server_handle_request(http_request_t* request, http_response_t* response);
```

Handles an HTTP request and generates a response.

## FTP Server Implementation

### Key Features

- **File Transfer**: Transfers files between client and server.
- **Directory Listing**: Lists directory contents.
- **File Management**: Creates, deletes, and renames files and directories.
- **Authentication**: Authenticates users.
- **Access Control**: Controls access to files and directories.

### API Reference

Similar to the HTTP server API, but with FTP-specific functionality.

## DNS Client Implementation

### Key Features

- **Name Resolution**: Resolves domain names to IP addresses.
- **Caching**: Caches DNS responses.
- **Multiple DNS Servers**: Supports multiple DNS servers.
- **Timeout and Retry**: Handles timeouts and retries.

### API Reference

#### Initialization

```c
void dns_client_init();
```

Initializes the DNS client.

#### Name Resolution

```c
int dns_resolve(const char* hostname, unsigned int* ip_addr);
```

Resolves a hostname to an IP address.

#### Cache Management

```c
void dns_flush_cache();
```

Flushes the DNS cache.

## Network Security

### Key Features

- **Firewall**: Filters network traffic based on rules.
- **Encryption**: Encrypts network traffic.
- **Authentication**: Authenticates network users.
- **Access Control**: Controls access to network resources.
- **Intrusion Detection**: Detects and prevents network intrusions.

### API Reference

#### Firewall

```c
void firewall_init();
int firewall_add_rule(firewall_rule_t* rule);
int firewall_remove_rule(int rule_id);
int firewall_check_packet(const unsigned char* packet, unsigned int length);
```

Initializes the firewall, adds or removes rules, and checks packets against rules.

#### Encryption

```c
void encryption_init();
int encryption_encrypt(const void* data, unsigned int length, void* encrypted_data, unsigned int* encrypted_length);
int encryption_decrypt(const void* encrypted_data, unsigned int encrypted_length, void* data, unsigned int* length);
```

Initializes the encryption system and encrypts or decrypts data.

## Network Utilities

### Key Features

- **Ping**: Tests network connectivity.
- **Traceroute**: Traces the route to a host.
- **Network Diagnostics**: Diagnoses network problems.
- **Bandwidth Monitoring**: Monitors network bandwidth usage.
- **Network Statistics**: Collects and displays network statistics.

### API Reference

#### Ping

```c
int ping(const char* hostname, unsigned int count, unsigned int timeout);
```

Pings a host.

#### Traceroute

```c
int traceroute(const char* hostname, unsigned int max_hops, unsigned int timeout);
```

Traces the route to a host.

#### Network Diagnostics

```c
void network_diagnostics();
```

Runs network diagnostics.

#### Bandwidth Monitoring

```c
void bandwidth_monitor_start();
void bandwidth_monitor_stop();
void bandwidth_monitor_print_stats();
```

Starts, stops, or prints statistics from the bandwidth monitor.

## Writing Custom Network Protocols

### Protocol Structure

```c
typedef struct {
    char name[32];
    unsigned int protocol_number;
    int (*init)();
    int (*handle_packet)(const unsigned char* packet, unsigned int length);
    int (*send_packet)(const void* data, unsigned int length, unsigned int dst_ip);
} network_protocol_t;
```

### Protocol Implementation Steps

1. **Create a Protocol Structure**: Define a protocol structure with the necessary information.
2. **Implement Protocol Functions**: Implement the required protocol functions (init, handle_packet, send_packet).
3. **Register the Protocol**: Register the protocol with the network stack.
4. **Handle Protocol Events**: Handle protocol events such as packet reception and transmission.

### Example Protocol

```c
// Example protocol for a fictional protocol
network_protocol_t example_protocol = {
    .name = "EXAMPLE",
    .protocol_number = 253, // Use a number not assigned by IANA
    .init = example_protocol_init,
    .handle_packet = example_protocol_handle_packet,
    .send_packet = example_protocol_send_packet
};

int example_protocol_init() {
    // Initialize the protocol
    terminal_write("Initializing example protocol\n");
    
    return 0;
}

int example_protocol_handle_packet(const unsigned char* packet, unsigned int length) {
    // Handle a packet
    terminal_write("Handling example protocol packet\n");
    
    return 0;
}

int example_protocol_send_packet(const void* data, unsigned int length, unsigned int dst_ip) {
    // Send a packet
    terminal_write("Sending example protocol packet\n");
    
    return 0;
}

// Register the protocol
void example_protocol_register() {
    network_register_protocol(&example_protocol);
}
```

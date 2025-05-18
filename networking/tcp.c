/**
 * LightOS Networking
 * TCP protocol implementation
 */

#include "tcp.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// TCP header structure
typedef struct {
    unsigned short src_port;
    unsigned short dst_port;
    unsigned int seq_num;
    unsigned int ack_num;
    unsigned char data_offset;
    unsigned char flags;
    unsigned short window;
    unsigned short checksum;
    unsigned short urgent_ptr;
    // Options may follow
} tcp_header_t;

// TCP flags
#define TCP_FIN  0x01
#define TCP_SYN  0x02
#define TCP_RST  0x04
#define TCP_PSH  0x08
#define TCP_ACK  0x10
#define TCP_URG  0x20
#define TCP_ECE  0x40
#define TCP_CWR  0x80

// Maximum number of TCP sockets
#define MAX_TCP_SOCKETS 128

// TCP socket array
static tcp_socket_t tcp_sockets[MAX_TCP_SOCKETS];
static int tcp_socket_count = 0;

// Next socket ID
static int next_socket_id = 1;

// Initialize TCP
void tcp_init() {
    terminal_write("Initializing TCP...\n");
    
    // Clear the socket array
    for (int i = 0; i < MAX_TCP_SOCKETS; i++) {
        tcp_sockets[i].socket_id = 0;
        tcp_sockets[i].state = TCP_STATE_CLOSED;
    }
    
    tcp_socket_count = 0;
    next_socket_id = 1;
    
    terminal_write("TCP initialized\n");
}

// Create a TCP socket
int tcp_socket_create() {
    // Check if we have room for another socket
    if (tcp_socket_count >= MAX_TCP_SOCKETS) {
        terminal_write("Error: Maximum number of TCP sockets reached\n");
        return -1;
    }
    
    // Find a free slot
    int index = -1;
    for (int i = 0; i < MAX_TCP_SOCKETS; i++) {
        if (tcp_sockets[i].socket_id == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: No free TCP socket slots\n");
        return -1;
    }
    
    // Initialize the socket
    tcp_sockets[index].socket_id = next_socket_id++;
    tcp_sockets[index].local_ip = 0;
    tcp_sockets[index].local_port = 0;
    tcp_sockets[index].remote_ip = 0;
    tcp_sockets[index].remote_port = 0;
    tcp_sockets[index].state = TCP_STATE_CLOSED;
    tcp_sockets[index].send_seq = 0;
    tcp_sockets[index].recv_seq = 0;
    tcp_sockets[index].send_window = 8192;
    tcp_sockets[index].recv_window = 8192;
    tcp_sockets[index].rtt = 100; // 100 ms
    tcp_sockets[index].rto = 200; // 200 ms
    tcp_sockets[index].send_buffer = NULL;
    tcp_sockets[index].send_buffer_size = 0;
    tcp_sockets[index].recv_buffer = NULL;
    tcp_sockets[index].recv_buffer_size = 0;
    tcp_sockets[index].on_connect = NULL;
    tcp_sockets[index].on_data = NULL;
    tcp_sockets[index].on_close = NULL;
    
    tcp_socket_count++;
    
    return tcp_sockets[index].socket_id;
}

// Find a TCP socket by ID
static tcp_socket_t* tcp_find_socket(int socket_id) {
    for (int i = 0; i < MAX_TCP_SOCKETS; i++) {
        if (tcp_sockets[i].socket_id == socket_id) {
            return &tcp_sockets[i];
        }
    }
    
    return NULL;
}

// Find a TCP socket by local and remote addresses
static tcp_socket_t* tcp_find_socket_by_addr(unsigned int local_ip, unsigned short local_port, unsigned int remote_ip, unsigned short remote_port) {
    for (int i = 0; i < MAX_TCP_SOCKETS; i++) {
        if (tcp_sockets[i].socket_id != 0 &&
            tcp_sockets[i].local_ip == local_ip &&
            tcp_sockets[i].local_port == local_port &&
            tcp_sockets[i].remote_ip == remote_ip &&
            tcp_sockets[i].remote_port == remote_port) {
            return &tcp_sockets[i];
        }
    }
    
    return NULL;
}

// Find a listening TCP socket
static tcp_socket_t* tcp_find_listening_socket(unsigned int local_ip, unsigned short local_port) {
    for (int i = 0; i < MAX_TCP_SOCKETS; i++) {
        if (tcp_sockets[i].socket_id != 0 &&
            tcp_sockets[i].state == TCP_STATE_LISTEN &&
            (tcp_sockets[i].local_ip == 0 || tcp_sockets[i].local_ip == local_ip) &&
            tcp_sockets[i].local_port == local_port) {
            return &tcp_sockets[i];
        }
    }
    
    return NULL;
}

// Bind a TCP socket to a local address
int tcp_socket_bind(int socket_id, unsigned int ip, unsigned short port) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state != TCP_STATE_CLOSED) {
        terminal_write("Error: TCP socket is not in CLOSED state\n");
        return -1;
    }
    
    // Check if the address is already in use
    for (int i = 0; i < MAX_TCP_SOCKETS; i++) {
        if (tcp_sockets[i].socket_id != 0 &&
            tcp_sockets[i].socket_id != socket_id &&
            (tcp_sockets[i].local_ip == 0 || tcp_sockets[i].local_ip == ip) &&
            tcp_sockets[i].local_port == port) {
            terminal_write("Error: TCP address already in use\n");
            return -1;
        }
    }
    
    socket->local_ip = ip;
    socket->local_port = port;
    
    return 0;
}

// Listen for incoming connections
int tcp_socket_listen(int socket_id, int backlog) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state != TCP_STATE_CLOSED) {
        terminal_write("Error: TCP socket is not in CLOSED state\n");
        return -1;
    }
    
    if (socket->local_port == 0) {
        terminal_write("Error: TCP socket is not bound\n");
        return -1;
    }
    
    socket->state = TCP_STATE_LISTEN;
    
    return 0;
}

// Accept an incoming connection
int tcp_socket_accept(int socket_id, unsigned int* remote_ip, unsigned short* remote_port) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state != TCP_STATE_LISTEN) {
        terminal_write("Error: TCP socket is not in LISTEN state\n");
        return -1;
    }
    
    // In a real system, we would:
    // 1. Check if there are any pending connections
    // 2. Create a new socket for the connection
    // 3. Return the new socket ID
    
    // For now, just simulate no pending connections
    return -1;
}

// Connect to a remote host
int tcp_socket_connect(int socket_id, unsigned int ip, unsigned short port) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state != TCP_STATE_CLOSED) {
        terminal_write("Error: TCP socket is not in CLOSED state\n");
        return -1;
    }
    
    // If the socket is not bound, bind it to a random port
    if (socket->local_port == 0) {
        socket->local_ip = 0; // Any local IP
        socket->local_port = 1024 + (next_socket_id % 64511); // Random port between 1024 and 65535
    }
    
    socket->remote_ip = ip;
    socket->remote_port = port;
    
    // In a real system, we would:
    // 1. Send a SYN packet
    // 2. Wait for a SYN-ACK packet
    // 3. Send an ACK packet
    
    // For now, just simulate a successful connection
    socket->state = TCP_STATE_ESTABLISHED;
    
    // Call the on_connect callback if set
    if (socket->on_connect) {
        socket->on_connect(socket);
    }
    
    return 0;
}

// Send data
int tcp_socket_send(int socket_id, const void* data, unsigned int length) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state != TCP_STATE_ESTABLISHED) {
        terminal_write("Error: TCP socket is not in ESTABLISHED state\n");
        return -1;
    }
    
    // In a real system, we would:
    // 1. Add the data to the send buffer
    // 2. Send as many packets as allowed by the window
    // 3. Wait for ACKs
    
    // For now, just simulate a successful send
    return length;
}

// Receive data
int tcp_socket_recv(int socket_id, void* buffer, unsigned int length) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state != TCP_STATE_ESTABLISHED) {
        terminal_write("Error: TCP socket is not in ESTABLISHED state\n");
        return -1;
    }
    
    // In a real system, we would:
    // 1. Check if there is data in the receive buffer
    // 2. Copy the data to the provided buffer
    // 3. Update the receive window
    
    // For now, just simulate no data available
    return 0;
}

// Close a socket
int tcp_socket_close(int socket_id) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    if (socket->state == TCP_STATE_CLOSED) {
        // Socket is already closed
        return 0;
    }
    
    // In a real system, we would:
    // 1. Send a FIN packet if the socket is connected
    // 2. Wait for a FIN-ACK
    // 3. Send an ACK
    
    // Call the on_close callback if set
    if (socket->on_close) {
        socket->on_close(socket);
    }
    
    // Free any allocated buffers
    if (socket->send_buffer) {
        free_block(socket->send_buffer);
        socket->send_buffer = NULL;
    }
    
    if (socket->recv_buffer) {
        free_block(socket->recv_buffer);
        socket->recv_buffer = NULL;
    }
    
    // Reset the socket
    socket->socket_id = 0;
    socket->state = TCP_STATE_CLOSED;
    tcp_socket_count--;
    
    return 0;
}

// Set socket options
int tcp_socket_set_option(int socket_id, int option, const void* value, unsigned int length) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    // In a real system, we would handle various socket options
    // For now, just simulate success
    return 0;
}

// Get socket options
int tcp_socket_get_option(int socket_id, int option, void* value, unsigned int* length) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    // In a real system, we would handle various socket options
    // For now, just simulate success
    return 0;
}

// Set socket callbacks
int tcp_socket_set_callbacks(int socket_id, int (*on_connect)(tcp_socket_t*), int (*on_data)(tcp_socket_t*, const void*, unsigned int), int (*on_close)(tcp_socket_t*)) {
    tcp_socket_t* socket = tcp_find_socket(socket_id);
    
    if (!socket) {
        terminal_write("Error: Invalid TCP socket ID\n");
        return -1;
    }
    
    socket->on_connect = on_connect;
    socket->on_data = on_data;
    socket->on_close = on_close;
    
    return 0;
}

// Handle a TCP packet
void tcp_handle_packet(const unsigned char* packet, unsigned int length, unsigned int src_ip, unsigned int dst_ip) {
    if (length < sizeof(tcp_header_t)) {
        // Packet is too small to be a valid TCP packet
        return;
    }
    
    tcp_header_t* header = (tcp_header_t*)packet;
    unsigned short src_port = header->src_port;
    unsigned short dst_port = header->dst_port;
    
    // Find the socket for this packet
    tcp_socket_t* socket = tcp_find_socket_by_addr(dst_ip, dst_port, src_ip, src_port);
    
    if (!socket) {
        // Check if this is a SYN packet for a listening socket
        if (header->flags & TCP_SYN) {
            socket = tcp_find_listening_socket(dst_ip, dst_port);
            
            if (socket) {
                // In a real system, we would:
                // 1. Create a new socket for the connection
                // 2. Send a SYN-ACK packet
                // 3. Wait for an ACK
            }
        }
        
        return;
    }
    
    // Handle the packet based on the socket state and packet flags
    switch (socket->state) {
        case TCP_STATE_LISTEN:
            // Handle incoming connections
            break;
        
        case TCP_STATE_SYN_SENT:
            // Handle SYN-ACK packets
            break;
        
        case TCP_STATE_SYN_RECEIVED:
            // Handle ACK packets
            break;
        
        case TCP_STATE_ESTABLISHED:
            // Handle data packets
            if (header->flags & TCP_PSH) {
                unsigned int data_offset = (header->data_offset >> 4) * 4;
                unsigned int data_length = length - data_offset;
                
                if (data_length > 0 && socket->on_data) {
                    socket->on_data(socket, packet + data_offset, data_length);
                }
            }
            
            // Handle FIN packets
            if (header->flags & TCP_FIN) {
                // In a real system, we would:
                // 1. Send an ACK
                // 2. Change the state to CLOSE_WAIT
                // 3. Notify the application
            }
            break;
        
        // Handle other states...
        
        default:
            break;
    }
}

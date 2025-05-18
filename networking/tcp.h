/**
 * LightOS Networking
 * TCP protocol header
 */

#ifndef TCP_H
#define TCP_H

#include "network.h"

// TCP connection states
typedef enum {
    TCP_STATE_CLOSED,
    TCP_STATE_LISTEN,
    TCP_STATE_SYN_SENT,
    TCP_STATE_SYN_RECEIVED,
    TCP_STATE_ESTABLISHED,
    TCP_STATE_FIN_WAIT_1,
    TCP_STATE_FIN_WAIT_2,
    TCP_STATE_CLOSE_WAIT,
    TCP_STATE_CLOSING,
    TCP_STATE_LAST_ACK,
    TCP_STATE_TIME_WAIT
} tcp_state_t;

// TCP socket structure
typedef struct {
    int socket_id;
    unsigned int local_ip;
    unsigned short local_port;
    unsigned int remote_ip;
    unsigned short remote_port;
    tcp_state_t state;
    unsigned int send_seq;
    unsigned int recv_seq;
    unsigned int send_window;
    unsigned int recv_window;
    unsigned int rtt;
    unsigned int rto;
    void* send_buffer;
    unsigned int send_buffer_size;
    void* recv_buffer;
    unsigned int recv_buffer_size;
    int (*on_connect)(struct tcp_socket* socket);
    int (*on_data)(struct tcp_socket* socket, const void* data, unsigned int length);
    int (*on_close)(struct tcp_socket* socket);
} tcp_socket_t;

// TCP functions
void tcp_init();
int tcp_socket_create();
int tcp_socket_bind(int socket_id, unsigned int ip, unsigned short port);
int tcp_socket_listen(int socket_id, int backlog);
int tcp_socket_accept(int socket_id, unsigned int* remote_ip, unsigned short* remote_port);
int tcp_socket_connect(int socket_id, unsigned int ip, unsigned short port);
int tcp_socket_send(int socket_id, const void* data, unsigned int length);
int tcp_socket_recv(int socket_id, void* buffer, unsigned int length);
int tcp_socket_close(int socket_id);
int tcp_socket_set_option(int socket_id, int option, const void* value, unsigned int length);
int tcp_socket_get_option(int socket_id, int option, void* value, unsigned int* length);
int tcp_socket_set_callbacks(int socket_id, int (*on_connect)(tcp_socket_t*), int (*on_data)(tcp_socket_t*, const void*, unsigned int), int (*on_close)(tcp_socket_t*));
void tcp_handle_packet(const unsigned char* packet, unsigned int length, unsigned int src_ip, unsigned int dst_ip);

#endif /* TCP_H */

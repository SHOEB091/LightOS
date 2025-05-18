/**
 * LightOS Networking
 * DNS protocol header
 */

#ifndef DNS_H
#define DNS_H

#include "network.h"
#include "udp.h"

// DNS record types
typedef enum {
    DNS_TYPE_A = 1,        // IPv4 address
    DNS_TYPE_NS = 2,       // Name server
    DNS_TYPE_CNAME = 5,    // Canonical name
    DNS_TYPE_SOA = 6,      // Start of authority
    DNS_TYPE_PTR = 12,     // Pointer
    DNS_TYPE_MX = 15,      // Mail exchange
    DNS_TYPE_TXT = 16,     // Text
    DNS_TYPE_AAAA = 28,    // IPv6 address
    DNS_TYPE_SRV = 33,     // Service
    DNS_TYPE_ANY = 255     // Any record type
} dns_type_t;

// DNS record classes
typedef enum {
    DNS_CLASS_IN = 1,      // Internet
    DNS_CLASS_CS = 2,      // CSNET
    DNS_CLASS_CH = 3,      // CHAOS
    DNS_CLASS_HS = 4,      // Hesiod
    DNS_CLASS_ANY = 255    // Any class
} dns_class_t;

// DNS header structure
typedef struct {
    unsigned short id;
    unsigned short flags;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
} dns_header_t;

// DNS question structure
typedef struct {
    char* qname;
    unsigned short qtype;
    unsigned short qclass;
} dns_question_t;

// DNS resource record structure
typedef struct {
    char* name;
    unsigned short type;
    unsigned short class;
    unsigned int ttl;
    unsigned short rdlength;
    void* rdata;
} dns_rr_t;

// DNS message structure
typedef struct {
    dns_header_t header;
    dns_question_t* questions;
    dns_rr_t* answers;
    dns_rr_t* authorities;
    dns_rr_t* additionals;
} dns_message_t;

// DNS cache entry structure
typedef struct {
    char hostname[256];
    unsigned int ip_address;
    unsigned int ttl;
    unsigned int timestamp;
} dns_cache_entry_t;

// DNS client structure
typedef struct {
    int socket_id;
    unsigned int server_ip;
    unsigned short server_port;
    unsigned int timeout;
    unsigned int retries;
    dns_cache_entry_t* cache;
    unsigned int cache_size;
    unsigned int cache_capacity;
} dns_client_t;

// DNS client functions
void dns_init();
dns_client_t* dns_client_create();
void dns_client_destroy(dns_client_t* client);
int dns_client_set_server(dns_client_t* client, unsigned int server_ip, unsigned short server_port);
int dns_client_set_timeout(dns_client_t* client, unsigned int timeout);
int dns_client_set_retries(dns_client_t* client, unsigned int retries);
int dns_client_set_cache_capacity(dns_client_t* client, unsigned int capacity);
int dns_client_clear_cache(dns_client_t* client);
int dns_resolve(dns_client_t* client, const char* hostname, unsigned int* ip_address);
int dns_resolve_ipv6(dns_client_t* client, const char* hostname, unsigned char* ipv6_address);
int dns_reverse_lookup(dns_client_t* client, unsigned int ip_address, char* hostname, unsigned int hostname_size);
int dns_reverse_lookup_ipv6(dns_client_t* client, const unsigned char* ipv6_address, char* hostname, unsigned int hostname_size);
dns_message_t* dns_query(dns_client_t* client, const char* hostname, dns_type_t type, dns_class_t class);

// DNS message functions
dns_message_t* dns_message_create();
void dns_message_destroy(dns_message_t* message);
int dns_message_add_question(dns_message_t* message, const char* qname, dns_type_t qtype, dns_class_t qclass);
int dns_message_add_answer(dns_message_t* message, const char* name, dns_type_t type, dns_class_t class, unsigned int ttl, const void* rdata, unsigned short rdlength);
int dns_message_add_authority(dns_message_t* message, const char* name, dns_type_t type, dns_class_t class, unsigned int ttl, const void* rdata, unsigned short rdlength);
int dns_message_add_additional(dns_message_t* message, const char* name, dns_type_t type, dns_class_t class, unsigned int ttl, const void* rdata, unsigned short rdlength);
int dns_message_to_packet(dns_message_t* message, unsigned char* packet, unsigned int* packet_size);
int dns_packet_to_message(const unsigned char* packet, unsigned int packet_size, dns_message_t* message);

// DNS utility functions
const char* dns_type_to_string(dns_type_t type);
dns_type_t dns_string_to_type(const char* type);
const char* dns_class_to_string(dns_class_t class);
dns_class_t dns_string_to_class(const char* class);
int dns_name_to_labels(const char* name, unsigned char* labels, unsigned int* labels_size);
int dns_labels_to_name(const unsigned char* labels, unsigned int labels_size, char* name, unsigned int name_size);
int dns_is_valid_hostname(const char* hostname);

// Global DNS functions
int dns_set_default_server(unsigned int server_ip, unsigned short server_port);
int dns_set_default_timeout(unsigned int timeout);
int dns_set_default_retries(unsigned int retries);
int dns_set_default_cache_capacity(unsigned int capacity);
int dns_clear_default_cache();
int dns_global_resolve(const char* hostname, unsigned int* ip_address);
int dns_global_reverse_lookup(unsigned int ip_address, char* hostname, unsigned int hostname_size);

#endif /* DNS_H */

/**
 * LightOS Networking
 * SSL/TLS protocol header
 */

#ifndef SSL_H
#define SSL_H

#include "network.h"
#include "tcp.h"

// SSL/TLS protocol versions
typedef enum {
    SSL_VERSION_SSL_3_0 = 0x0300,
    SSL_VERSION_TLS_1_0 = 0x0301,
    SSL_VERSION_TLS_1_1 = 0x0302,
    SSL_VERSION_TLS_1_2 = 0x0303,
    SSL_VERSION_TLS_1_3 = 0x0304
} ssl_version_t;

// SSL/TLS content types
typedef enum {
    SSL_CONTENT_TYPE_CHANGE_CIPHER_SPEC = 20,
    SSL_CONTENT_TYPE_ALERT = 21,
    SSL_CONTENT_TYPE_HANDSHAKE = 22,
    SSL_CONTENT_TYPE_APPLICATION_DATA = 23,
    SSL_CONTENT_TYPE_HEARTBEAT = 24
} ssl_content_type_t;

// SSL/TLS handshake types
typedef enum {
    SSL_HANDSHAKE_TYPE_HELLO_REQUEST = 0,
    SSL_HANDSHAKE_TYPE_CLIENT_HELLO = 1,
    SSL_HANDSHAKE_TYPE_SERVER_HELLO = 2,
    SSL_HANDSHAKE_TYPE_NEW_SESSION_TICKET = 4,
    SSL_HANDSHAKE_TYPE_END_OF_EARLY_DATA = 5,
    SSL_HANDSHAKE_TYPE_ENCRYPTED_EXTENSIONS = 8,
    SSL_HANDSHAKE_TYPE_CERTIFICATE = 11,
    SSL_HANDSHAKE_TYPE_SERVER_KEY_EXCHANGE = 12,
    SSL_HANDSHAKE_TYPE_CERTIFICATE_REQUEST = 13,
    SSL_HANDSHAKE_TYPE_SERVER_HELLO_DONE = 14,
    SSL_HANDSHAKE_TYPE_CERTIFICATE_VERIFY = 15,
    SSL_HANDSHAKE_TYPE_CLIENT_KEY_EXCHANGE = 16,
    SSL_HANDSHAKE_TYPE_FINISHED = 20,
    SSL_HANDSHAKE_TYPE_CERTIFICATE_URL = 21,
    SSL_HANDSHAKE_TYPE_CERTIFICATE_STATUS = 22,
    SSL_HANDSHAKE_TYPE_SUPPLEMENTAL_DATA = 23,
    SSL_HANDSHAKE_TYPE_KEY_UPDATE = 24,
    SSL_HANDSHAKE_TYPE_MESSAGE_HASH = 254
} ssl_handshake_type_t;

// SSL/TLS alert levels
typedef enum {
    SSL_ALERT_LEVEL_WARNING = 1,
    SSL_ALERT_LEVEL_FATAL = 2
} ssl_alert_level_t;

// SSL/TLS alert descriptions
typedef enum {
    SSL_ALERT_CLOSE_NOTIFY = 0,
    SSL_ALERT_UNEXPECTED_MESSAGE = 10,
    SSL_ALERT_BAD_RECORD_MAC = 20,
    SSL_ALERT_DECRYPTION_FAILED = 21,
    SSL_ALERT_RECORD_OVERFLOW = 22,
    SSL_ALERT_DECOMPRESSION_FAILURE = 30,
    SSL_ALERT_HANDSHAKE_FAILURE = 40,
    SSL_ALERT_NO_CERTIFICATE = 41,
    SSL_ALERT_BAD_CERTIFICATE = 42,
    SSL_ALERT_UNSUPPORTED_CERTIFICATE = 43,
    SSL_ALERT_CERTIFICATE_REVOKED = 44,
    SSL_ALERT_CERTIFICATE_EXPIRED = 45,
    SSL_ALERT_CERTIFICATE_UNKNOWN = 46,
    SSL_ALERT_ILLEGAL_PARAMETER = 47,
    SSL_ALERT_UNKNOWN_CA = 48,
    SSL_ALERT_ACCESS_DENIED = 49,
    SSL_ALERT_DECODE_ERROR = 50,
    SSL_ALERT_DECRYPT_ERROR = 51,
    SSL_ALERT_EXPORT_RESTRICTION = 60,
    SSL_ALERT_PROTOCOL_VERSION = 70,
    SSL_ALERT_INSUFFICIENT_SECURITY = 71,
    SSL_ALERT_INTERNAL_ERROR = 80,
    SSL_ALERT_INAPPROPRIATE_FALLBACK = 86,
    SSL_ALERT_USER_CANCELED = 90,
    SSL_ALERT_NO_RENEGOTIATION = 100,
    SSL_ALERT_MISSING_EXTENSION = 109,
    SSL_ALERT_UNSUPPORTED_EXTENSION = 110,
    SSL_ALERT_CERTIFICATE_UNOBTAINABLE = 111,
    SSL_ALERT_UNRECOGNIZED_NAME = 112,
    SSL_ALERT_BAD_CERTIFICATE_STATUS_RESPONSE = 113,
    SSL_ALERT_BAD_CERTIFICATE_HASH_VALUE = 114,
    SSL_ALERT_UNKNOWN_PSK_IDENTITY = 115,
    SSL_ALERT_CERTIFICATE_REQUIRED = 116,
    SSL_ALERT_NO_APPLICATION_PROTOCOL = 120
} ssl_alert_description_t;

// SSL/TLS cipher suites
typedef enum {
    SSL_CIPHER_TLS_RSA_WITH_AES_128_CBC_SHA = 0x002F,
    SSL_CIPHER_TLS_RSA_WITH_AES_256_CBC_SHA = 0x0035,
    SSL_CIPHER_TLS_RSA_WITH_AES_128_CBC_SHA256 = 0x003C,
    SSL_CIPHER_TLS_RSA_WITH_AES_256_CBC_SHA256 = 0x003D,
    SSL_CIPHER_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA = 0xC009,
    SSL_CIPHER_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA = 0xC00A,
    SSL_CIPHER_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA = 0xC013,
    SSL_CIPHER_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA = 0xC014,
    SSL_CIPHER_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 = 0xC023,
    SSL_CIPHER_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 = 0xC024,
    SSL_CIPHER_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 = 0xC027,
    SSL_CIPHER_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 = 0xC028,
    SSL_CIPHER_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 = 0xC02B,
    SSL_CIPHER_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 = 0xC02C,
    SSL_CIPHER_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 = 0xC02F,
    SSL_CIPHER_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 = 0xC030,
    SSL_CIPHER_TLS_AES_128_GCM_SHA256 = 0x1301,
    SSL_CIPHER_TLS_AES_256_GCM_SHA384 = 0x1302,
    SSL_CIPHER_TLS_CHACHA20_POLY1305_SHA256 = 0x1303
} ssl_cipher_suite_t;

// SSL/TLS context structure
typedef struct {
    ssl_version_t min_version;
    ssl_version_t max_version;
    ssl_cipher_suite_t* cipher_suites;
    unsigned int cipher_suite_count;
    char* certificate_file;
    char* private_key_file;
    char* ca_file;
    char* ca_path;
    unsigned int verify_peer;
    unsigned int verify_fail_if_no_peer_cert;
    unsigned int verify_depth;
    char* password;
    void* private_data;
} ssl_context_t;

// SSL/TLS connection structure
typedef struct {
    int socket_id;
    ssl_context_t* context;
    ssl_version_t version;
    ssl_cipher_suite_t cipher_suite;
    unsigned int handshake_completed;
    void* session;
    void* private_data;
} ssl_connection_t;

// SSL/TLS certificate structure
typedef struct {
    char* subject;
    char* issuer;
    char* serial_number;
    char* not_before;
    char* not_after;
    char* fingerprint;
    void* private_data;
} ssl_certificate_t;

// SSL/TLS functions
void ssl_init();
ssl_context_t* ssl_context_create();
void ssl_context_destroy(ssl_context_t* context);
int ssl_context_set_min_version(ssl_context_t* context, ssl_version_t version);
int ssl_context_set_max_version(ssl_context_t* context, ssl_version_t version);
int ssl_context_set_cipher_suites(ssl_context_t* context, ssl_cipher_suite_t* cipher_suites, unsigned int count);
int ssl_context_set_certificate_file(ssl_context_t* context, const char* file);
int ssl_context_set_private_key_file(ssl_context_t* context, const char* file);
int ssl_context_set_ca_file(ssl_context_t* context, const char* file);
int ssl_context_set_ca_path(ssl_context_t* context, const char* path);
int ssl_context_set_verify_peer(ssl_context_t* context, unsigned int verify_peer);
int ssl_context_set_verify_fail_if_no_peer_cert(ssl_context_t* context, unsigned int verify_fail_if_no_peer_cert);
int ssl_context_set_verify_depth(ssl_context_t* context, unsigned int verify_depth);
int ssl_context_set_password(ssl_context_t* context, const char* password);

ssl_connection_t* ssl_connection_create(ssl_context_t* context, int socket_id);
void ssl_connection_destroy(ssl_connection_t* connection);
int ssl_connection_handshake(ssl_connection_t* connection);
int ssl_connection_send(ssl_connection_t* connection, const void* data, unsigned int length);
int ssl_connection_receive(ssl_connection_t* connection, void* buffer, unsigned int buffer_size);
int ssl_connection_shutdown(ssl_connection_t* connection);
ssl_certificate_t* ssl_connection_get_peer_certificate(ssl_connection_t* connection);
int ssl_connection_verify_peer_certificate(ssl_connection_t* connection);
ssl_version_t ssl_connection_get_version(ssl_connection_t* connection);
ssl_cipher_suite_t ssl_connection_get_cipher_suite(ssl_connection_t* connection);

ssl_certificate_t* ssl_certificate_create();
void ssl_certificate_destroy(ssl_certificate_t* certificate);
int ssl_certificate_load_from_file(ssl_certificate_t* certificate, const char* file);
int ssl_certificate_load_from_data(ssl_certificate_t* certificate, const void* data, unsigned int length);
int ssl_certificate_verify(ssl_certificate_t* certificate, ssl_certificate_t* ca_certificate);
int ssl_certificate_get_subject(ssl_certificate_t* certificate, char* subject, unsigned int subject_size);
int ssl_certificate_get_issuer(ssl_certificate_t* certificate, char* issuer, unsigned int issuer_size);
int ssl_certificate_get_serial_number(ssl_certificate_t* certificate, char* serial_number, unsigned int serial_number_size);
int ssl_certificate_get_not_before(ssl_certificate_t* certificate, char* not_before, unsigned int not_before_size);
int ssl_certificate_get_not_after(ssl_certificate_t* certificate, char* not_after, unsigned int not_after_size);
int ssl_certificate_get_fingerprint(ssl_certificate_t* certificate, char* fingerprint, unsigned int fingerprint_size);

// SSL/TLS utility functions
const char* ssl_version_to_string(ssl_version_t version);
ssl_version_t ssl_string_to_version(const char* version);
const char* ssl_cipher_suite_to_string(ssl_cipher_suite_t cipher_suite);
ssl_cipher_suite_t ssl_string_to_cipher_suite(const char* cipher_suite);
const char* ssl_alert_level_to_string(ssl_alert_level_t level);
const char* ssl_alert_description_to_string(ssl_alert_description_t description);
int ssl_generate_random(void* buffer, unsigned int length);

#endif /* SSL_H */

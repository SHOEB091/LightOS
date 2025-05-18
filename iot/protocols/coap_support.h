/**
 * LightOS IoT
 * CoAP Protocol Support header
 */

#ifndef COAP_SUPPORT_H
#define COAP_SUPPORT_H

#include "../iot_manager.h"

// CoAP message types
#define COAP_MESSAGE_CON 0 // Confirmable
#define COAP_MESSAGE_NON 1 // Non-confirmable
#define COAP_MESSAGE_ACK 2 // Acknowledgement
#define COAP_MESSAGE_RST 3 // Reset

// CoAP request methods
#define COAP_METHOD_GET 1
#define COAP_METHOD_POST 2
#define COAP_METHOD_PUT 3
#define COAP_METHOD_DELETE 4

// CoAP response codes
#define COAP_RESPONSE_CREATED 201
#define COAP_RESPONSE_DELETED 202
#define COAP_RESPONSE_VALID 203
#define COAP_RESPONSE_CHANGED 204
#define COAP_RESPONSE_CONTENT 205
#define COAP_RESPONSE_BAD_REQUEST 400
#define COAP_RESPONSE_UNAUTHORIZED 401
#define COAP_RESPONSE_BAD_OPTION 402
#define COAP_RESPONSE_FORBIDDEN 403
#define COAP_RESPONSE_NOT_FOUND 404
#define COAP_RESPONSE_METHOD_NOT_ALLOWED 405
#define COAP_RESPONSE_NOT_ACCEPTABLE 406
#define COAP_RESPONSE_PRECONDITION_FAILED 412
#define COAP_RESPONSE_REQUEST_ENTITY_TOO_LARGE 413
#define COAP_RESPONSE_UNSUPPORTED_CONTENT_FORMAT 415
#define COAP_RESPONSE_INTERNAL_SERVER_ERROR 500
#define COAP_RESPONSE_NOT_IMPLEMENTED 501
#define COAP_RESPONSE_BAD_GATEWAY 502
#define COAP_RESPONSE_SERVICE_UNAVAILABLE 503
#define COAP_RESPONSE_GATEWAY_TIMEOUT 504
#define COAP_RESPONSE_PROXYING_NOT_SUPPORTED 505

// CoAP content formats
#define COAP_CONTENT_TEXT_PLAIN 0
#define COAP_CONTENT_APPLICATION_LINK_FORMAT 40
#define COAP_CONTENT_APPLICATION_XML 41
#define COAP_CONTENT_APPLICATION_OCTET_STREAM 42
#define COAP_CONTENT_APPLICATION_EXI 47
#define COAP_CONTENT_APPLICATION_JSON 50
#define COAP_CONTENT_APPLICATION_CBOR 60

// CoAP option numbers
#define COAP_OPTION_IF_MATCH 1
#define COAP_OPTION_URI_HOST 3
#define COAP_OPTION_ETAG 4
#define COAP_OPTION_IF_NONE_MATCH 5
#define COAP_OPTION_URI_PORT 7
#define COAP_OPTION_LOCATION_PATH 8
#define COAP_OPTION_URI_PATH 11
#define COAP_OPTION_CONTENT_FORMAT 12
#define COAP_OPTION_MAX_AGE 14
#define COAP_OPTION_URI_QUERY 15
#define COAP_OPTION_ACCEPT 17
#define COAP_OPTION_LOCATION_QUERY 20
#define COAP_OPTION_PROXY_URI 35
#define COAP_OPTION_PROXY_SCHEME 39
#define COAP_OPTION_SIZE1 60

// CoAP message structure
typedef struct {
    unsigned int type;
    unsigned int code;
    unsigned int message_id;
    char token[8];
    unsigned int token_length;
    char* options;
    unsigned int option_count;
    char* payload;
    unsigned int payload_length;
} coap_message_t;

// CoAP functions
int coap_init();
int coap_connect(iot_device_t* device);
int coap_disconnect(iot_device_t* device);
int coap_get(iot_device_t* device, const char* path, char* response, unsigned int response_size);
int coap_post(iot_device_t* device, const char* path, const char* payload, unsigned int payload_size, char* response, unsigned int response_size);
int coap_put(iot_device_t* device, const char* path, const char* payload, unsigned int payload_size, char* response, unsigned int response_size);
int coap_delete(iot_device_t* device, const char* path, char* response, unsigned int response_size);
int coap_observe(iot_device_t* device, const char* path, void (*callback)(const char* path, const char* payload, unsigned int payload_size));
int coap_cancel_observe(iot_device_t* device, const char* path);
int coap_send_message(iot_device_t* device, coap_message_t* message, coap_message_t* response);
int coap_create_message(unsigned int type, unsigned int code, unsigned int message_id, const char* token, unsigned int token_length, coap_message_t* message);
int coap_add_option(coap_message_t* message, unsigned int option_number, const char* option_value, unsigned int option_length);
int coap_set_payload(coap_message_t* message, const char* payload, unsigned int payload_length);
int coap_parse_message(const char* data, unsigned int data_length, coap_message_t* message);
int coap_format_message(coap_message_t* message, char* buffer, unsigned int buffer_size);
int coap_get_option(coap_message_t* message, unsigned int option_number, char* option_value, unsigned int* option_length);
int coap_set_block_option(coap_message_t* message, unsigned int block_number, unsigned int block_size, unsigned int more);
int coap_get_block_option(coap_message_t* message, unsigned int* block_number, unsigned int* block_size, unsigned int* more);
int coap_set_observe_option(coap_message_t* message, unsigned int observe);
int coap_get_observe_option(coap_message_t* message, unsigned int* observe);
int coap_set_content_format(coap_message_t* message, unsigned int content_format);
int coap_get_content_format(coap_message_t* message, unsigned int* content_format);
int coap_set_max_age(coap_message_t* message, unsigned int max_age);
int coap_get_max_age(coap_message_t* message, unsigned int* max_age);
int coap_set_etag(coap_message_t* message, const char* etag, unsigned int etag_length);
int coap_get_etag(coap_message_t* message, char* etag, unsigned int* etag_length);
int coap_set_uri_path(coap_message_t* message, const char* path);
int coap_get_uri_path(coap_message_t* message, char* path, unsigned int path_size);
int coap_set_uri_query(coap_message_t* message, const char* query);
int coap_get_uri_query(coap_message_t* message, char* query, unsigned int query_size);
int coap_set_location_path(coap_message_t* message, const char* path);
int coap_get_location_path(coap_message_t* message, char* path, unsigned int path_size);
int coap_set_location_query(coap_message_t* message, const char* query);
int coap_get_location_query(coap_message_t* message, char* query, unsigned int query_size);

#endif /* COAP_SUPPORT_H */

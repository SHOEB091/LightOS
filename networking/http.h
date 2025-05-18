/**
 * LightOS Networking
 * HTTP protocol header
 */

#ifndef HTTP_H
#define HTTP_H

#include "network.h"
#include "tcp.h"

// HTTP method types
typedef enum {
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_PATCH
} http_method_t;

// HTTP status codes
typedef enum {
    HTTP_STATUS_CONTINUE = 100,
    HTTP_STATUS_SWITCHING_PROTOCOLS = 101,
    HTTP_STATUS_PROCESSING = 102,
    HTTP_STATUS_EARLY_HINTS = 103,
    
    HTTP_STATUS_OK = 200,
    HTTP_STATUS_CREATED = 201,
    HTTP_STATUS_ACCEPTED = 202,
    HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
    HTTP_STATUS_NO_CONTENT = 204,
    HTTP_STATUS_RESET_CONTENT = 205,
    HTTP_STATUS_PARTIAL_CONTENT = 206,
    HTTP_STATUS_MULTI_STATUS = 207,
    HTTP_STATUS_ALREADY_REPORTED = 208,
    HTTP_STATUS_IM_USED = 226,
    
    HTTP_STATUS_MULTIPLE_CHOICES = 300,
    HTTP_STATUS_MOVED_PERMANENTLY = 301,
    HTTP_STATUS_FOUND = 302,
    HTTP_STATUS_SEE_OTHER = 303,
    HTTP_STATUS_NOT_MODIFIED = 304,
    HTTP_STATUS_USE_PROXY = 305,
    HTTP_STATUS_TEMPORARY_REDIRECT = 307,
    HTTP_STATUS_PERMANENT_REDIRECT = 308,
    
    HTTP_STATUS_BAD_REQUEST = 400,
    HTTP_STATUS_UNAUTHORIZED = 401,
    HTTP_STATUS_PAYMENT_REQUIRED = 402,
    HTTP_STATUS_FORBIDDEN = 403,
    HTTP_STATUS_NOT_FOUND = 404,
    HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
    HTTP_STATUS_NOT_ACCEPTABLE = 406,
    HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_STATUS_REQUEST_TIMEOUT = 408,
    HTTP_STATUS_CONFLICT = 409,
    HTTP_STATUS_GONE = 410,
    HTTP_STATUS_LENGTH_REQUIRED = 411,
    HTTP_STATUS_PRECONDITION_FAILED = 412,
    HTTP_STATUS_PAYLOAD_TOO_LARGE = 413,
    HTTP_STATUS_URI_TOO_LONG = 414,
    HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_STATUS_RANGE_NOT_SATISFIABLE = 416,
    HTTP_STATUS_EXPECTATION_FAILED = 417,
    HTTP_STATUS_IM_A_TEAPOT = 418,
    HTTP_STATUS_MISDIRECTED_REQUEST = 421,
    HTTP_STATUS_UNPROCESSABLE_ENTITY = 422,
    HTTP_STATUS_LOCKED = 423,
    HTTP_STATUS_FAILED_DEPENDENCY = 424,
    HTTP_STATUS_TOO_EARLY = 425,
    HTTP_STATUS_UPGRADE_REQUIRED = 426,
    HTTP_STATUS_PRECONDITION_REQUIRED = 428,
    HTTP_STATUS_TOO_MANY_REQUESTS = 429,
    HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    
    HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
    HTTP_STATUS_NOT_IMPLEMENTED = 501,
    HTTP_STATUS_BAD_GATEWAY = 502,
    HTTP_STATUS_SERVICE_UNAVAILABLE = 503,
    HTTP_STATUS_GATEWAY_TIMEOUT = 504,
    HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED = 505,
    HTTP_STATUS_VARIANT_ALSO_NEGOTIATES = 506,
    HTTP_STATUS_INSUFFICIENT_STORAGE = 507,
    HTTP_STATUS_LOOP_DETECTED = 508,
    HTTP_STATUS_NOT_EXTENDED = 510,
    HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED = 511
} http_status_t;

// HTTP header structure
typedef struct {
    char name[64];
    char value[256];
} http_header_t;

// HTTP request structure
typedef struct {
    http_method_t method;
    char url[1024];
    char version[16];
    http_header_t* headers;
    unsigned int header_count;
    void* body;
    unsigned int body_length;
} http_request_t;

// HTTP response structure
typedef struct {
    char version[16];
    http_status_t status;
    char status_text[64];
    http_header_t* headers;
    unsigned int header_count;
    void* body;
    unsigned int body_length;
} http_response_t;

// HTTP client structure
typedef struct {
    int socket_id;
    char host[256];
    unsigned short port;
    unsigned int timeout;
    unsigned int follow_redirects;
    unsigned int max_redirects;
    char user_agent[256];
    char* cookies;
    unsigned int cookie_count;
} http_client_t;

// HTTP server structure
typedef struct {
    int socket_id;
    unsigned short port;
    unsigned int max_connections;
    unsigned int timeout;
    char server_name[256];
    char document_root[256];
    int (*request_handler)(http_request_t* request, http_response_t* response);
} http_server_t;

// HTTP client functions
http_client_t* http_client_create();
void http_client_destroy(http_client_t* client);
int http_client_set_host(http_client_t* client, const char* host, unsigned short port);
int http_client_set_timeout(http_client_t* client, unsigned int timeout);
int http_client_set_follow_redirects(http_client_t* client, unsigned int follow_redirects, unsigned int max_redirects);
int http_client_set_user_agent(http_client_t* client, const char* user_agent);
int http_client_add_cookie(http_client_t* client, const char* name, const char* value);
int http_client_clear_cookies(http_client_t* client);
http_response_t* http_client_get(http_client_t* client, const char* url);
http_response_t* http_client_post(http_client_t* client, const char* url, const void* body, unsigned int body_length, const char* content_type);
http_response_t* http_client_put(http_client_t* client, const char* url, const void* body, unsigned int body_length, const char* content_type);
http_response_t* http_client_delete(http_client_t* client, const char* url);
http_response_t* http_client_head(http_client_t* client, const char* url);
http_response_t* http_client_options(http_client_t* client, const char* url);
http_response_t* http_client_send_request(http_client_t* client, http_request_t* request);

// HTTP server functions
http_server_t* http_server_create(unsigned short port, const char* document_root);
void http_server_destroy(http_server_t* server);
int http_server_set_max_connections(http_server_t* server, unsigned int max_connections);
int http_server_set_timeout(http_server_t* server, unsigned int timeout);
int http_server_set_server_name(http_server_t* server, const char* server_name);
int http_server_set_request_handler(http_server_t* server, int (*request_handler)(http_request_t* request, http_response_t* response));
int http_server_start(http_server_t* server);
int http_server_stop(http_server_t* server);
int http_server_is_running(http_server_t* server);

// HTTP request functions
http_request_t* http_request_create();
void http_request_destroy(http_request_t* request);
int http_request_set_method(http_request_t* request, http_method_t method);
int http_request_set_url(http_request_t* request, const char* url);
int http_request_set_version(http_request_t* request, const char* version);
int http_request_add_header(http_request_t* request, const char* name, const char* value);
int http_request_set_header(http_request_t* request, const char* name, const char* value);
const char* http_request_get_header(http_request_t* request, const char* name);
int http_request_remove_header(http_request_t* request, const char* name);
int http_request_set_body(http_request_t* request, const void* body, unsigned int body_length);
int http_request_parse(http_request_t* request, const char* data, unsigned int length);
int http_request_to_string(http_request_t* request, char* buffer, unsigned int buffer_size);

// HTTP response functions
http_response_t* http_response_create();
void http_response_destroy(http_response_t* response);
int http_response_set_status(http_response_t* response, http_status_t status);
int http_response_set_version(http_response_t* response, const char* version);
int http_response_add_header(http_response_t* response, const char* name, const char* value);
int http_response_set_header(http_response_t* response, const char* name, const char* value);
const char* http_response_get_header(http_response_t* response, const char* name);
int http_response_remove_header(http_response_t* response, const char* name);
int http_response_set_body(http_response_t* response, const void* body, unsigned int body_length);
int http_response_parse(http_response_t* response, const char* data, unsigned int length);
int http_response_to_string(http_response_t* response, char* buffer, unsigned int buffer_size);

// HTTP utility functions
const char* http_method_to_string(http_method_t method);
http_method_t http_string_to_method(const char* method);
const char* http_status_to_string(http_status_t status);
char* http_url_encode(const char* url);
char* http_url_decode(const char* url);

#endif /* HTTP_H */

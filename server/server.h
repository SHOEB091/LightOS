/**
 * LightOS Server
 * Server header
 */

#ifndef SERVER_H
#define SERVER_H

// HTTP methods
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD
} http_method_t;

// HTTP request structure
typedef struct {
    http_method_t method;
    const char* path;
    const char* http_version;
    const char* host;
    const char* user_agent;
    const char* content_type;
    const char* content;
    int content_length;
} http_request_t;

// HTTP response structure
typedef struct {
    int status_code;
    const char* status_message;
    const char* content_type;
    const char* content;
    int content_length;
} http_response_t;

// Server configuration structure
typedef struct {
    int port;
    int max_connections;
    const char* document_root;
    const char* server_name;
} server_config_t;

// Server functions
void server_init();
void server_configure(server_config_t* config);
int server_start();
void server_stop();
int server_is_running();
void server_handle_connection(int client_socket);
void server_generate_response(http_request_t* request, http_response_t* response);
int server_parse_request(const char* request_str, http_request_t* request);
void server_format_response(http_response_t* response, char* buffer, int buffer_size);
void server_print_status();

#endif /* SERVER_H */

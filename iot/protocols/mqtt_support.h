/**
 * LightOS IoT
 * MQTT Protocol Support header
 */

#ifndef MQTT_SUPPORT_H
#define MQTT_SUPPORT_H

#include "../iot_manager.h"

// MQTT QoS levels
#define MQTT_QOS_0 0 // At most once
#define MQTT_QOS_1 1 // At least once
#define MQTT_QOS_2 2 // Exactly once

// MQTT protocol versions
#define MQTT_VERSION_3_1 3
#define MQTT_VERSION_3_1_1 4
#define MQTT_VERSION_5_0 5

// MQTT connection options
typedef struct {
    unsigned int version;
    unsigned int keep_alive;
    unsigned int clean_session;
    char will_topic[128];
    char will_message[256];
    unsigned int will_qos;
    unsigned int will_retain;
    unsigned int auto_reconnect;
    unsigned int reconnect_interval;
    unsigned int max_reconnect_attempts;
    unsigned int connect_timeout;
    unsigned int ack_timeout;
} mqtt_options_t;

// MQTT functions
int mqtt_init();
int mqtt_connect(iot_device_t* device);
int mqtt_disconnect(iot_device_t* device);
int mqtt_subscribe(iot_device_t* device, const char* topic, unsigned int qos);
int mqtt_unsubscribe(iot_device_t* device, const char* topic);
int mqtt_publish(iot_device_t* device, const char* topic, const char* payload, unsigned int payload_size, unsigned int qos, unsigned int retained);
int mqtt_receive(iot_device_t* device, iot_message_t* message);
int mqtt_set_options(iot_device_t* device, mqtt_options_t* options);
int mqtt_get_options(iot_device_t* device, mqtt_options_t* options);
int mqtt_set_callback(iot_device_t* device, void (*callback)(iot_device_t* device, const char* topic, const char* payload, unsigned int payload_size));
int mqtt_set_will(iot_device_t* device, const char* topic, const char* message, unsigned int qos, unsigned int retain);
int mqtt_clear_will(iot_device_t* device);
int mqtt_ping(iot_device_t* device);
int mqtt_is_connected(iot_device_t* device);
int mqtt_get_client_id(iot_device_t* device, char* client_id, unsigned int client_id_size);
int mqtt_set_client_id(iot_device_t* device, const char* client_id);
int mqtt_get_username(iot_device_t* device, char* username, unsigned int username_size);
int mqtt_set_username(iot_device_t* device, const char* username);
int mqtt_get_password(iot_device_t* device, char* password, unsigned int password_size);
int mqtt_set_password(iot_device_t* device, const char* password);
int mqtt_get_server(iot_device_t* device, char* server, unsigned int server_size);
int mqtt_set_server(iot_device_t* device, const char* server);
int mqtt_get_port(iot_device_t* device, unsigned int* port);
int mqtt_set_port(iot_device_t* device, unsigned int port);
int mqtt_get_keep_alive(iot_device_t* device, unsigned int* keep_alive);
int mqtt_set_keep_alive(iot_device_t* device, unsigned int keep_alive);
int mqtt_get_clean_session(iot_device_t* device, unsigned int* clean_session);
int mqtt_set_clean_session(iot_device_t* device, unsigned int clean_session);
int mqtt_get_version(iot_device_t* device, unsigned int* version);
int mqtt_set_version(iot_device_t* device, unsigned int version);
int mqtt_get_auto_reconnect(iot_device_t* device, unsigned int* auto_reconnect);
int mqtt_set_auto_reconnect(iot_device_t* device, unsigned int auto_reconnect);
int mqtt_get_reconnect_interval(iot_device_t* device, unsigned int* reconnect_interval);
int mqtt_set_reconnect_interval(iot_device_t* device, unsigned int reconnect_interval);
int mqtt_get_max_reconnect_attempts(iot_device_t* device, unsigned int* max_reconnect_attempts);
int mqtt_set_max_reconnect_attempts(iot_device_t* device, unsigned int max_reconnect_attempts);
int mqtt_get_connect_timeout(iot_device_t* device, unsigned int* connect_timeout);
int mqtt_set_connect_timeout(iot_device_t* device, unsigned int connect_timeout);
int mqtt_get_ack_timeout(iot_device_t* device, unsigned int* ack_timeout);
int mqtt_set_ack_timeout(iot_device_t* device, unsigned int ack_timeout);
int mqtt_get_secure(iot_device_t* device, unsigned int* secure);
int mqtt_set_secure(iot_device_t* device, unsigned int secure);
int mqtt_get_ca_file(iot_device_t* device, char* ca_file, unsigned int ca_file_size);
int mqtt_set_ca_file(iot_device_t* device, const char* ca_file);
int mqtt_get_cert_file(iot_device_t* device, char* cert_file, unsigned int cert_file_size);
int mqtt_set_cert_file(iot_device_t* device, const char* cert_file);
int mqtt_get_key_file(iot_device_t* device, char* key_file, unsigned int key_file_size);
int mqtt_set_key_file(iot_device_t* device, const char* key_file);
int mqtt_get_tls_version(iot_device_t* device, char* tls_version, unsigned int tls_version_size);
int mqtt_set_tls_version(iot_device_t* device, const char* tls_version);
int mqtt_get_cipher_suites(iot_device_t* device, char* cipher_suites, unsigned int cipher_suites_size);
int mqtt_set_cipher_suites(iot_device_t* device, const char* cipher_suites);
int mqtt_get_verify_peer(iot_device_t* device, unsigned int* verify_peer);
int mqtt_set_verify_peer(iot_device_t* device, unsigned int verify_peer);
int mqtt_get_verify_hostname(iot_device_t* device, unsigned int* verify_hostname);
int mqtt_set_verify_hostname(iot_device_t* device, unsigned int verify_hostname);

#endif /* MQTT_SUPPORT_H */

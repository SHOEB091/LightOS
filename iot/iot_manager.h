/**
 * LightOS IoT
 * IoT Manager header
 */

#ifndef IOT_MANAGER_H
#define IOT_MANAGER_H

// IoT protocol types
typedef enum {
    IOT_PROTOCOL_MQTT,
    IOT_PROTOCOL_COAP,
    IOT_PROTOCOL_HTTP,
    IOT_PROTOCOL_WEBSOCKET,
    IOT_PROTOCOL_ZIGBEE,
    IOT_PROTOCOL_ZWAVE,
    IOT_PROTOCOL_BLUETOOTH,
    IOT_PROTOCOL_BLE,
    IOT_PROTOCOL_LORA,
    IOT_PROTOCOL_CUSTOM
} iot_protocol_t;

// IoT device types
typedef enum {
    IOT_DEVICE_SENSOR,
    IOT_DEVICE_ACTUATOR,
    IOT_DEVICE_GATEWAY,
    IOT_DEVICE_CONTROLLER,
    IOT_DEVICE_CAMERA,
    IOT_DEVICE_DISPLAY,
    IOT_DEVICE_AUDIO,
    IOT_DEVICE_LIGHT,
    IOT_DEVICE_THERMOSTAT,
    IOT_DEVICE_LOCK,
    IOT_DEVICE_SWITCH,
    IOT_DEVICE_CUSTOM
} iot_device_type_t;

// IoT connection states
typedef enum {
    IOT_CONNECTION_DISCONNECTED,
    IOT_CONNECTION_CONNECTING,
    IOT_CONNECTION_CONNECTED,
    IOT_CONNECTION_DISCONNECTING,
    IOT_CONNECTION_ERROR
} iot_connection_state_t;

// IoT message structure
typedef struct {
    char topic[128];
    char payload[1024];
    unsigned int payload_size;
    unsigned int qos;
    unsigned int retained;
    void* private_data;
} iot_message_t;

// IoT device structure
typedef struct {
    char id[64];
    char name[64];
    iot_device_type_t type;
    iot_protocol_t protocol;
    iot_connection_state_t state;
    char address[128];
    unsigned int port;
    char username[64];
    char password[64];
    char client_id[64];
    unsigned int secure;
    char* topics;
    unsigned int topic_count;
    void* private_data;
} iot_device_t;

// IoT manager functions
void iot_manager_init();
int iot_add_device(const char* name, iot_device_type_t type, iot_protocol_t protocol, const char* address, unsigned int port);
int iot_remove_device(const char* name);
int iot_connect_device(const char* name);
int iot_disconnect_device(const char* name);
iot_device_t* iot_get_device(const char* name);
iot_connection_state_t iot_get_device_state(const char* name);
int iot_set_device_credentials(const char* name, const char* username, const char* password);
int iot_set_device_client_id(const char* name, const char* client_id);
int iot_set_device_secure(const char* name, unsigned int secure);
int iot_subscribe_topic(const char* name, const char* topic, unsigned int qos);
int iot_unsubscribe_topic(const char* name, const char* topic);
int iot_publish_message(const char* name, const char* topic, const char* payload, unsigned int payload_size, unsigned int qos, unsigned int retained);
int iot_receive_message(const char* name, iot_message_t* message);
int iot_list_devices(iot_device_t** devices, unsigned int* count);
int iot_list_topics(const char* name, char*** topics, unsigned int* count);

// MQTT protocol functions
int mqtt_init();
int mqtt_connect(iot_device_t* device);
int mqtt_disconnect(iot_device_t* device);
int mqtt_subscribe(iot_device_t* device, const char* topic, unsigned int qos);
int mqtt_unsubscribe(iot_device_t* device, const char* topic);
int mqtt_publish(iot_device_t* device, const char* topic, const char* payload, unsigned int payload_size, unsigned int qos, unsigned int retained);
int mqtt_receive(iot_device_t* device, iot_message_t* message);

// CoAP protocol functions
int coap_init();
int coap_connect(iot_device_t* device);
int coap_disconnect(iot_device_t* device);
int coap_get(iot_device_t* device, const char* path, char* response, unsigned int response_size);
int coap_post(iot_device_t* device, const char* path, const char* payload, unsigned int payload_size, char* response, unsigned int response_size);
int coap_put(iot_device_t* device, const char* path, const char* payload, unsigned int payload_size, char* response, unsigned int response_size);
int coap_delete(iot_device_t* device, const char* path, char* response, unsigned int response_size);
int coap_observe(iot_device_t* device, const char* path, void (*callback)(const char* path, const char* payload, unsigned int payload_size));
int coap_cancel_observe(iot_device_t* device, const char* path);

// Zigbee protocol functions
int zigbee_init();
int zigbee_connect(iot_device_t* device);
int zigbee_disconnect(iot_device_t* device);
int zigbee_send_command(iot_device_t* device, const char* command, const char* payload, unsigned int payload_size);
int zigbee_receive_command(iot_device_t* device, char* command, unsigned int command_size, char* payload, unsigned int payload_size);
int zigbee_discover_devices(iot_device_t* gateway, iot_device_t** devices, unsigned int* count);
int zigbee_pair_device(iot_device_t* gateway, iot_device_t* device);
int zigbee_unpair_device(iot_device_t* gateway, iot_device_t* device);

// Sensor device functions
int sensor_init();
int sensor_read(iot_device_t* device, char* value, unsigned int value_size);
int sensor_set_threshold(iot_device_t* device, double min_threshold, double max_threshold);
int sensor_get_threshold(iot_device_t* device, double* min_threshold, double* max_threshold);
int sensor_set_interval(iot_device_t* device, unsigned int interval);
int sensor_get_interval(iot_device_t* device);
int sensor_enable_notifications(iot_device_t* device, unsigned int enable);
int sensor_is_notifications_enabled(iot_device_t* device);

// Actuator device functions
int actuator_init();
int actuator_set_state(iot_device_t* device, unsigned int state);
int actuator_get_state(iot_device_t* device);
int actuator_toggle(iot_device_t* device);
int actuator_set_level(iot_device_t* device, unsigned int level);
int actuator_get_level(iot_device_t* device);

#endif /* IOT_MANAGER_H */

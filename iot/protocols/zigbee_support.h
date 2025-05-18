/**
 * LightOS IoT
 * Zigbee Protocol Support header
 */

#ifndef ZIGBEE_SUPPORT_H
#define ZIGBEE_SUPPORT_H

#include "../iot_manager.h"

// Zigbee device types
#define ZIGBEE_DEVICE_COORDINATOR 0
#define ZIGBEE_DEVICE_ROUTER 1
#define ZIGBEE_DEVICE_END_DEVICE 2

// Zigbee profile IDs
#define ZIGBEE_PROFILE_HOME_AUTOMATION 0x0104
#define ZIGBEE_PROFILE_SMART_ENERGY 0x0109
#define ZIGBEE_PROFILE_LIGHT_LINK 0xC05E
#define ZIGBEE_PROFILE_GREEN_POWER 0xA1E0

// Zigbee cluster IDs
#define ZIGBEE_CLUSTER_BASIC 0x0000
#define ZIGBEE_CLUSTER_POWER_CONFIG 0x0001
#define ZIGBEE_CLUSTER_DEVICE_TEMP 0x0002
#define ZIGBEE_CLUSTER_IDENTIFY 0x0003
#define ZIGBEE_CLUSTER_GROUPS 0x0004
#define ZIGBEE_CLUSTER_SCENES 0x0005
#define ZIGBEE_CLUSTER_ON_OFF 0x0006
#define ZIGBEE_CLUSTER_ON_OFF_SWITCH_CONFIG 0x0007
#define ZIGBEE_CLUSTER_LEVEL_CONTROL 0x0008
#define ZIGBEE_CLUSTER_ALARMS 0x0009
#define ZIGBEE_CLUSTER_TIME 0x000A
#define ZIGBEE_CLUSTER_RSSI_LOCATION 0x000B
#define ZIGBEE_CLUSTER_ANALOG_INPUT 0x000C
#define ZIGBEE_CLUSTER_ANALOG_OUTPUT 0x000D
#define ZIGBEE_CLUSTER_ANALOG_VALUE 0x000E
#define ZIGBEE_CLUSTER_BINARY_INPUT 0x000F
#define ZIGBEE_CLUSTER_BINARY_OUTPUT 0x0010
#define ZIGBEE_CLUSTER_BINARY_VALUE 0x0011
#define ZIGBEE_CLUSTER_MULTISTATE_INPUT 0x0012
#define ZIGBEE_CLUSTER_MULTISTATE_OUTPUT 0x0013
#define ZIGBEE_CLUSTER_MULTISTATE_VALUE 0x0014
#define ZIGBEE_CLUSTER_COMMISSIONING 0x0015
#define ZIGBEE_CLUSTER_OTA_UPGRADE 0x0019
#define ZIGBEE_CLUSTER_POLL_CONTROL 0x0020
#define ZIGBEE_CLUSTER_GREEN_POWER 0x0021
#define ZIGBEE_CLUSTER_KEEP_ALIVE 0x0025
#define ZIGBEE_CLUSTER_SHADE_CONFIG 0x0100
#define ZIGBEE_CLUSTER_DOOR_LOCK 0x0101
#define ZIGBEE_CLUSTER_WINDOW_COVERING 0x0102
#define ZIGBEE_CLUSTER_PUMP_CONFIG_CONTROL 0x0200
#define ZIGBEE_CLUSTER_THERMOSTAT 0x0201
#define ZIGBEE_CLUSTER_FAN_CONTROL 0x0202
#define ZIGBEE_CLUSTER_DEHUMIDIFICATION_CONTROL 0x0203
#define ZIGBEE_CLUSTER_THERMOSTAT_UI_CONFIG 0x0204
#define ZIGBEE_CLUSTER_COLOR_CONTROL 0x0300
#define ZIGBEE_CLUSTER_BALLAST_CONFIG 0x0301
#define ZIGBEE_CLUSTER_ILLUMINANCE_MEASUREMENT 0x0400
#define ZIGBEE_CLUSTER_ILLUMINANCE_LEVEL_SENSING 0x0401
#define ZIGBEE_CLUSTER_TEMPERATURE_MEASUREMENT 0x0402
#define ZIGBEE_CLUSTER_PRESSURE_MEASUREMENT 0x0403
#define ZIGBEE_CLUSTER_FLOW_MEASUREMENT 0x0404
#define ZIGBEE_CLUSTER_RELATIVE_HUMIDITY 0x0405
#define ZIGBEE_CLUSTER_OCCUPANCY_SENSING 0x0406
#define ZIGBEE_CLUSTER_IAS_ZONE 0x0500
#define ZIGBEE_CLUSTER_IAS_ACE 0x0501
#define ZIGBEE_CLUSTER_IAS_WD 0x0502
#define ZIGBEE_CLUSTER_PRICE 0x0700
#define ZIGBEE_CLUSTER_DEMAND_RESPONSE_LOAD_CONTROL 0x0701
#define ZIGBEE_CLUSTER_SIMPLE_METERING 0x0702
#define ZIGBEE_CLUSTER_MESSAGING 0x0703
#define ZIGBEE_CLUSTER_TUNNELING 0x0704
#define ZIGBEE_CLUSTER_KEY_ESTABLISHMENT 0x0800
#define ZIGBEE_CLUSTER_INFORMATION 0x0900
#define ZIGBEE_CLUSTER_VOICE_OVER_ZIGBEE 0x0904
#define ZIGBEE_CLUSTER_CHATTING 0x0905
#define ZIGBEE_CLUSTER_PAYMENT 0x0A01
#define ZIGBEE_CLUSTER_BILLING 0x0A02
#define ZIGBEE_CLUSTER_APPLIANCE_IDENTIFICATION 0x0B00
#define ZIGBEE_CLUSTER_METER_IDENTIFICATION 0x0B01
#define ZIGBEE_CLUSTER_APPLIANCE_EVENTS_ALERTS 0x0B02
#define ZIGBEE_CLUSTER_APPLIANCE_STATISTICS 0x0B03
#define ZIGBEE_CLUSTER_ELECTRICAL_MEASUREMENT 0x0B04
#define ZIGBEE_CLUSTER_DIAGNOSTICS 0x0B05
#define ZIGBEE_CLUSTER_ZLL_COMMISSIONING 0x1000

// Zigbee device structure
typedef struct {
    unsigned int device_type;
    unsigned int profile_id;
    unsigned int device_id;
    char ieee_address[16];
    unsigned int network_address;
    unsigned int parent_network_address;
    unsigned int lqi;
    unsigned int rssi;
    unsigned int depth;
    unsigned int permit_joining;
    char* endpoints;
    unsigned int endpoint_count;
} zigbee_device_t;

// Zigbee endpoint structure
typedef struct {
    unsigned int endpoint_id;
    unsigned int profile_id;
    unsigned int device_id;
    unsigned int device_version;
    char* input_clusters;
    unsigned int input_cluster_count;
    char* output_clusters;
    unsigned int output_cluster_count;
} zigbee_endpoint_t;

// Zigbee functions
int zigbee_init();
int zigbee_connect(iot_device_t* device);
int zigbee_disconnect(iot_device_t* device);
int zigbee_send_command(iot_device_t* device, const char* command, const char* payload, unsigned int payload_size);
int zigbee_receive_command(iot_device_t* device, char* command, unsigned int command_size, char* payload, unsigned int payload_size);
int zigbee_discover_devices(iot_device_t* gateway, iot_device_t** devices, unsigned int* count);
int zigbee_pair_device(iot_device_t* gateway, iot_device_t* device);
int zigbee_unpair_device(iot_device_t* gateway, iot_device_t* device);
int zigbee_permit_joining(iot_device_t* gateway, unsigned int duration);
int zigbee_get_network_info(iot_device_t* gateway, char* info, unsigned int info_size);
int zigbee_get_device_info(iot_device_t* gateway, const char* device_id, char* info, unsigned int info_size);
int zigbee_get_endpoint_info(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, char* info, unsigned int info_size);
int zigbee_get_cluster_info(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, char* info, unsigned int info_size);
int zigbee_get_attribute_info(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, unsigned int attribute_id, char* info, unsigned int info_size);
int zigbee_read_attribute(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, unsigned int attribute_id, char* value, unsigned int value_size);
int zigbee_write_attribute(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, unsigned int attribute_id, const char* value, unsigned int value_size);
int zigbee_configure_reporting(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, unsigned int attribute_id, unsigned int min_interval, unsigned int max_interval, unsigned int reportable_change);
int zigbee_bind(iot_device_t* gateway, const char* source_device_id, unsigned int source_endpoint_id, unsigned int cluster_id, const char* destination_device_id, unsigned int destination_endpoint_id);
int zigbee_unbind(iot_device_t* gateway, const char* source_device_id, unsigned int source_endpoint_id, unsigned int cluster_id, const char* destination_device_id, unsigned int destination_endpoint_id);
int zigbee_send_zcl_command(iot_device_t* gateway, const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, unsigned int command_id, const char* payload, unsigned int payload_size);
int zigbee_send_zdp_command(iot_device_t* gateway, const char* device_id, unsigned int cluster_id, unsigned int command_id, const char* payload, unsigned int payload_size);
int zigbee_set_callback(iot_device_t* gateway, void (*callback)(const char* device_id, unsigned int endpoint_id, unsigned int cluster_id, unsigned int command_id, const char* payload, unsigned int payload_size));

#endif /* ZIGBEE_SUPPORT_H */

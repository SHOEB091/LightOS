/**
 * LightOS CLI
 * IoT Commands header
 */

#ifndef IOT_COMMANDS_H
#define IOT_COMMANDS_H

// Register IoT commands
void register_iot_commands();

// Command handlers
int iot_command(int argc, char** argv);
int mqtt_command(int argc, char** argv);
int coap_command(int argc, char** argv);
int zigbee_command(int argc, char** argv);
int sensor_command(int argc, char** argv);
int actuator_command(int argc, char** argv);

#endif /* IOT_COMMANDS_H */

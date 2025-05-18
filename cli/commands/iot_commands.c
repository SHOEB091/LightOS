/**
 * LightOS CLI
 * IoT Commands implementation
 */

#include "iot_commands.h"
#include "../../kernel/kernel.h"
#include "../../iot/iot_manager.h"
#include "../../iot/protocols/mqtt_support.h"
#include "../../iot/protocols/coap_support.h"
#include "../../iot/protocols/zigbee_support.h"
#include "../../iot/devices/sensor_support.h"
#include "../../libc/string.h"

// Register IoT commands
void register_iot_commands() {
    cli_register_command("iot", iot_command, "IoT device management commands");
    cli_register_command("mqtt", mqtt_command, "MQTT protocol commands");
    cli_register_command("coap", coap_command, "CoAP protocol commands");
    cli_register_command("zigbee", zigbee_command, "Zigbee protocol commands");
    cli_register_command("sensor", sensor_command, "Sensor device commands");
    cli_register_command("actuator", actuator_command, "Actuator device commands");
}

// IoT command handler
int iot_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: iot <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  add <name> <type> <protocol> <address> <port>  Add an IoT device\n");
        terminal_write("  remove <name>                                  Remove an IoT device\n");
        terminal_write("  connect <name>                                 Connect to an IoT device\n");
        terminal_write("  disconnect <name>                              Disconnect from an IoT device\n");
        terminal_write("  list                                           List all IoT devices\n");
        terminal_write("  status <name>                                  Show the status of an IoT device\n");
        terminal_write("  set-credentials <name> <username> <password>   Set credentials for an IoT device\n");
        terminal_write("  set-client-id <name> <client-id>               Set client ID for an IoT device\n");
        terminal_write("  set-secure <name> <secure>                     Set secure flag for an IoT device\n");
        terminal_write("Device types:\n");
        terminal_write("  sensor                                         Sensor device\n");
        terminal_write("  actuator                                       Actuator device\n");
        terminal_write("  gateway                                        Gateway device\n");
        terminal_write("  controller                                     Controller device\n");
        terminal_write("  camera                                         Camera device\n");
        terminal_write("  display                                        Display device\n");
        terminal_write("  audio                                          Audio device\n");
        terminal_write("  light                                          Light device\n");
        terminal_write("  thermostat                                     Thermostat device\n");
        terminal_write("  lock                                           Lock device\n");
        terminal_write("  switch                                         Switch device\n");
        terminal_write("  custom                                         Custom device\n");
        terminal_write("Protocols:\n");
        terminal_write("  mqtt                                           MQTT protocol\n");
        terminal_write("  coap                                           CoAP protocol\n");
        terminal_write("  http                                           HTTP protocol\n");
        terminal_write("  websocket                                      WebSocket protocol\n");
        terminal_write("  zigbee                                         Zigbee protocol\n");
        terminal_write("  zwave                                          Z-Wave protocol\n");
        terminal_write("  bluetooth                                      Bluetooth protocol\n");
        terminal_write("  ble                                            Bluetooth Low Energy protocol\n");
        terminal_write("  lora                                           LoRa protocol\n");
        terminal_write("  custom                                         Custom protocol\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "add") == 0) {
        if (argc < 6) {
            terminal_write("Usage: iot add <name> <type> <protocol> <address> <port>\n");
            return -1;
        }
        
        const char* name = argv[2];
        const char* type_str = argv[3];
        const char* protocol_str = argv[4];
        const char* address = argv[5];
        unsigned int port = argc > 6 ? atoi(argv[6]) : 0;
        
        // Convert type string to enum
        iot_device_type_t type;
        if (strcmp(type_str, "sensor") == 0) {
            type = IOT_DEVICE_SENSOR;
        } else if (strcmp(type_str, "actuator") == 0) {
            type = IOT_DEVICE_ACTUATOR;
        } else if (strcmp(type_str, "gateway") == 0) {
            type = IOT_DEVICE_GATEWAY;
        } else if (strcmp(type_str, "controller") == 0) {
            type = IOT_DEVICE_CONTROLLER;
        } else if (strcmp(type_str, "camera") == 0) {
            type = IOT_DEVICE_CAMERA;
        } else if (strcmp(type_str, "display") == 0) {
            type = IOT_DEVICE_DISPLAY;
        } else if (strcmp(type_str, "audio") == 0) {
            type = IOT_DEVICE_AUDIO;
        } else if (strcmp(type_str, "light") == 0) {
            type = IOT_DEVICE_LIGHT;
        } else if (strcmp(type_str, "thermostat") == 0) {
            type = IOT_DEVICE_THERMOSTAT;
        } else if (strcmp(type_str, "lock") == 0) {
            type = IOT_DEVICE_LOCK;
        } else if (strcmp(type_str, "switch") == 0) {
            type = IOT_DEVICE_SWITCH;
        } else if (strcmp(type_str, "custom") == 0) {
            type = IOT_DEVICE_CUSTOM;
        } else {
            terminal_write("Error: Invalid device type\n");
            return -1;
        }
        
        // Convert protocol string to enum
        iot_protocol_t protocol;
        if (strcmp(protocol_str, "mqtt") == 0) {
            protocol = IOT_PROTOCOL_MQTT;
        } else if (strcmp(protocol_str, "coap") == 0) {
            protocol = IOT_PROTOCOL_COAP;
        } else if (strcmp(protocol_str, "http") == 0) {
            protocol = IOT_PROTOCOL_HTTP;
        } else if (strcmp(protocol_str, "websocket") == 0) {
            protocol = IOT_PROTOCOL_WEBSOCKET;
        } else if (strcmp(protocol_str, "zigbee") == 0) {
            protocol = IOT_PROTOCOL_ZIGBEE;
        } else if (strcmp(protocol_str, "zwave") == 0) {
            protocol = IOT_PROTOCOL_ZWAVE;
        } else if (strcmp(protocol_str, "bluetooth") == 0) {
            protocol = IOT_PROTOCOL_BLUETOOTH;
        } else if (strcmp(protocol_str, "ble") == 0) {
            protocol = IOT_PROTOCOL_BLE;
        } else if (strcmp(protocol_str, "lora") == 0) {
            protocol = IOT_PROTOCOL_LORA;
        } else if (strcmp(protocol_str, "custom") == 0) {
            protocol = IOT_PROTOCOL_CUSTOM;
        } else {
            terminal_write("Error: Invalid protocol\n");
            return -1;
        }
        
        return iot_add_device(name, type, protocol, address, port);
    }
    else if (strcmp(command, "remove") == 0) {
        if (argc < 3) {
            terminal_write("Usage: iot remove <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return iot_remove_device(name);
    }
    else if (strcmp(command, "connect") == 0) {
        if (argc < 3) {
            terminal_write("Usage: iot connect <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return iot_connect_device(name);
    }
    else if (strcmp(command, "disconnect") == 0) {
        if (argc < 3) {
            terminal_write("Usage: iot disconnect <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return iot_disconnect_device(name);
    }
    else if (strcmp(command, "list") == 0) {
        iot_device_t* devices[100];
        unsigned int count = 0;
        
        if (iot_list_devices(devices, &count) != 0) {
            terminal_write("Error: Failed to list devices\n");
            return -1;
        }
        
        terminal_write("IoT Devices:\n");
        
        if (count == 0) {
            terminal_write("  No devices found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(devices[i]->name);
            terminal_write(" (");
            
            // Print device type
            switch (devices[i]->type) {
                case IOT_DEVICE_SENSOR:
                    terminal_write("Sensor");
                    break;
                case IOT_DEVICE_ACTUATOR:
                    terminal_write("Actuator");
                    break;
                case IOT_DEVICE_GATEWAY:
                    terminal_write("Gateway");
                    break;
                case IOT_DEVICE_CONTROLLER:
                    terminal_write("Controller");
                    break;
                case IOT_DEVICE_CAMERA:
                    terminal_write("Camera");
                    break;
                case IOT_DEVICE_DISPLAY:
                    terminal_write("Display");
                    break;
                case IOT_DEVICE_AUDIO:
                    terminal_write("Audio");
                    break;
                case IOT_DEVICE_LIGHT:
                    terminal_write("Light");
                    break;
                case IOT_DEVICE_THERMOSTAT:
                    terminal_write("Thermostat");
                    break;
                case IOT_DEVICE_LOCK:
                    terminal_write("Lock");
                    break;
                case IOT_DEVICE_SWITCH:
                    terminal_write("Switch");
                    break;
                case IOT_DEVICE_CUSTOM:
                    terminal_write("Custom");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", ");
            
            // Print protocol
            switch (devices[i]->protocol) {
                case IOT_PROTOCOL_MQTT:
                    terminal_write("MQTT");
                    break;
                case IOT_PROTOCOL_COAP:
                    terminal_write("CoAP");
                    break;
                case IOT_PROTOCOL_HTTP:
                    terminal_write("HTTP");
                    break;
                case IOT_PROTOCOL_WEBSOCKET:
                    terminal_write("WebSocket");
                    break;
                case IOT_PROTOCOL_ZIGBEE:
                    terminal_write("Zigbee");
                    break;
                case IOT_PROTOCOL_ZWAVE:
                    terminal_write("Z-Wave");
                    break;
                case IOT_PROTOCOL_BLUETOOTH:
                    terminal_write("Bluetooth");
                    break;
                case IOT_PROTOCOL_BLE:
                    terminal_write("BLE");
                    break;
                case IOT_PROTOCOL_LORA:
                    terminal_write("LoRa");
                    break;
                case IOT_PROTOCOL_CUSTOM:
                    terminal_write("Custom");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", ");
            
            // Print connection state
            switch (devices[i]->state) {
                case IOT_CONNECTION_DISCONNECTED:
                    terminal_write("Disconnected");
                    break;
                case IOT_CONNECTION_CONNECTING:
                    terminal_write("Connecting");
                    break;
                case IOT_CONNECTION_CONNECTED:
                    terminal_write("Connected");
                    break;
                case IOT_CONNECTION_DISCONNECTING:
                    terminal_write("Disconnecting");
                    break;
                case IOT_CONNECTION_ERROR:
                    terminal_write("Error");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(")\n");
        }
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// MQTT command handler
int mqtt_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: mqtt <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  subscribe <device> <topic> [qos]       Subscribe to an MQTT topic\n");
        terminal_write("  unsubscribe <device> <topic>           Unsubscribe from an MQTT topic\n");
        terminal_write("  publish <device> <topic> <message>     Publish a message to an MQTT topic\n");
        terminal_write("  list-topics <device>                   List subscribed topics\n");
        return 0;
    }
    
    // For now, just forward to the IoT command
    return iot_command(argc, argv);
}

// CoAP command handler
int coap_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: coap <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  get <device> <path>                    Send a GET request\n");
        terminal_write("  post <device> <path> <payload>         Send a POST request\n");
        terminal_write("  put <device> <path> <payload>          Send a PUT request\n");
        terminal_write("  delete <device> <path>                 Send a DELETE request\n");
        terminal_write("  observe <device> <path>                Observe a resource\n");
        terminal_write("  cancel-observe <device> <path>         Cancel observation of a resource\n");
        return 0;
    }
    
    // For now, just forward to the IoT command
    return iot_command(argc, argv);
}

// Zigbee command handler
int zigbee_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: zigbee <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  discover <gateway>                     Discover Zigbee devices\n");
        terminal_write("  pair <gateway> <device>                Pair a Zigbee device\n");
        terminal_write("  unpair <gateway> <device>              Unpair a Zigbee device\n");
        terminal_write("  permit-joining <gateway> <duration>    Allow devices to join the network\n");
        terminal_write("  send-command <device> <command>        Send a command to a Zigbee device\n");
        return 0;
    }
    
    // For now, just forward to the IoT command
    return iot_command(argc, argv);
}

// Sensor command handler
int sensor_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: sensor <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  read <device>                          Read a sensor value\n");
        terminal_write("  set-threshold <device> <min> <max>     Set the threshold for a sensor\n");
        terminal_write("  get-threshold <device>                 Get the threshold for a sensor\n");
        terminal_write("  set-interval <device> <interval>       Set the reading interval for a sensor\n");
        terminal_write("  get-interval <device>                  Get the reading interval for a sensor\n");
        terminal_write("  enable-notifications <device>          Enable notifications for a sensor\n");
        terminal_write("  disable-notifications <device>         Disable notifications for a sensor\n");
        return 0;
    }
    
    // For now, just forward to the IoT command
    return iot_command(argc, argv);
}

// Actuator command handler
int actuator_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: actuator <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  set-state <device> <state>             Set the state of an actuator\n");
        terminal_write("  get-state <device>                     Get the state of an actuator\n");
        terminal_write("  toggle <device>                        Toggle the state of an actuator\n");
        terminal_write("  set-level <device> <level>             Set the level of an actuator\n");
        terminal_write("  get-level <device>                     Get the level of an actuator\n");
        return 0;
    }
    
    // For now, just forward to the IoT command
    return iot_command(argc, argv);
}

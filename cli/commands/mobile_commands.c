/**
 * LightOS CLI
 * Mobile Commands implementation
 */

#include "mobile_commands.h"
#include "../../kernel/kernel.h"
#include "../../mobile/mobile_manager.h"
#include "../../mobile/protocols/adb_support.h"
#include "../../mobile/sync/file_sync.h"
#include "../../libc/string.h"

// Register mobile commands
void register_mobile_commands() {
    cli_register_command("mobile", mobile_command, "Mobile device management commands");
    cli_register_command("android", android_command, "Android device management commands");
    cli_register_command("ios", ios_command, "iOS device management commands");
    cli_register_command("mobile-sync", mobile_sync_command, "Mobile device synchronization commands");
}

// Mobile command handler
int mobile_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: mobile <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  detect                                  Detect connected mobile devices\n");
        terminal_write("  connect <id> <type>                     Connect to a mobile device\n");
        terminal_write("  disconnect <id>                         Disconnect from a mobile device\n");
        terminal_write("  list                                    List all mobile devices\n");
        terminal_write("  info <id>                               Show information about a mobile device\n");
        terminal_write("  files <id> <path>                       List files on a mobile device\n");
        terminal_write("  get <id> <device-path> <local-path>     Get a file from a mobile device\n");
        terminal_write("  put <id> <local-path> <device-path>     Put a file on a mobile device\n");
        terminal_write("  delete <id> <path>                      Delete a file on a mobile device\n");
        terminal_write("  mkdir <id> <path>                       Create a directory on a mobile device\n");
        terminal_write("  rmdir <id> <path>                       Remove a directory on a mobile device\n");
        terminal_write("  contacts <id>                           List contacts on a mobile device\n");
        terminal_write("  add-contact <id> <name> <phone> <email> Add a contact to a mobile device\n");
        terminal_write("  delete-contact <id> <contact-id>        Delete a contact from a mobile device\n");
        terminal_write("  calendar <id>                           List calendar events on a mobile device\n");
        terminal_write("  add-event <id> <title> <start> <end>    Add a calendar event to a mobile device\n");
        terminal_write("  delete-event <id> <event-id>            Delete a calendar event from a mobile device\n");
        terminal_write("  messages <id>                           List messages on a mobile device\n");
        terminal_write("  send-message <id> <to> <message>        Send a message from a mobile device\n");
        terminal_write("  delete-message <id> <message-id>        Delete a message from a mobile device\n");
        terminal_write("  photos <id>                             List photos on a mobile device\n");
        terminal_write("  music <id>                              List music on a mobile device\n");
        terminal_write("  videos <id>                             List videos on a mobile device\n");
        terminal_write("  apps <id>                               List apps on a mobile device\n");
        terminal_write("  install-app <id> <app-path>             Install an app on a mobile device\n");
        terminal_write("  uninstall-app <id> <app-id>             Uninstall an app from a mobile device\n");
        terminal_write("  launch-app <id> <app-id>                Launch an app on a mobile device\n");
        terminal_write("  stop-app <id> <app-id>                  Stop an app on a mobile device\n");
        terminal_write("  screenshot <id> <local-path>            Take a screenshot of a mobile device\n");
        terminal_write("  reboot <id>                             Reboot a mobile device\n");
        terminal_write("  shutdown <id>                           Shut down a mobile device\n");
        terminal_write("Connection types:\n");
        terminal_write("  usb                                     USB connection\n");
        terminal_write("  wifi                                    Wi-Fi connection\n");
        terminal_write("  bluetooth                               Bluetooth connection\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "detect") == 0) {
        mobile_device_t* devices[10];
        unsigned int count = 0;
        
        if (mobile_detect_devices(devices, &count) != 0) {
            terminal_write("Error: Failed to detect mobile devices\n");
            return -1;
        }
        
        terminal_write("Detected ");
        
        // Convert count to string
        char count_str[16];
        sprintf(count_str, "%u", count);
        terminal_write(count_str);
        terminal_write(" mobile device(s):\n");
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(devices[i]->id);
            terminal_write(": ");
            terminal_write(devices[i]->name);
            terminal_write(" (");
            
            // Print device type
            switch (devices[i]->type) {
                case MOBILE_DEVICE_ANDROID:
                    terminal_write("Android");
                    break;
                case MOBILE_DEVICE_IOS:
                    terminal_write("iOS");
                    break;
                case MOBILE_DEVICE_WINDOWS:
                    terminal_write("Windows");
                    break;
                case MOBILE_DEVICE_BLACKBERRY:
                    terminal_write("BlackBerry");
                    break;
                case MOBILE_DEVICE_SYMBIAN:
                    terminal_write("Symbian");
                    break;
                case MOBILE_DEVICE_CUSTOM:
                    terminal_write("Custom");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(")\n");
        }
        
        return 0;
    }
    else if (strcmp(command, "connect") == 0) {
        if (argc < 4) {
            terminal_write("Usage: mobile connect <id> <type>\n");
            return -1;
        }
        
        const char* id = argv[2];
        const char* type_str = argv[3];
        
        // Convert type string to enum
        mobile_connection_type_t type;
        if (strcmp(type_str, "usb") == 0) {
            type = MOBILE_CONNECTION_USB;
        } else if (strcmp(type_str, "wifi") == 0) {
            type = MOBILE_CONNECTION_WIFI;
        } else if (strcmp(type_str, "bluetooth") == 0) {
            type = MOBILE_CONNECTION_BLUETOOTH;
        } else {
            terminal_write("Error: Invalid connection type\n");
            return -1;
        }
        
        return mobile_connect_device(id, type);
    }
    else if (strcmp(command, "disconnect") == 0) {
        if (argc < 3) {
            terminal_write("Usage: mobile disconnect <id>\n");
            return -1;
        }
        
        const char* id = argv[2];
        
        return mobile_disconnect_device(id);
    }
    else if (strcmp(command, "list") == 0) {
        mobile_device_t* devices[10];
        unsigned int count = 0;
        
        if (mobile_list_devices(devices, &count) != 0) {
            terminal_write("Error: Failed to list mobile devices\n");
            return -1;
        }
        
        terminal_write("Mobile Devices:\n");
        
        if (count == 0) {
            terminal_write("  No devices found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(devices[i]->id);
            terminal_write(": ");
            terminal_write(devices[i]->name);
            terminal_write(" (");
            
            // Print device type
            switch (devices[i]->type) {
                case MOBILE_DEVICE_ANDROID:
                    terminal_write("Android");
                    break;
                case MOBILE_DEVICE_IOS:
                    terminal_write("iOS");
                    break;
                case MOBILE_DEVICE_WINDOWS:
                    terminal_write("Windows");
                    break;
                case MOBILE_DEVICE_BLACKBERRY:
                    terminal_write("BlackBerry");
                    break;
                case MOBILE_DEVICE_SYMBIAN:
                    terminal_write("Symbian");
                    break;
                case MOBILE_DEVICE_CUSTOM:
                    terminal_write("Custom");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", ");
            
            // Print connection type
            switch (devices[i]->connection_type) {
                case MOBILE_CONNECTION_USB:
                    terminal_write("USB");
                    break;
                case MOBILE_CONNECTION_WIFI:
                    terminal_write("Wi-Fi");
                    break;
                case MOBILE_CONNECTION_BLUETOOTH:
                    terminal_write("Bluetooth");
                    break;
                case MOBILE_CONNECTION_CUSTOM:
                    terminal_write("Custom");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", ");
            
            // Print connection state
            switch (devices[i]->connection_state) {
                case MOBILE_CONNECTION_STATE_DISCONNECTED:
                    terminal_write("Disconnected");
                    break;
                case MOBILE_CONNECTION_STATE_CONNECTING:
                    terminal_write("Connecting");
                    break;
                case MOBILE_CONNECTION_STATE_CONNECTED:
                    terminal_write("Connected");
                    break;
                case MOBILE_CONNECTION_STATE_DISCONNECTING:
                    terminal_write("Disconnecting");
                    break;
                case MOBILE_CONNECTION_STATE_ERROR:
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

// Android command handler
int android_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: android <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  adb <id> <command>                      Run an ADB command on an Android device\n");
        terminal_write("  shell <id> <command>                    Run a shell command on an Android device\n");
        terminal_write("  install <id> <apk-path>                 Install an APK on an Android device\n");
        terminal_write("  uninstall <id> <package-name>           Uninstall an app from an Android device\n");
        terminal_write("  push <id> <local-path> <device-path>    Push a file to an Android device\n");
        terminal_write("  pull <id> <device-path> <local-path>    Pull a file from an Android device\n");
        terminal_write("  logcat <id>                             Show the logcat output from an Android device\n");
        terminal_write("  screenshot <id> <local-path>            Take a screenshot of an Android device\n");
        terminal_write("  screenrecord <id> <local-path> <time>   Record the screen of an Android device\n");
        terminal_write("  reboot <id>                             Reboot an Android device\n");
        terminal_write("  reboot-bootloader <id>                  Reboot an Android device into bootloader mode\n");
        terminal_write("  reboot-recovery <id>                    Reboot an Android device into recovery mode\n");
        return 0;
    }
    
    // For now, just forward to the mobile command
    return mobile_command(argc, argv);
}

// iOS command handler
int ios_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: ios <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  install <id> <ipa-path>                 Install an IPA on an iOS device\n");
        terminal_write("  uninstall <id> <bundle-id>              Uninstall an app from an iOS device\n");
        terminal_write("  backup <id> <backup-path>               Backup an iOS device\n");
        terminal_write("  restore <id> <backup-path>              Restore an iOS device from a backup\n");
        terminal_write("  screenshot <id> <local-path>            Take a screenshot of an iOS device\n");
        terminal_write("  reboot <id>                             Reboot an iOS device\n");
        terminal_write("  shutdown <id>                           Shut down an iOS device\n");
        return 0;
    }
    
    // For now, just forward to the mobile command
    return mobile_command(argc, argv);
}

// Mobile sync command handler
int mobile_sync_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: mobile-sync <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  start <id> <type> <direction> <source> <dest>  Start a sync operation\n");
        terminal_write("  stop <id> <type>                              Stop a sync operation\n");
        terminal_write("  status <id> <type>                            Show the status of a sync operation\n");
        terminal_write("  options <id> <type>                           Show the options for a sync operation\n");
        terminal_write("  set-option <id> <type> <option> <value>       Set an option for a sync operation\n");
        terminal_write("Sync types:\n");
        terminal_write("  files                                         File synchronization\n");
        terminal_write("  contacts                                      Contact synchronization\n");
        terminal_write("  calendar                                      Calendar synchronization\n");
        terminal_write("  messages                                      Message synchronization\n");
        terminal_write("  photos                                        Photo synchronization\n");
        terminal_write("  music                                         Music synchronization\n");
        terminal_write("  videos                                        Video synchronization\n");
        terminal_write("  apps                                          App synchronization\n");
        terminal_write("Sync directions:\n");
        terminal_write("  to-device                                     Sync from computer to device\n");
        terminal_write("  from-device                                   Sync from device to computer\n");
        terminal_write("  bidirectional                                 Sync in both directions\n");
        return 0;
    }
    
    // For now, just forward to the mobile command
    return mobile_command(argc, argv);
}

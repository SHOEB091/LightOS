/**
 * LightOS Driver Manager
 * Manages hardware drivers and device detection
 */

#ifndef DRIVER_MANAGER_H
#define DRIVER_MANAGER_H

#include "../kernel/kernel.h"

// Device types
typedef enum {
    DEVICE_UNKNOWN,
    DEVICE_KEYBOARD,
    DEVICE_MOUSE,
    DEVICE_STORAGE,
    DEVICE_DISPLAY,
    DEVICE_NETWORK,
    DEVICE_AUDIO,
    DEVICE_PRINTER,
    DEVICE_SCANNER,
    DEVICE_WEBCAM,
    DEVICE_GAMEPAD,
    DEVICE_TOUCHSCREEN,
    DEVICE_BIOMETRIC,
    DEVICE_OTHER
} DeviceType;

// Device bus types
typedef enum {
    BUS_UNKNOWN,
    BUS_PCI,
    BUS_USB,
    BUS_PS2,
    BUS_SATA,
    BUS_IDE,
    BUS_NVME,
    BUS_BLUETOOTH,
    BUS_WIRELESS,
    BUS_OTHER
} BusType;

// Device status
typedef enum {
    STATUS_UNKNOWN,
    STATUS_DETECTED,
    STATUS_DRIVER_LOADED,
    STATUS_ACTIVE,
    STATUS_ERROR,
    STATUS_DISABLED
} DeviceStatus;

// Device structure
typedef struct {
    char name[64];
    char description[128];
    char manufacturer[64];
    char model[64];
    char serial[32];
    DeviceType type;
    BusType bus;
    DeviceStatus status;
    unsigned int vendor_id;
    unsigned int device_id;
    unsigned int class_id;
    unsigned int subclass_id;
    void* driver;
    void* private_data;
} Device;

// Driver structure
typedef struct {
    char name[64];
    char description[128];
    char version[32];
    DeviceType supported_type;
    unsigned int supported_vendor_id;
    unsigned int supported_device_id;
    unsigned int supported_class_id;
    unsigned int supported_subclass_id;
    int (*probe)(Device* device);
    int (*init)(Device* device);
    int (*remove)(Device* device);
    int (*suspend)(Device* device);
    int (*resume)(Device* device);
    void* private_data;
} Driver;

// Driver manager functions
void driver_manager_init();
int driver_manager_register_driver(Driver* driver);
int driver_manager_unregister_driver(Driver* driver);
Device* driver_manager_detect_devices();
int driver_manager_load_driver(Device* device);
int driver_manager_unload_driver(Device* device);
Device* driver_manager_get_device_by_type(DeviceType type, int index);
Device* driver_manager_get_device_by_name(const char* name);
void driver_manager_print_devices();
void driver_manager_print_drivers();

// Specific device type detection
int detect_keyboard_devices();
int detect_mouse_devices();
int detect_storage_devices();
int detect_display_devices();
int detect_network_devices();
int detect_audio_devices();
int detect_peripheral_devices();

// Hot-plug support
void driver_manager_handle_device_added(Device* device);
void driver_manager_handle_device_removed(Device* device);

#endif /* DRIVER_MANAGER_H */

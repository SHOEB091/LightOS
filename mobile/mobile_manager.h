/**
 * LightOS Mobile
 * Mobile Manager header
 */

#ifndef MOBILE_MANAGER_H
#define MOBILE_MANAGER_H

// Mobile device types
typedef enum {
    MOBILE_DEVICE_ANDROID,
    MOBILE_DEVICE_IOS,
    MOBILE_DEVICE_WINDOWS,
    MOBILE_DEVICE_BLACKBERRY,
    MOBILE_DEVICE_SYMBIAN,
    MOBILE_DEVICE_CUSTOM
} mobile_device_type_t;

// Mobile connection types
typedef enum {
    MOBILE_CONNECTION_USB,
    MOBILE_CONNECTION_WIFI,
    MOBILE_CONNECTION_BLUETOOTH,
    MOBILE_CONNECTION_CUSTOM
} mobile_connection_type_t;

// Mobile connection states
typedef enum {
    MOBILE_CONNECTION_STATE_DISCONNECTED,
    MOBILE_CONNECTION_STATE_CONNECTING,
    MOBILE_CONNECTION_STATE_CONNECTED,
    MOBILE_CONNECTION_STATE_DISCONNECTING,
    MOBILE_CONNECTION_STATE_ERROR
} mobile_connection_state_t;

// Mobile sync types
typedef enum {
    MOBILE_SYNC_FILES,
    MOBILE_SYNC_CONTACTS,
    MOBILE_SYNC_CALENDAR,
    MOBILE_SYNC_MESSAGES,
    MOBILE_SYNC_PHOTOS,
    MOBILE_SYNC_MUSIC,
    MOBILE_SYNC_VIDEOS,
    MOBILE_SYNC_APPS,
    MOBILE_SYNC_CUSTOM
} mobile_sync_type_t;

// Mobile sync directions
typedef enum {
    MOBILE_SYNC_DIRECTION_TO_DEVICE,
    MOBILE_SYNC_DIRECTION_FROM_DEVICE,
    MOBILE_SYNC_DIRECTION_BIDIRECTIONAL
} mobile_sync_direction_t;

// Mobile sync states
typedef enum {
    MOBILE_SYNC_STATE_IDLE,
    MOBILE_SYNC_STATE_SYNCING,
    MOBILE_SYNC_STATE_COMPLETED,
    MOBILE_SYNC_STATE_ERROR
} mobile_sync_state_t;

// Mobile device structure
typedef struct {
    char id[64];
    char name[128];
    mobile_device_type_t type;
    char model[128];
    char manufacturer[128];
    char os_version[64];
    mobile_connection_type_t connection_type;
    mobile_connection_state_t connection_state;
    char serial_number[64];
    char imei[64];
    unsigned int battery_level;
    unsigned int storage_total;
    unsigned int storage_used;
    unsigned int storage_free;
    void* private_data;
} mobile_device_t;

// Mobile sync structure
typedef struct {
    mobile_sync_type_t type;
    mobile_sync_direction_t direction;
    mobile_sync_state_t state;
    char source_path[256];
    char destination_path[256];
    unsigned int total_items;
    unsigned int completed_items;
    unsigned int error_items;
    unsigned int progress;
    void* private_data;
} mobile_sync_t;

// Mobile manager functions
void mobile_manager_init();
int mobile_detect_devices(mobile_device_t** devices, unsigned int* count);
int mobile_connect_device(const char* id, mobile_connection_type_t connection_type);
int mobile_disconnect_device(const char* id);
mobile_device_t* mobile_get_device(const char* id);
int mobile_get_device_info(const char* id, mobile_device_t* device);
int mobile_list_devices(mobile_device_t** devices, unsigned int* count);
int mobile_start_sync(const char* device_id, mobile_sync_type_t type, mobile_sync_direction_t direction, const char* source_path, const char* destination_path);
int mobile_stop_sync(const char* device_id, mobile_sync_type_t type);
int mobile_get_sync_status(const char* device_id, mobile_sync_type_t type, mobile_sync_t* sync);
int mobile_list_files(const char* device_id, const char* path, char*** file_names, unsigned int* count);
int mobile_get_file(const char* device_id, const char* device_path, const char* local_path);
int mobile_put_file(const char* device_id, const char* local_path, const char* device_path);
int mobile_delete_file(const char* device_id, const char* device_path);
int mobile_create_directory(const char* device_id, const char* device_path);
int mobile_delete_directory(const char* device_id, const char* device_path);
int mobile_get_contacts(const char* device_id, char*** contacts, unsigned int* count);
int mobile_add_contact(const char* device_id, const char* name, const char* phone, const char* email);
int mobile_delete_contact(const char* device_id, const char* contact_id);
int mobile_get_calendar_events(const char* device_id, char*** events, unsigned int* count);
int mobile_add_calendar_event(const char* device_id, const char* title, const char* description, const char* start_time, const char* end_time);
int mobile_delete_calendar_event(const char* device_id, const char* event_id);
int mobile_get_messages(const char* device_id, char*** messages, unsigned int* count);
int mobile_send_message(const char* device_id, const char* recipient, const char* message);
int mobile_delete_message(const char* device_id, const char* message_id);
int mobile_get_photos(const char* device_id, char*** photos, unsigned int* count);
int mobile_get_music(const char* device_id, char*** music, unsigned int* count);
int mobile_get_videos(const char* device_id, char*** videos, unsigned int* count);
int mobile_get_apps(const char* device_id, char*** apps, unsigned int* count);
int mobile_install_app(const char* device_id, const char* app_path);
int mobile_uninstall_app(const char* device_id, const char* app_id);
int mobile_launch_app(const char* device_id, const char* app_id);
int mobile_stop_app(const char* device_id, const char* app_id);
int mobile_take_screenshot(const char* device_id, const char* local_path);
int mobile_reboot_device(const char* device_id);
int mobile_shutdown_device(const char* device_id);
int mobile_set_device_volume(const char* device_id, unsigned int volume);
int mobile_get_device_volume(const char* device_id, unsigned int* volume);
int mobile_set_device_brightness(const char* device_id, unsigned int brightness);
int mobile_get_device_brightness(const char* device_id, unsigned int* brightness);
int mobile_enable_wifi(const char* device_id, unsigned int enable);
int mobile_is_wifi_enabled(const char* device_id, unsigned int* enabled);
int mobile_enable_bluetooth(const char* device_id, unsigned int enable);
int mobile_is_bluetooth_enabled(const char* device_id, unsigned int* enabled);
int mobile_enable_airplane_mode(const char* device_id, unsigned int enable);
int mobile_is_airplane_mode_enabled(const char* device_id, unsigned int* enabled);

// Android-specific functions
int android_adb_connect(const char* device_id);
int android_adb_disconnect(const char* device_id);
int android_adb_shell(const char* device_id, const char* command, char* output, unsigned int output_size);
int android_adb_install(const char* device_id, const char* apk_path);
int android_adb_uninstall(const char* device_id, const char* package_name);
int android_adb_push(const char* device_id, const char* local_path, const char* device_path);
int android_adb_pull(const char* device_id, const char* device_path, const char* local_path);

// iOS-specific functions
int ios_connect(const char* device_id);
int ios_disconnect(const char* device_id);
int ios_install_app(const char* device_id, const char* ipa_path);
int ios_uninstall_app(const char* device_id, const char* bundle_id);
int ios_backup(const char* device_id, const char* backup_path);
int ios_restore(const char* device_id, const char* backup_path);

#endif /* MOBILE_MANAGER_H */

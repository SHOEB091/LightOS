/**
 * LightOS Mobile
 * ADB Protocol Support header
 */

#ifndef ADB_SUPPORT_H
#define ADB_SUPPORT_H

#include "../mobile_manager.h"

// ADB connection states
typedef enum {
    ADB_CONNECTION_STATE_DISCONNECTED,
    ADB_CONNECTION_STATE_CONNECTING,
    ADB_CONNECTION_STATE_CONNECTED,
    ADB_CONNECTION_STATE_DISCONNECTING,
    ADB_CONNECTION_STATE_ERROR
} adb_connection_state_t;

// ADB device structure
typedef struct {
    char serial[64];
    char model[128];
    char device[128];
    char product[128];
    adb_connection_state_t state;
    char transport_id[64];
    char usb_port[64];
    char features[256];
} adb_device_t;

// ADB functions
int adb_init();
int adb_detect_devices(mobile_device_t** devices, unsigned int* count);
int adb_connect(const char* serial);
int adb_disconnect(const char* serial);
int adb_shell(const char* serial, const char* command, char* output, unsigned int output_size);
int adb_push(const char* serial, const char* local_path, const char* device_path);
int adb_pull(const char* serial, const char* device_path, const char* local_path);
int adb_install(const char* serial, const char* apk_path);
int adb_uninstall(const char* serial, const char* package_name);
int adb_start_activity(const char* serial, const char* package_name, const char* activity_name);
int adb_stop_activity(const char* serial, const char* package_name);
int adb_list_packages(const char* serial, char*** packages, unsigned int* count);
int adb_get_property(const char* serial, const char* property, char* value, unsigned int value_size);
int adb_set_property(const char* serial, const char* property, const char* value);
int adb_reboot(const char* serial);
int adb_reboot_bootloader(const char* serial);
int adb_reboot_recovery(const char* serial);
int adb_sideload(const char* serial, const char* zip_path);
int adb_root(const char* serial);
int adb_unroot(const char* serial);
int adb_remount(const char* serial);
int adb_get_state(const char* serial, adb_connection_state_t* state);
int adb_wait_for_device(const char* serial);
int adb_forward(const char* serial, const char* local, const char* remote);
int adb_reverse(const char* serial, const char* remote, const char* local);
int adb_tcpip(const char* serial, unsigned int port);
int adb_usb(const char* serial);
int adb_devices(adb_device_t** devices, unsigned int* count);
int adb_connect_network(const char* host, unsigned int port);
int adb_disconnect_network(const char* host, unsigned int port);
int adb_pair(const char* host, unsigned int port, const char* pairing_code);
int adb_get_features(const char* serial, char* features, unsigned int features_size);
int adb_has_feature(const char* serial, const char* feature);
int adb_enable_feature(const char* serial, const char* feature);
int adb_disable_feature(const char* serial, const char* feature);
int adb_sync_start(const char* serial);
int adb_sync_end(const char* serial);
int adb_sync_list(const char* serial, const char* path, char*** files, unsigned int* count);
int adb_sync_push(const char* serial, const char* local_path, const char* device_path);
int adb_sync_pull(const char* serial, const char* device_path, const char* local_path);
int adb_logcat(const char* serial, char* buffer, unsigned int buffer_size);
int adb_bugreport(const char* serial, const char* output_path);
int adb_jdwp(const char* serial, unsigned int** pids, unsigned int* count);
int adb_track_devices(void (*callback)(const char* serial, adb_connection_state_t state));
int adb_track_jdwp(const char* serial, void (*callback)(unsigned int pid));
int adb_screencap(const char* serial, const char* output_path);
int adb_screenrecord(const char* serial, const char* output_path, unsigned int time_limit);
int adb_backup(const char* serial, const char* output_path, unsigned int include_system, unsigned int include_apks);
int adb_restore(const char* serial, const char* input_path);
int adb_get_battery_info(const char* serial, unsigned int* level, unsigned int* temperature, unsigned int* voltage);
int adb_get_network_info(const char* serial, char* info, unsigned int info_size);
int adb_get_cpu_info(const char* serial, char* info, unsigned int info_size);
int adb_get_memory_info(const char* serial, char* info, unsigned int info_size);
int adb_get_disk_info(const char* serial, char* info, unsigned int info_size);
int adb_get_display_info(const char* serial, char* info, unsigned int info_size);
int adb_input_text(const char* serial, const char* text);
int adb_input_keyevent(const char* serial, unsigned int keycode);
int adb_input_tap(const char* serial, unsigned int x, unsigned int y);
int adb_input_swipe(const char* serial, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
int adb_input_drag(const char* serial, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int duration);
int adb_input_press(const char* serial, unsigned int x, unsigned int y, unsigned int duration);
int adb_input_roll(const char* serial, unsigned int dx, unsigned int dy);
int adb_dumpsys(const char* serial, const char* service, char* output, unsigned int output_size);
int adb_dumpstate(const char* serial, char* output, unsigned int output_size);
int adb_dumpcrash(const char* serial, char* output, unsigned int output_size);

#endif /* ADB_SUPPORT_H */

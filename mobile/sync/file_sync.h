/**
 * LightOS Mobile
 * File Synchronization header
 */

#ifndef FILE_SYNC_H
#define FILE_SYNC_H

#include "../mobile_manager.h"

// File sync modes
#define FILE_SYNC_MODE_FULL 0
#define FILE_SYNC_MODE_INCREMENTAL 1
#define FILE_SYNC_MODE_MIRROR 2

// File sync conflict resolution
#define FILE_SYNC_CONFLICT_NEWER 0
#define FILE_SYNC_CONFLICT_LARGER 1
#define FILE_SYNC_CONFLICT_PROMPT 2
#define FILE_SYNC_CONFLICT_SKIP 3
#define FILE_SYNC_CONFLICT_OVERWRITE 4
#define FILE_SYNC_CONFLICT_RENAME 5

// File sync options
typedef struct {
    unsigned int mode;
    unsigned int conflict_resolution;
    unsigned int delete_source;
    unsigned int preserve_attributes;
    unsigned int recursive;
    unsigned int follow_symlinks;
    unsigned int skip_hidden;
    unsigned int skip_system;
    unsigned int skip_temp;
    unsigned int skip_backup;
    unsigned int skip_readonly;
    unsigned int skip_larger_than;
    unsigned int skip_older_than;
    unsigned int skip_newer_than;
    char* include_patterns;
    unsigned int include_pattern_count;
    char* exclude_patterns;
    unsigned int exclude_pattern_count;
} file_sync_options_t;

// File sync functions
int file_sync_init();
int file_sync_start(mobile_device_t* device, mobile_sync_direction_t direction, const char* source_path, const char* destination_path);
int file_sync_stop(mobile_device_t* device);
int file_sync_get_status(mobile_device_t* device, mobile_sync_t* sync);
int file_sync_set_options(mobile_device_t* device, file_sync_options_t* options);
int file_sync_get_options(mobile_device_t* device, file_sync_options_t* options);
int file_sync_reset_options(mobile_device_t* device);
int file_sync_add_include_pattern(mobile_device_t* device, const char* pattern);
int file_sync_add_exclude_pattern(mobile_device_t* device, const char* pattern);
int file_sync_clear_include_patterns(mobile_device_t* device);
int file_sync_clear_exclude_patterns(mobile_device_t* device);
int file_sync_set_conflict_resolution(mobile_device_t* device, unsigned int resolution);
int file_sync_get_conflict_resolution(mobile_device_t* device);
int file_sync_set_mode(mobile_device_t* device, unsigned int mode);
int file_sync_get_mode(mobile_device_t* device);
int file_sync_set_delete_source(mobile_device_t* device, unsigned int delete_source);
int file_sync_get_delete_source(mobile_device_t* device);
int file_sync_set_preserve_attributes(mobile_device_t* device, unsigned int preserve_attributes);
int file_sync_get_preserve_attributes(mobile_device_t* device);
int file_sync_set_recursive(mobile_device_t* device, unsigned int recursive);
int file_sync_get_recursive(mobile_device_t* device);
int file_sync_set_follow_symlinks(mobile_device_t* device, unsigned int follow_symlinks);
int file_sync_get_follow_symlinks(mobile_device_t* device);
int file_sync_set_skip_hidden(mobile_device_t* device, unsigned int skip_hidden);
int file_sync_get_skip_hidden(mobile_device_t* device);
int file_sync_set_skip_system(mobile_device_t* device, unsigned int skip_system);
int file_sync_get_skip_system(mobile_device_t* device);
int file_sync_set_skip_temp(mobile_device_t* device, unsigned int skip_temp);
int file_sync_get_skip_temp(mobile_device_t* device);
int file_sync_set_skip_backup(mobile_device_t* device, unsigned int skip_backup);
int file_sync_get_skip_backup(mobile_device_t* device);
int file_sync_set_skip_readonly(mobile_device_t* device, unsigned int skip_readonly);
int file_sync_get_skip_readonly(mobile_device_t* device);
int file_sync_set_skip_larger_than(mobile_device_t* device, unsigned int skip_larger_than);
int file_sync_get_skip_larger_than(mobile_device_t* device);
int file_sync_set_skip_older_than(mobile_device_t* device, unsigned int skip_older_than);
int file_sync_get_skip_older_than(mobile_device_t* device);
int file_sync_set_skip_newer_than(mobile_device_t* device, unsigned int skip_newer_than);
int file_sync_get_skip_newer_than(mobile_device_t* device);
int file_sync_get_progress(mobile_device_t* device, unsigned int* progress);
int file_sync_get_total_files(mobile_device_t* device, unsigned int* total_files);
int file_sync_get_processed_files(mobile_device_t* device, unsigned int* processed_files);
int file_sync_get_skipped_files(mobile_device_t* device, unsigned int* skipped_files);
int file_sync_get_error_files(mobile_device_t* device, unsigned int* error_files);
int file_sync_get_total_bytes(mobile_device_t* device, unsigned long long* total_bytes);
int file_sync_get_processed_bytes(mobile_device_t* device, unsigned long long* processed_bytes);
int file_sync_get_current_file(mobile_device_t* device, char* file, unsigned int file_size);
int file_sync_get_current_operation(mobile_device_t* device, char* operation, unsigned int operation_size);
int file_sync_get_error_message(mobile_device_t* device, char* message, unsigned int message_size);
int file_sync_get_log(mobile_device_t* device, char* log, unsigned int log_size);
int file_sync_clear_log(mobile_device_t* device);
int file_sync_set_callback(mobile_device_t* device, void (*callback)(mobile_device_t* device, const char* file, unsigned int progress));

#endif /* FILE_SYNC_H */

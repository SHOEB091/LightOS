/**
 * LightOS System
 * Backup Manager header
 */

#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

// Backup type
typedef enum {
    BACKUP_TYPE_FULL,
    BACKUP_TYPE_INCREMENTAL,
    BACKUP_TYPE_DIFFERENTIAL,
    BACKUP_TYPE_CUSTOM
} backup_type_t;

// Backup compression
typedef enum {
    BACKUP_COMPRESSION_NONE,
    BACKUP_COMPRESSION_GZIP,
    BACKUP_COMPRESSION_BZIP2,
    BACKUP_COMPRESSION_XZ,
    BACKUP_COMPRESSION_ZSTD,
    BACKUP_COMPRESSION_CUSTOM
} backup_compression_t;

// Backup encryption
typedef enum {
    BACKUP_ENCRYPTION_NONE,
    BACKUP_ENCRYPTION_AES_128,
    BACKUP_ENCRYPTION_AES_256,
    BACKUP_ENCRYPTION_CUSTOM
} backup_encryption_t;

// Backup state
typedef enum {
    BACKUP_STATE_IDLE,
    BACKUP_STATE_RUNNING,
    BACKUP_STATE_COMPLETED,
    BACKUP_STATE_FAILED
} backup_state_t;

// Backup structure
typedef struct {
    char id[64];
    char name[128];
    char description[256];
    backup_type_t type;
    backup_compression_t compression;
    backup_encryption_t encryption;
    char encryption_key[256];
    char source_path[256];
    char destination_path[256];
    char* include_patterns;
    unsigned int include_pattern_count;
    char* exclude_patterns;
    unsigned int exclude_pattern_count;
    unsigned long long total_size;
    unsigned long long compressed_size;
    unsigned int file_count;
    char creation_date[32];
    backup_state_t state;
    unsigned int progress;
    char error_message[256];
    void* private_data;
} backup_t;

// Backup manager functions
void backup_manager_init();
int backup_create(const char* name, const char* description, backup_type_t type, const char* source_path, const char* destination_path);
int backup_restore(const char* id, const char* destination_path);
int backup_delete(const char* id);
int backup_list(backup_t** backups, unsigned int* count);
backup_t* backup_get(const char* id);
int backup_get_info(const char* id, backup_t* backup);
int backup_verify(const char* id);
int backup_extract(const char* id, const char* destination_path, const char* file_pattern);
int backup_list_files(const char* id, char*** files, unsigned int* count);
int backup_add_include_pattern(const char* id, const char* pattern);
int backup_add_exclude_pattern(const char* id, const char* pattern);
int backup_clear_include_patterns(const char* id);
int backup_clear_exclude_patterns(const char* id);
int backup_set_compression(const char* id, backup_compression_t compression);
int backup_set_encryption(const char* id, backup_encryption_t encryption, const char* encryption_key);
int backup_set_type(const char* id, backup_type_t type);
int backup_get_progress(const char* id, unsigned int* progress);
int backup_cancel(const char* id);
int backup_pause(const char* id);
int backup_resume(const char* id);
int backup_set_backup_directory(const char* directory);
int backup_get_backup_directory(char* directory, unsigned int directory_size);
int backup_set_temp_directory(const char* directory);
int backup_get_temp_directory(char* directory, unsigned int directory_size);
int backup_set_log_file(const char* file);
int backup_get_log_file(char* file, unsigned int file_size);
int backup_set_log_level(unsigned int level);
int backup_get_log_level();
int backup_set_max_backups(unsigned int max_backups);
int backup_get_max_backups();
int backup_set_auto_prune(unsigned int auto_prune);
int backup_get_auto_prune();
int backup_set_auto_verify(unsigned int auto_verify);
int backup_get_auto_verify();
int backup_set_default_compression(backup_compression_t compression);
int backup_get_default_compression();
int backup_set_default_encryption(backup_encryption_t encryption);
int backup_get_default_encryption();
int backup_set_default_type(backup_type_t type);
int backup_get_default_type();
int backup_set_bandwidth_limit(unsigned int limit);
int backup_get_bandwidth_limit();
int backup_set_schedule(const char* id, const char* schedule);
int backup_get_schedule(const char* id, char* schedule, unsigned int schedule_size);
int backup_list_scheduled(backup_t** backups, unsigned int* count);
int backup_cancel_schedule(const char* id);
int backup_run_scheduled();
int backup_import(const char* file);
int backup_export(const char* id, const char* file);
int backup_create_config(const char* config_file);
int backup_parse_config(const char* config_file);
int backup_set_config(const char* key, const char* value);
int backup_get_config(const char* key, char* value, unsigned int value_size);
int backup_set_notification_email(const char* email);
int backup_get_notification_email(char* email, unsigned int email_size);
int backup_send_notification(const char* subject, const char* message);
int backup_set_retention_policy(unsigned int days);
int backup_get_retention_policy();
int backup_prune();
int backup_get_stats(unsigned long long* total_size, unsigned long long* compressed_size, unsigned int* file_count, unsigned int* backup_count);
int backup_get_history(char* history, unsigned int history_size);
int backup_clear_history();
int backup_get_last_backup_time(char* time, unsigned int time_size);
int backup_get_last_restore_time(char* time, unsigned int time_size);
int backup_set_remote_destination(const char* id, const char* url, const char* username, const char* password);
int backup_get_remote_destination(const char* id, char* url, unsigned int url_size, char* username, unsigned int username_size);
int backup_sync_remote(const char* id);
int backup_set_remote_sync_schedule(const char* id, const char* schedule);
int backup_get_remote_sync_schedule(const char* id, char* schedule, unsigned int schedule_size);
int backup_cancel_remote_sync_schedule(const char* id);
int backup_run_remote_sync_scheduled();

#endif /* BACKUP_MANAGER_H */

/**
 * LightOS System
 * Update Manager header
 */

#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

// Update type
typedef enum {
    UPDATE_TYPE_SYSTEM,
    UPDATE_TYPE_KERNEL,
    UPDATE_TYPE_DRIVER,
    UPDATE_TYPE_APPLICATION,
    UPDATE_TYPE_SECURITY,
    UPDATE_TYPE_FIRMWARE,
    UPDATE_TYPE_CUSTOM
} update_type_t;

// Update priority
typedef enum {
    UPDATE_PRIORITY_LOW,
    UPDATE_PRIORITY_MEDIUM,
    UPDATE_PRIORITY_HIGH,
    UPDATE_PRIORITY_CRITICAL
} update_priority_t;

// Update state
typedef enum {
    UPDATE_STATE_AVAILABLE,
    UPDATE_STATE_DOWNLOADING,
    UPDATE_STATE_DOWNLOADED,
    UPDATE_STATE_INSTALLING,
    UPDATE_STATE_INSTALLED,
    UPDATE_STATE_FAILED,
    UPDATE_STATE_REVERTED
} update_state_t;

// Update structure
typedef struct {
    char id[64];
    char name[128];
    char version[32];
    char description[256];
    update_type_t type;
    update_priority_t priority;
    update_state_t state;
    unsigned long long size;
    char release_date[32];
    char download_url[256];
    char checksum[64];
    char signature[256];
    char* dependencies;
    unsigned int dependency_count;
    unsigned int reboot_required;
    unsigned int auto_install;
    unsigned int installed;
    char install_date[32];
    void* private_data;
} update_t;

// Update manager functions
void update_manager_init();
int update_check();
int update_download(const char* id);
int update_install(const char* id);
int update_revert(const char* id);
int update_list(update_t** updates, unsigned int* count);
int update_list_available(update_t** updates, unsigned int* count);
int update_list_installed(update_t** updates, unsigned int* count);
update_t* update_get(const char* id);
int update_get_changelog(const char* id, char* changelog, unsigned int changelog_size);
int update_set_auto_check(unsigned int auto_check);
int update_get_auto_check();
int update_set_auto_download(unsigned int auto_download);
int update_get_auto_download();
int update_set_auto_install(unsigned int auto_install);
int update_get_auto_install();
int update_set_check_interval(unsigned int interval);
int update_get_check_interval();
int update_set_download_timeout(unsigned int timeout);
int update_get_download_timeout();
int update_set_download_retries(unsigned int retries);
int update_get_download_retries();
int update_set_proxy(const char* proxy);
int update_get_proxy(char* proxy, unsigned int proxy_size);
int update_set_update_server(const char* server);
int update_get_update_server(char* server, unsigned int server_size);
int update_set_update_channel(const char* channel);
int update_get_update_channel(char* channel, unsigned int channel_size);
int update_set_verify_signatures(unsigned int verify);
int update_get_verify_signatures();
int update_set_allow_untrusted(unsigned int allow);
int update_get_allow_untrusted();
int update_set_backup_before_update(unsigned int backup);
int update_get_backup_before_update();
int update_set_backup_directory(const char* directory);
int update_get_backup_directory(char* directory, unsigned int directory_size);
int update_set_log_file(const char* file);
int update_get_log_file(char* file, unsigned int file_size);
int update_set_log_level(unsigned int level);
int update_get_log_level();
int update_create_backup(const char* id, const char* backup_file);
int update_restore_backup(const char* backup_file);
int update_list_backups(char*** backup_files, unsigned int* count);
int update_delete_backup(const char* backup_file);
int update_verify_backup(const char* backup_file);
int update_extract_backup(const char* backup_file, const char* destination);
int update_create_config(const char* config_file);
int update_parse_config(const char* config_file);
int update_set_config(const char* key, const char* value);
int update_get_config(const char* key, char* value, unsigned int value_size);
int update_add_repository(const char* name, const char* url);
int update_remove_repository(const char* name);
int update_enable_repository(const char* name);
int update_disable_repository(const char* name);
int update_list_repositories(char*** repositories, unsigned int* count);
int update_get_repository_url(const char* name, char* url, unsigned int url_size);
int update_set_repository_url(const char* name, const char* url);
int update_is_repository_enabled(const char* name);
int update_import_key(const char* key_file);
int update_export_key(const char* key_id, const char* key_file);
int update_list_keys(char*** key_ids, unsigned int* count);
int update_remove_key(const char* key_id);
int update_verify_signature(const char* file, const char* signature);
int update_sign_file(const char* file, const char* key_id, const char* output_file);
int update_get_system_info(char* info, unsigned int info_size);
int update_get_update_history(char* history, unsigned int history_size);
int update_clear_history();
int update_get_last_check_time(char* time, unsigned int time_size);
int update_get_last_update_time(char* time, unsigned int time_size);
int update_set_notification_email(const char* email);
int update_get_notification_email(char* email, unsigned int email_size);
int update_send_notification(const char* subject, const char* message);
int update_schedule_check(const char* time);
int update_schedule_install(const char* id, const char* time);
int update_cancel_scheduled_check();
int update_cancel_scheduled_install(const char* id);
int update_get_scheduled_check(char* time, unsigned int time_size);
int update_get_scheduled_installs(char*** ids, char*** times, unsigned int* count);
int update_set_bandwidth_limit(unsigned int limit);
int update_get_bandwidth_limit();
int update_set_concurrent_downloads(unsigned int count);
int update_get_concurrent_downloads();
int update_pause_downloads();
int update_resume_downloads();
int update_cancel_downloads();
int update_get_download_progress(const char* id, unsigned int* progress);
int update_get_install_progress(const char* id, unsigned int* progress);
int update_set_install_timeout(unsigned int timeout);
int update_get_install_timeout();
int update_set_reboot_delay(unsigned int delay);
int update_get_reboot_delay();
int update_reboot_system();
int update_shutdown_system();

#endif /* UPDATE_MANAGER_H */

/**
 * LightOS System
 * Monitor Manager header
 */

#ifndef MONITOR_MANAGER_H
#define MONITOR_MANAGER_H

// Resource type
typedef enum {
    RESOURCE_TYPE_CPU,
    RESOURCE_TYPE_MEMORY,
    RESOURCE_TYPE_DISK,
    RESOURCE_TYPE_NETWORK,
    RESOURCE_TYPE_PROCESS,
    RESOURCE_TYPE_SYSTEM,
    RESOURCE_TYPE_CUSTOM
} resource_type_t;

// Alert level
typedef enum {
    ALERT_LEVEL_INFO,
    ALERT_LEVEL_WARNING,
    ALERT_LEVEL_ERROR,
    ALERT_LEVEL_CRITICAL
} alert_level_t;

// Alert state
typedef enum {
    ALERT_STATE_ACTIVE,
    ALERT_STATE_ACKNOWLEDGED,
    ALERT_STATE_RESOLVED
} alert_state_t;

// Resource structure
typedef struct {
    char id[64];
    char name[128];
    resource_type_t type;
    char description[256];
    unsigned int enabled;
    unsigned int interval;
    unsigned int threshold_warning;
    unsigned int threshold_error;
    unsigned int threshold_critical;
    unsigned int current_value;
    unsigned int peak_value;
    unsigned int average_value;
    char unit[32];
    char* history;
    unsigned int history_size;
    unsigned int history_capacity;
    char last_update[32];
    void* private_data;
} resource_t;

// Alert structure
typedef struct {
    char id[64];
    char resource_id[64];
    alert_level_t level;
    alert_state_t state;
    char message[256];
    char creation_time[32];
    char acknowledgement_time[32];
    char resolution_time[32];
    char* notes;
    unsigned int note_count;
    void* private_data;
} alert_t;

// Monitor manager functions
void monitor_manager_init();
int monitor_add_resource(const char* name, resource_type_t type, const char* description, unsigned int interval);
int monitor_remove_resource(const char* id);
int monitor_enable_resource(const char* id);
int monitor_disable_resource(const char* id);
resource_t* monitor_get_resource(const char* id);
int monitor_list_resources(resource_t** resources, unsigned int* count);
int monitor_set_threshold(const char* id, unsigned int warning, unsigned int error, unsigned int critical);
int monitor_get_threshold(const char* id, unsigned int* warning, unsigned int* error, unsigned int* critical);
int monitor_set_interval(const char* id, unsigned int interval);
int monitor_get_interval(const char* id);
int monitor_get_value(const char* id, unsigned int* value);
int monitor_get_peak(const char* id, unsigned int* peak);
int monitor_get_average(const char* id, unsigned int* average);
int monitor_get_history(const char* id, unsigned int** history, unsigned int* count);
int monitor_clear_history(const char* id);
int monitor_set_history_capacity(const char* id, unsigned int capacity);
int monitor_get_history_capacity(const char* id);
int monitor_add_alert(const char* resource_id, alert_level_t level, const char* message);
int monitor_remove_alert(const char* id);
int monitor_acknowledge_alert(const char* id);
int monitor_resolve_alert(const char* id);
alert_t* monitor_get_alert(const char* id);
int monitor_list_alerts(alert_t** alerts, unsigned int* count);
int monitor_list_active_alerts(alert_t** alerts, unsigned int* count);
int monitor_add_note(const char* alert_id, const char* note);
int monitor_get_notes(const char* alert_id, char*** notes, unsigned int* count);
int monitor_clear_alerts();
int monitor_set_alert_handler(void (*handler)(alert_t* alert));
int monitor_set_notification_email(const char* email);
int monitor_get_notification_email(char* email, unsigned int email_size);
int monitor_send_notification(const char* subject, const char* message);
int monitor_set_log_file(const char* file);
int monitor_get_log_file(char* file, unsigned int file_size);
int monitor_set_log_level(unsigned int level);
int monitor_get_log_level();
int monitor_create_report(const char* file);
int monitor_get_cpu_info(char* info, unsigned int info_size);
int monitor_get_memory_info(char* info, unsigned int info_size);
int monitor_get_disk_info(char* info, unsigned int info_size);
int monitor_get_network_info(char* info, unsigned int info_size);
int monitor_get_process_info(unsigned int pid, char* info, unsigned int info_size);
int monitor_get_system_info(char* info, unsigned int info_size);
int monitor_get_uptime(unsigned int* uptime);
int monitor_get_load_average(float* load1, float* load5, float* load15);
int monitor_get_cpu_usage(unsigned int* usage);
int monitor_get_memory_usage(unsigned int* usage);
int monitor_get_disk_usage(const char* mount_point, unsigned int* usage);
int monitor_get_network_usage(const char* interface, unsigned int* rx_bytes, unsigned int* tx_bytes);
int monitor_get_process_count(unsigned int* count);
int monitor_get_thread_count(unsigned int* count);
int monitor_get_process_list(unsigned int** pids, unsigned int* count);
int monitor_kill_process(unsigned int pid);
int monitor_set_process_priority(unsigned int pid, int priority);
int monitor_get_process_priority(unsigned int pid, int* priority);
int monitor_get_process_name(unsigned int pid, char* name, unsigned int name_size);
int monitor_get_process_user(unsigned int pid, char* user, unsigned int user_size);
int monitor_get_process_memory(unsigned int pid, unsigned int* memory);
int monitor_get_process_cpu(unsigned int pid, unsigned int* cpu);
int monitor_get_process_state(unsigned int pid, char* state, unsigned int state_size);
int monitor_get_process_start_time(unsigned int pid, char* time, unsigned int time_size);
int monitor_get_process_command(unsigned int pid, char* command, unsigned int command_size);
int monitor_get_process_environment(unsigned int pid, char* environment, unsigned int environment_size);
int monitor_get_process_files(unsigned int pid, char*** files, unsigned int* count);
int monitor_get_process_connections(unsigned int pid, char*** connections, unsigned int* count);
int monitor_set_auto_restart(const char* process_name, unsigned int auto_restart);
int monitor_get_auto_restart(const char* process_name);
int monitor_set_max_restarts(const char* process_name, unsigned int max_restarts);
int monitor_get_max_restarts(const char* process_name);
int monitor_set_restart_interval(const char* process_name, unsigned int interval);
int monitor_get_restart_interval(const char* process_name);
int monitor_set_max_cpu(const char* process_name, unsigned int max_cpu);
int monitor_get_max_cpu(const char* process_name);
int monitor_set_max_memory(const char* process_name, unsigned int max_memory);
int monitor_get_max_memory(const char* process_name);
int monitor_set_max_files(const char* process_name, unsigned int max_files);
int monitor_get_max_files(const char* process_name);
int monitor_set_max_connections(const char* process_name, unsigned int max_connections);
int monitor_get_max_connections(const char* process_name);
int monitor_create_config(const char* config_file);
int monitor_parse_config(const char* config_file);
int monitor_set_config(const char* key, const char* value);
int monitor_get_config(const char* key, char* value, unsigned int value_size);

#endif /* MONITOR_MANAGER_H */

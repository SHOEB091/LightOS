/**
 * LightOS Containerization
 * Container Manager header
 */

#ifndef CONTAINER_MANAGER_H
#define CONTAINER_MANAGER_H

// Container types
typedef enum {
    CONTAINER_TYPE_DOCKER,
    CONTAINER_TYPE_LXC,
    CONTAINER_TYPE_PODMAN,
    CONTAINER_TYPE_CUSTOM
} container_type_t;

// Container states
typedef enum {
    CONTAINER_STATE_CREATED,
    CONTAINER_STATE_RUNNING,
    CONTAINER_STATE_PAUSED,
    CONTAINER_STATE_STOPPED,
    CONTAINER_STATE_EXITED,
    CONTAINER_STATE_ERROR
} container_state_t;

// Container network modes
typedef enum {
    CONTAINER_NETWORK_BRIDGE,
    CONTAINER_NETWORK_HOST,
    CONTAINER_NETWORK_NONE,
    CONTAINER_NETWORK_CUSTOM
} container_network_mode_t;

// Container volume structure
typedef struct {
    char host_path[256];
    char container_path[256];
    unsigned int read_only;
} container_volume_t;

// Container port mapping structure
typedef struct {
    unsigned int host_port;
    unsigned int container_port;
    unsigned int protocol; // 0 = TCP, 1 = UDP
} container_port_mapping_t;

// Container environment variable structure
typedef struct {
    char name[64];
    char value[256];
} container_env_var_t;

// Container structure
typedef struct {
    char id[64];
    char name[64];
    container_type_t type;
    container_state_t state;
    char image[256];
    char command[256];
    container_network_mode_t network_mode;
    char network_name[64];
    container_volume_t* volumes;
    unsigned int volume_count;
    container_port_mapping_t* port_mappings;
    unsigned int port_mapping_count;
    container_env_var_t* env_vars;
    unsigned int env_var_count;
    unsigned int privileged;
    char* labels;
    unsigned int label_count;
    void* private_data;
} container_t;

// Container manager functions
void container_manager_init();
int container_create(const char* name, container_type_t type, const char* image, const char* command);
int container_destroy(const char* name);
int container_start(const char* name);
int container_stop(const char* name);
int container_pause(const char* name);
int container_resume(const char* name);
int container_restart(const char* name);
container_t* container_get(const char* name);
container_state_t container_get_state(const char* name);
int container_add_volume(const char* name, const char* host_path, const char* container_path, unsigned int read_only);
int container_remove_volume(const char* name, const char* container_path);
int container_add_port_mapping(const char* name, unsigned int host_port, unsigned int container_port, unsigned int protocol);
int container_remove_port_mapping(const char* name, unsigned int container_port);
int container_add_env_var(const char* name, const char* env_name, const char* env_value);
int container_remove_env_var(const char* name, const char* env_name);
int container_set_network_mode(const char* name, container_network_mode_t mode, const char* network_name);
int container_set_privileged(const char* name, unsigned int privileged);
int container_list(container_t** containers, unsigned int* count);
int container_exec(const char* name, const char* command, char* output, unsigned int output_size);
int container_logs(const char* name, char* logs, unsigned int logs_size);
int container_stats(const char* name, unsigned int* cpu_usage, unsigned long long* memory_usage, unsigned long long* network_rx, unsigned long long* network_tx);
int container_commit(const char* name, const char* repository, const char* tag);
int container_export(const char* name, const char* output_path);
int container_import(const char* input_path, const char* repository, const char* tag);

// Container image functions
int container_image_list(char*** image_names, unsigned int* count);
int container_image_pull(const char* repository, const char* tag);
int container_image_remove(const char* repository, const char* tag);
int container_image_inspect(const char* repository, const char* tag, char* info, unsigned int info_size);

// Container network functions
int container_network_list(char*** network_names, unsigned int* count);
int container_network_create(const char* name, const char* subnet, const char* gateway);
int container_network_remove(const char* name);
int container_network_inspect(const char* name, char* info, unsigned int info_size);

// Container volume functions
int container_volume_list(char*** volume_names, unsigned int* count);
int container_volume_create(const char* name, const char* path);
int container_volume_remove(const char* name);
int container_volume_inspect(const char* name, char* info, unsigned int info_size);

#endif /* CONTAINER_MANAGER_H */

/**
 * LightOS Cloud
 * Cloud Manager header
 */

#ifndef CLOUD_MANAGER_H
#define CLOUD_MANAGER_H

// Cloud provider types
typedef enum {
    CLOUD_PROVIDER_AWS,
    CLOUD_PROVIDER_AZURE,
    CLOUD_PROVIDER_GCP,
    CLOUD_PROVIDER_DIGITALOCEAN,
    CLOUD_PROVIDER_LINODE,
    CLOUD_PROVIDER_VULTR,
    CLOUD_PROVIDER_CUSTOM
} cloud_provider_t;

// Cloud resource types
typedef enum {
    CLOUD_RESOURCE_INSTANCE,
    CLOUD_RESOURCE_STORAGE,
    CLOUD_RESOURCE_DATABASE,
    CLOUD_RESOURCE_NETWORK,
    CLOUD_RESOURCE_LOADBALANCER,
    CLOUD_RESOURCE_FUNCTION,
    CLOUD_RESOURCE_CONTAINER,
    CLOUD_RESOURCE_KUBERNETES,
    CLOUD_RESOURCE_CUSTOM
} cloud_resource_type_t;

// Cloud instance types
typedef enum {
    CLOUD_INSTANCE_GENERAL,
    CLOUD_INSTANCE_COMPUTE,
    CLOUD_INSTANCE_MEMORY,
    CLOUD_INSTANCE_STORAGE,
    CLOUD_INSTANCE_GPU,
    CLOUD_INSTANCE_FPGA,
    CLOUD_INSTANCE_CUSTOM
} cloud_instance_t;

// Cloud storage types
typedef enum {
    CLOUD_STORAGE_BLOCK,
    CLOUD_STORAGE_OBJECT,
    CLOUD_STORAGE_FILE,
    CLOUD_STORAGE_ARCHIVE,
    CLOUD_STORAGE_CUSTOM
} cloud_storage_t;

// Cloud database types
typedef enum {
    CLOUD_DATABASE_MYSQL,
    CLOUD_DATABASE_POSTGRESQL,
    CLOUD_DATABASE_MONGODB,
    CLOUD_DATABASE_REDIS,
    CLOUD_DATABASE_ELASTICSEARCH,
    CLOUD_DATABASE_CUSTOM
} cloud_database_t;

// Cloud network types
typedef enum {
    CLOUD_NETWORK_VPC,
    CLOUD_NETWORK_SUBNET,
    CLOUD_NETWORK_SECURITY_GROUP,
    CLOUD_NETWORK_ROUTE_TABLE,
    CLOUD_NETWORK_CUSTOM
} cloud_network_t;

// Cloud resource state
typedef enum {
    CLOUD_RESOURCE_STATE_CREATING,
    CLOUD_RESOURCE_STATE_RUNNING,
    CLOUD_RESOURCE_STATE_STOPPING,
    CLOUD_RESOURCE_STATE_STOPPED,
    CLOUD_RESOURCE_STATE_DELETING,
    CLOUD_RESOURCE_STATE_ERROR
} cloud_resource_state_t;

// Cloud provider structure
typedef struct {
    cloud_provider_t type;
    char name[64];
    char region[64];
    char access_key[256];
    char secret_key[256];
    char endpoint[256];
    void* private_data;
} cloud_provider_config_t;

// Cloud resource structure
typedef struct {
    char id[64];
    char name[64];
    cloud_resource_type_t type;
    cloud_resource_state_t state;
    cloud_provider_t provider;
    char region[64];
    char zone[64];
    char* tags;
    unsigned int tag_count;
    void* resource_data;
    void* private_data;
} cloud_resource_t;

// Cloud instance structure
typedef struct {
    cloud_instance_t type;
    char instance_type[64];
    unsigned int vcpus;
    unsigned long long memory;
    unsigned long long storage;
    char image_id[64];
    char* security_groups;
    unsigned int security_group_count;
    char* network_interfaces;
    unsigned int network_interface_count;
    char key_name[64];
    char* user_data;
} cloud_instance_data_t;

// Cloud storage structure
typedef struct {
    cloud_storage_t type;
    unsigned long long size;
    unsigned int iops;
    unsigned int throughput;
    unsigned int encrypted;
    char* snapshot_id;
} cloud_storage_data_t;

// Cloud database structure
typedef struct {
    cloud_database_t type;
    char engine[64];
    char engine_version[64];
    unsigned int instance_count;
    char instance_class[64];
    unsigned long long storage;
    unsigned int storage_iops;
    unsigned int storage_throughput;
    unsigned int multi_az;
    unsigned int publicly_accessible;
    char master_username[64];
    char master_password[64];
    char* parameter_group;
    char* option_group;
} cloud_database_data_t;

// Cloud network structure
typedef struct {
    cloud_network_t type;
    char cidr_block[64];
    unsigned int is_public;  // Renamed from 'public' to avoid C++ keyword conflict
    char* route_tables;
    unsigned int route_table_count;
    char* security_groups;
    unsigned int security_group_count;
} cloud_network_data_t;

// Cloud manager functions
void cloud_manager_init();
int cloud_provider_add(cloud_provider_t type, const char* name, const char* region, const char* access_key, const char* secret_key, const char* endpoint);
int cloud_provider_remove(const char* name);
cloud_provider_config_t* cloud_provider_get(const char* name);
int cloud_provider_list(cloud_provider_config_t** providers, unsigned int* count);
int cloud_provider_test_connection(const char* name);

// Cloud resource functions
int cloud_resource_create(const char* provider_name, const char* resource_name, cloud_resource_type_t type, void* resource_data);
int cloud_resource_delete(const char* provider_name, const char* resource_id);
int cloud_resource_start(const char* provider_name, const char* resource_id);
int cloud_resource_stop(const char* provider_name, const char* resource_id);
int cloud_resource_restart(const char* provider_name, const char* resource_id);
cloud_resource_t* cloud_resource_get(const char* provider_name, const char* resource_id);
int cloud_resource_list(const char* provider_name, cloud_resource_t** resources, unsigned int* count);
int cloud_resource_add_tag(const char* provider_name, const char* resource_id, const char* key, const char* value);
int cloud_resource_remove_tag(const char* provider_name, const char* resource_id, const char* key);

// Cloud instance functions
int cloud_instance_create(const char* provider_name, const char* instance_name, cloud_instance_data_t* instance_data);
int cloud_instance_delete(const char* provider_name, const char* instance_id);
int cloud_instance_start(const char* provider_name, const char* instance_id);
int cloud_instance_stop(const char* provider_name, const char* instance_id);
int cloud_instance_restart(const char* provider_name, const char* instance_id);
int cloud_instance_resize(const char* provider_name, const char* instance_id, const char* instance_type);
int cloud_instance_get_console_output(const char* provider_name, const char* instance_id, char* output, unsigned int output_size);
int cloud_instance_get_screenshot(const char* provider_name, const char* instance_id, void* buffer, unsigned int buffer_size);
int cloud_instance_list(const char* provider_name, cloud_resource_t** instances, unsigned int* count);

// Cloud storage functions
int cloud_storage_create(const char* provider_name, const char* storage_name, cloud_storage_data_t* storage_data);
int cloud_storage_delete(const char* provider_name, const char* storage_id);
int cloud_storage_attach(const char* provider_name, const char* storage_id, const char* instance_id, const char* device);
int cloud_storage_detach(const char* provider_name, const char* storage_id, const char* instance_id);
int cloud_storage_snapshot(const char* provider_name, const char* storage_id, const char* snapshot_name);
int cloud_storage_restore(const char* provider_name, const char* storage_id, const char* snapshot_id);
int cloud_storage_resize(const char* provider_name, const char* storage_id, unsigned long long size);
int cloud_storage_list(const char* provider_name, cloud_resource_t** storages, unsigned int* count);

// Cloud database functions
int cloud_database_create(const char* provider_name, const char* database_name, cloud_database_data_t* database_data);
int cloud_database_delete(const char* provider_name, const char* database_id);
int cloud_database_start(const char* provider_name, const char* database_id);
int cloud_database_stop(const char* provider_name, const char* database_id);
int cloud_database_restart(const char* provider_name, const char* database_id);
int cloud_database_snapshot(const char* provider_name, const char* database_id, const char* snapshot_name);
int cloud_database_restore(const char* provider_name, const char* database_id, const char* snapshot_id);
int cloud_database_resize(const char* provider_name, const char* database_id, const char* instance_class, unsigned long long storage);
int cloud_database_list(const char* provider_name, cloud_resource_t** databases, unsigned int* count);

// Cloud network functions
int cloud_network_create(const char* provider_name, const char* network_name, cloud_network_data_t* network_data);
int cloud_network_delete(const char* provider_name, const char* network_id);
int cloud_network_add_route(const char* provider_name, const char* network_id, const char* cidr_block, const char* gateway_id);
int cloud_network_remove_route(const char* provider_name, const char* network_id, const char* cidr_block);
int cloud_network_add_security_group_rule(const char* provider_name, const char* security_group_id, const char* protocol, unsigned int from_port, unsigned int to_port, const char* cidr_block);
int cloud_network_remove_security_group_rule(const char* provider_name, const char* security_group_id, const char* protocol, unsigned int from_port, unsigned int to_port, const char* cidr_block);
int cloud_network_list(const char* provider_name, cloud_resource_t** networks, unsigned int* count);

// Cloud utility functions
const char* cloud_provider_to_string(cloud_provider_t provider);
cloud_provider_t cloud_string_to_provider(const char* provider);
const char* cloud_resource_type_to_string(cloud_resource_type_t type);
cloud_resource_type_t cloud_string_to_resource_type(const char* type);
const char* cloud_resource_state_to_string(cloud_resource_state_t state);
cloud_resource_state_t cloud_string_to_resource_state(const char* state);
const char* cloud_instance_type_to_string(cloud_instance_t type);
cloud_instance_t cloud_string_to_instance_type(const char* type);
const char* cloud_storage_type_to_string(cloud_storage_t type);
cloud_storage_t cloud_string_to_storage_type(const char* type);
const char* cloud_database_type_to_string(cloud_database_t type);
cloud_database_t cloud_string_to_database_type(const char* type);
const char* cloud_network_type_to_string(cloud_network_t type);
cloud_network_t cloud_string_to_network_type(const char* type);

#endif /* CLOUD_MANAGER_H */

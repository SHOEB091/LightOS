/**
 * LightOS Virtualization
 * Virtual Machine Manager header
 */

#ifndef VM_MANAGER_H
#define VM_MANAGER_H

// VM types
typedef enum {
    VM_TYPE_KVM,
    VM_TYPE_QEMU,
    VM_TYPE_VIRTUALBOX,
    VM_TYPE_VMWARE,
    VM_TYPE_XEN,
    VM_TYPE_HYPER_V,
    VM_TYPE_CUSTOM
} vm_type_t;

// VM states
typedef enum {
    VM_STATE_STOPPED,
    VM_STATE_RUNNING,
    VM_STATE_PAUSED,
    VM_STATE_SUSPENDED,
    VM_STATE_ERROR
} vm_state_t;

// VM disk types
typedef enum {
    VM_DISK_TYPE_RAW,
    VM_DISK_TYPE_QCOW2,
    VM_DISK_TYPE_VDI,
    VM_DISK_TYPE_VMDK,
    VM_DISK_TYPE_VHD
} vm_disk_type_t;

// VM network types
typedef enum {
    VM_NETWORK_TYPE_NAT,
    VM_NETWORK_TYPE_BRIDGE,
    VM_NETWORK_TYPE_HOST_ONLY,
    VM_NETWORK_TYPE_INTERNAL
} vm_network_type_t;

// VM disk structure
typedef struct {
    char path[256];
    vm_disk_type_t type;
    unsigned long long size;
    unsigned int boot;
    unsigned int read_only;
} vm_disk_t;

// VM network interface structure
typedef struct {
    char name[32];
    vm_network_type_t type;
    char mac_address[18];
    char bridge[32];
} vm_network_interface_t;

// VM structure
typedef struct {
    char name[64];
    vm_type_t type;
    vm_state_t state;
    unsigned int vcpus;
    unsigned long long memory;
    vm_disk_t* disks;
    unsigned int disk_count;
    vm_network_interface_t* network_interfaces;
    unsigned int network_interface_count;
    char* custom_config;
    void* private_data;
} vm_t;

// VM manager functions
void vm_manager_init();
int vm_create(const char* name, vm_type_t type, unsigned int vcpus, unsigned long long memory);
int vm_destroy(const char* name);
int vm_start(const char* name);
int vm_stop(const char* name);
int vm_pause(const char* name);
int vm_resume(const char* name);
int vm_suspend(const char* name);
int vm_reset(const char* name);
vm_t* vm_get(const char* name);
vm_state_t vm_get_state(const char* name);
int vm_set_vcpus(const char* name, unsigned int vcpus);
int vm_set_memory(const char* name, unsigned long long memory);
int vm_add_disk(const char* name, const char* path, vm_disk_type_t type, unsigned long long size, unsigned int boot, unsigned int read_only);
int vm_remove_disk(const char* name, const char* path);
int vm_add_network_interface(const char* name, const char* interface_name, vm_network_type_t type, const char* mac_address, const char* bridge);
int vm_remove_network_interface(const char* name, const char* interface_name);
int vm_set_custom_config(const char* name, const char* config);
int vm_get_custom_config(const char* name, char* config, unsigned int config_size);
int vm_list(vm_t** vms, unsigned int* count);
int vm_snapshot_create(const char* vm_name, const char* snapshot_name, const char* description);
int vm_snapshot_delete(const char* vm_name, const char* snapshot_name);
int vm_snapshot_restore(const char* vm_name, const char* snapshot_name);
int vm_snapshot_list(const char* vm_name, char*** snapshot_names, unsigned int* count);
int vm_clone(const char* source_name, const char* target_name);
int vm_migrate(const char* name, const char* target_host, unsigned int live);
int vm_console(const char* name);
int vm_monitor(const char* name, const char* command, char* response, unsigned int response_size);
int vm_get_performance_stats(const char* name, unsigned int* cpu_usage, unsigned long long* memory_usage, unsigned long long* disk_read, unsigned long long* disk_write, unsigned long long* network_rx, unsigned long long* network_tx);

// VM utility functions
const char* vm_type_to_string(vm_type_t type);
vm_type_t vm_string_to_type(const char* type);
const char* vm_state_to_string(vm_state_t state);
vm_state_t vm_string_to_state(const char* state);
const char* vm_disk_type_to_string(vm_disk_type_t type);
vm_disk_type_t vm_string_to_disk_type(const char* type);
const char* vm_network_type_to_string(vm_network_type_t type);
vm_network_type_t vm_string_to_network_type(const char* type);
int vm_generate_mac_address(char* mac_address, unsigned int mac_address_size);

// VM host functions
int vm_host_get_capabilities(unsigned int* kvm_supported, unsigned int* vt_x_supported, unsigned int* amd_v_supported, unsigned int* max_vcpus, unsigned long long* max_memory);
int vm_host_get_performance_stats(unsigned int* cpu_usage, unsigned long long* memory_usage, unsigned long long* disk_read, unsigned long long* disk_write, unsigned long long* network_rx, unsigned long long* network_tx);
int vm_host_get_network_interfaces(char*** interface_names, unsigned int* count);
int vm_host_get_storage_pools(char*** pool_names, unsigned int* count);
int vm_host_get_storage_pool_info(const char* pool_name, unsigned long long* total_size, unsigned long long* free_size, char* path, unsigned int path_size);

#endif /* VM_MANAGER_H */

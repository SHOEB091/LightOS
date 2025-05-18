/**
 * LightOS Containerization
 * LXC Support header
 */

#ifndef LXC_SUPPORT_H
#define LXC_SUPPORT_H

#include "../container_manager.h"

// LXC-specific functions
void lxc_init();
int lxc_create_container(container_t* container);
int lxc_destroy_container(container_t* container);
int lxc_start_container(container_t* container);
int lxc_stop_container(container_t* container);
int lxc_pause_container(container_t* container);
int lxc_resume_container(container_t* container);
int lxc_restart_container(container_t* container);
int lxc_exec_command(container_t* container, const char* command, char* output, unsigned int output_size);
int lxc_get_logs(container_t* container, char* logs, unsigned int logs_size);
int lxc_get_stats(container_t* container, unsigned int* cpu_usage, unsigned long long* memory_usage, unsigned long long* network_rx, unsigned long long* network_tx);
int lxc_snapshot_container(container_t* container, const char* snapshot_name);
int lxc_restore_snapshot(container_t* container, const char* snapshot_name);
int lxc_list_snapshots(container_t* container, char*** snapshot_names, unsigned int* count);
int lxc_delete_snapshot(container_t* container, const char* snapshot_name);
int lxc_clone_container(container_t* container, const char* new_name);

// LXC template functions
int lxc_list_templates(char*** template_names, unsigned int* count);
int lxc_create_container_from_template(const char* name, const char* template_name, const char* template_args);

// LXC network functions
int lxc_list_networks(char*** network_names, unsigned int* count);
int lxc_create_network(const char* name, const char* subnet, const char* gateway);
int lxc_remove_network(const char* name);
int lxc_inspect_network(const char* name, char* info, unsigned int info_size);

#endif /* LXC_SUPPORT_H */

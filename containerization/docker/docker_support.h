/**
 * LightOS Containerization
 * Docker Support header
 */

#ifndef DOCKER_SUPPORT_H
#define DOCKER_SUPPORT_H

#include "../container_manager.h"

// Docker-specific functions
void docker_init();
int docker_create_container(container_t* container);
int docker_destroy_container(container_t* container);
int docker_start_container(container_t* container);
int docker_stop_container(container_t* container);
int docker_pause_container(container_t* container);
int docker_resume_container(container_t* container);
int docker_restart_container(container_t* container);
int docker_exec_command(container_t* container, const char* command, char* output, unsigned int output_size);
int docker_get_logs(container_t* container, char* logs, unsigned int logs_size);
int docker_get_stats(container_t* container, unsigned int* cpu_usage, unsigned long long* memory_usage, unsigned long long* network_rx, unsigned long long* network_tx);
int docker_commit_container(container_t* container, const char* repository, const char* tag);
int docker_export_container(container_t* container, const char* output_path);
int docker_import_image(const char* input_path, const char* repository, const char* tag);

// Docker image functions
int docker_list_images(char*** image_names, unsigned int* count);
int docker_pull_image(const char* repository, const char* tag);
int docker_remove_image(const char* repository, const char* tag);
int docker_inspect_image(const char* repository, const char* tag, char* info, unsigned int info_size);

// Docker network functions
int docker_list_networks(char*** network_names, unsigned int* count);
int docker_create_network(const char* name, const char* subnet, const char* gateway);
int docker_remove_network(const char* name);
int docker_inspect_network(const char* name, char* info, unsigned int info_size);

// Docker volume functions
int docker_list_volumes(char*** volume_names, unsigned int* count);
int docker_create_volume(const char* name, const char* path);
int docker_remove_volume(const char* name);
int docker_inspect_volume(const char* name, char* info, unsigned int info_size);

#endif /* DOCKER_SUPPORT_H */

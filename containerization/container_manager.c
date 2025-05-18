/**
 * LightOS Containerization
 * Container Manager implementation
 */

#include "container_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "docker/docker_support.h"
#include "lxc/lxc_support.h"

// Maximum number of containers
#define MAX_CONTAINERS 100

// Container array
static container_t* containers[MAX_CONTAINERS];
static unsigned int container_count = 0;

// Initialize the container manager
void container_manager_init() {
    terminal_write("Initializing container manager...\n");
    
    // Clear the container array
    for (int i = 0; i < MAX_CONTAINERS; i++) {
        containers[i] = NULL;
    }
    
    container_count = 0;
    
    // Initialize container backends
    docker_init();
    lxc_init();
    
    terminal_write("Container manager initialized\n");
}

// Create a container
int container_create(const char* name, container_type_t type, const char* image, const char* command) {
    if (!name || !image) {
        terminal_write("Error: Container name and image cannot be NULL\n");
        return -1;
    }
    
    // Check if a container with the same name already exists
    for (unsigned int i = 0; i < container_count; i++) {
        if (strcmp(containers[i]->name, name) == 0) {
            terminal_write("Error: Container with name '");
            terminal_write(name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Check if we have room for another container
    if (container_count >= MAX_CONTAINERS) {
        terminal_write("Error: Maximum number of containers reached\n");
        return -1;
    }
    
    // Allocate memory for the container
    container_t* container = (container_t*)allocate_block();
    
    if (!container) {
        terminal_write("Error: Failed to allocate memory for container\n");
        return -1;
    }
    
    // Initialize the container
    // In a real system, we would generate a unique ID
    strcpy(container->id, "cont-");
    for (int i = 5; i < 13; i++) {
        container->id[i] = '0' + (i % 10);
    }
    container->id[13] = '\0';
    
    strcpy(container->name, name);
    container->type = type;
    container->state = CONTAINER_STATE_CREATED;
    strcpy(container->image, image);
    
    if (command) {
        strcpy(container->command, command);
    } else {
        container->command[0] = '\0';
    }
    
    container->network_mode = CONTAINER_NETWORK_BRIDGE;
    container->network_name[0] = '\0';
    
    // Allocate memory for volumes
    container->volumes = NULL;
    container->volume_count = 0;
    
    // Allocate memory for port mappings
    container->port_mappings = NULL;
    container->port_mapping_count = 0;
    
    // Allocate memory for environment variables
    container->env_vars = NULL;
    container->env_var_count = 0;
    
    container->privileged = 0;
    container->labels = NULL;
    container->label_count = 0;
    container->private_data = NULL;
    
    // Add the container to the array
    containers[container_count++] = container;
    
    // Create the container based on its type
    int result = -1;
    
    switch (container->type) {
        case CONTAINER_TYPE_DOCKER:
            result = docker_create_container(container);
            break;
        
        case CONTAINER_TYPE_LXC:
            result = lxc_create_container(container);
            break;
        
        case CONTAINER_TYPE_PODMAN:
            // Not implemented yet
            terminal_write("Error: Podman containers not implemented yet\n");
            break;
        
        case CONTAINER_TYPE_CUSTOM:
            // Not implemented yet
            terminal_write("Error: Custom containers not implemented yet\n");
            break;
        
        default:
            terminal_write("Error: Unknown container type\n");
            break;
    }
    
    if (result != 0) {
        // Failed to create the container, remove it from the array
        containers[--container_count] = NULL;
        free_block(container);
        return -1;
    }
    
    terminal_write("Created container '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Destroy a container
int container_destroy(const char* name) {
    if (!name) {
        terminal_write("Error: Container name cannot be NULL\n");
        return -1;
    }
    
    // Find the container
    int index = -1;
    for (unsigned int i = 0; i < container_count; i++) {
        if (strcmp(containers[i]->name, name) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: Container '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    container_t* container = containers[index];
    
    // Destroy the container based on its type
    int result = -1;
    
    switch (container->type) {
        case CONTAINER_TYPE_DOCKER:
            result = docker_destroy_container(container);
            break;
        
        case CONTAINER_TYPE_LXC:
            result = lxc_destroy_container(container);
            break;
        
        case CONTAINER_TYPE_PODMAN:
            // Not implemented yet
            terminal_write("Error: Podman containers not implemented yet\n");
            break;
        
        case CONTAINER_TYPE_CUSTOM:
            // Not implemented yet
            terminal_write("Error: Custom containers not implemented yet\n");
            break;
        
        default:
            terminal_write("Error: Unknown container type\n");
            break;
    }
    
    if (result != 0) {
        return -1;
    }
    
    // Free the container's resources
    if (container->volumes) {
        free_blocks(container->volumes, (container->volume_count * sizeof(container_volume_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    if (container->port_mappings) {
        free_blocks(container->port_mappings, (container->port_mapping_count * sizeof(container_port_mapping_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    if (container->env_vars) {
        free_blocks(container->env_vars, (container->env_var_count * sizeof(container_env_var_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    if (container->labels) {
        free_blocks(container->labels, (container->label_count * sizeof(char*) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    // Remove the container from the array
    free_block(container);
    
    for (unsigned int i = index; i < container_count - 1; i++) {
        containers[i] = containers[i + 1];
    }
    
    containers[--container_count] = NULL;
    
    terminal_write("Destroyed container '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Start a container
int container_start(const char* name) {
    container_t* container = container_get(name);
    
    if (!container) {
        terminal_write("Error: Container '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Start the container based on its type
    int result = -1;
    
    switch (container->type) {
        case CONTAINER_TYPE_DOCKER:
            result = docker_start_container(container);
            break;
        
        case CONTAINER_TYPE_LXC:
            result = lxc_start_container(container);
            break;
        
        case CONTAINER_TYPE_PODMAN:
            // Not implemented yet
            terminal_write("Error: Podman containers not implemented yet\n");
            break;
        
        case CONTAINER_TYPE_CUSTOM:
            // Not implemented yet
            terminal_write("Error: Custom containers not implemented yet\n");
            break;
        
        default:
            terminal_write("Error: Unknown container type\n");
            break;
    }
    
    if (result == 0) {
        container->state = CONTAINER_STATE_RUNNING;
        terminal_write("Started container '");
        terminal_write(name);
        terminal_write("'\n");
    }
    
    return result;
}

// Get a container by name
container_t* container_get(const char* name) {
    if (!name) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < container_count; i++) {
        if (strcmp(containers[i]->name, name) == 0) {
            return containers[i];
        }
    }
    
    return NULL;
}

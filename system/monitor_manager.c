/**
 * LightOS System
 * Monitor Manager implementation
 */

#include "monitor_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "../kernel/process.h"
#include "../kernel/filesystem.h"
#include "../networking/network.h"

// Maximum number of resources
#define MAX_RESOURCES 100

// Maximum number of alerts
#define MAX_ALERTS 1000

// Resource array
static resource_t* resources[MAX_RESOURCES];
static unsigned int resource_count = 0;

// Alert array
static alert_t* alerts[MAX_ALERTS];
static unsigned int alert_count = 0;

// Alert handler
static void (*alert_handler)(alert_t* alert) = NULL;

// Configuration
static char log_file[256] = "/var/log/monitor.log";
static unsigned int log_level = 1;
static char notification_email[256] = "";

// Initialize the monitor manager
void monitor_manager_init() {
    terminal_write("Initializing monitor manager...\n");
    
    // Clear the resource array
    for (int i = 0; i < MAX_RESOURCES; i++) {
        resources[i] = NULL;
    }
    
    resource_count = 0;
    
    // Clear the alert array
    for (int i = 0; i < MAX_ALERTS; i++) {
        alerts[i] = NULL;
    }
    
    alert_count = 0;
    
    // Parse the configuration file
    monitor_parse_config("/etc/monitor/monitor.conf");
    
    // Add default resources
    monitor_add_resource("CPU Usage", RESOURCE_TYPE_CPU, "CPU usage percentage", 5);
    monitor_add_resource("Memory Usage", RESOURCE_TYPE_MEMORY, "Memory usage percentage", 5);
    monitor_add_resource("Disk Usage", RESOURCE_TYPE_DISK, "Disk usage percentage", 60);
    monitor_add_resource("Network Usage", RESOURCE_TYPE_NETWORK, "Network usage in bytes per second", 5);
    monitor_add_resource("Process Count", RESOURCE_TYPE_PROCESS, "Number of running processes", 10);
    monitor_add_resource("System Load", RESOURCE_TYPE_SYSTEM, "System load average", 10);
    
    terminal_write("Monitor manager initialized\n");
}

// Add a resource to monitor
int monitor_add_resource(const char* name, resource_type_t type, const char* description, unsigned int interval) {
    if (!name) {
        terminal_write("Error: Resource name cannot be NULL\n");
        return -1;
    }
    
    // Check if we have room for another resource
    if (resource_count >= MAX_RESOURCES) {
        terminal_write("Error: Maximum number of resources reached\n");
        return -1;
    }
    
    // Generate a unique ID
    char id[64];
    sprintf(id, "resource-%u", resource_count + 1);
    
    // Allocate memory for the resource
    resource_t* resource = (resource_t*)allocate_block();
    
    if (!resource) {
        terminal_write("Error: Failed to allocate memory for resource\n");
        return -1;
    }
    
    // Initialize the resource
    strncpy(resource->id, id, sizeof(resource->id) - 1);
    resource->id[sizeof(resource->id) - 1] = '\0';
    strncpy(resource->name, name, sizeof(resource->name) - 1);
    resource->name[sizeof(resource->name) - 1] = '\0';
    resource->type = type;
    
    if (description) {
        strncpy(resource->description, description, sizeof(resource->description) - 1);
        resource->description[sizeof(resource->description) - 1] = '\0';
    } else {
        resource->description[0] = '\0';
    }
    
    resource->enabled = 1;
    resource->interval = interval;
    resource->threshold_warning = 70;
    resource->threshold_error = 85;
    resource->threshold_critical = 95;
    resource->current_value = 0;
    resource->peak_value = 0;
    resource->average_value = 0;
    
    // Set the unit based on the resource type
    switch (type) {
        case RESOURCE_TYPE_CPU:
        case RESOURCE_TYPE_MEMORY:
        case RESOURCE_TYPE_DISK:
            strcpy(resource->unit, "%");
            break;
        
        case RESOURCE_TYPE_NETWORK:
            strcpy(resource->unit, "B/s");
            break;
        
        case RESOURCE_TYPE_PROCESS:
            strcpy(resource->unit, "count");
            break;
        
        case RESOURCE_TYPE_SYSTEM:
            strcpy(resource->unit, "load");
            break;
        
        default:
            strcpy(resource->unit, "");
            break;
    }
    
    resource->history = NULL;
    resource->history_size = 0;
    resource->history_capacity = 100;
    
    // Set the last update time
    // In a real system, we would get the current date and time
    strcpy(resource->last_update, "2023-01-01 00:00:00");
    
    resource->private_data = NULL;
    
    // Add the resource to the array
    resources[resource_count++] = resource;
    
    terminal_write("Added resource '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Remove a resource
int monitor_remove_resource(const char* id) {
    if (!id) {
        terminal_write("Error: Resource ID cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    int index = -1;
    for (unsigned int i = 0; i < resource_count; i++) {
        if (strcmp(resources[i]->id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    terminal_write("Removing resource '");
    terminal_write(resources[index]->name);
    terminal_write("'...\n");
    
    // Free the resource's memory
    if (resources[index]->history) {
        free_blocks(resources[index]->history, (resources[index]->history_capacity * sizeof(unsigned int) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    if (resources[index]->private_data) {
        free_block(resources[index]->private_data);
    }
    
    free_block(resources[index]);
    
    // Remove the resource from the array
    for (unsigned int i = index; i < resource_count - 1; i++) {
        resources[i] = resources[i + 1];
    }
    
    resources[--resource_count] = NULL;
    
    terminal_write("Resource removed\n");
    
    return 0;
}

// Enable a resource
int monitor_enable_resource(const char* id) {
    if (!id) {
        terminal_write("Error: Resource ID cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Enable the resource
    resource->enabled = 1;
    
    terminal_write("Resource '");
    terminal_write(resource->name);
    terminal_write("' enabled\n");
    
    return 0;
}

// Disable a resource
int monitor_disable_resource(const char* id) {
    if (!id) {
        terminal_write("Error: Resource ID cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Disable the resource
    resource->enabled = 0;
    
    terminal_write("Resource '");
    terminal_write(resource->name);
    terminal_write("' disabled\n");
    
    return 0;
}

// Get a resource by ID
resource_t* monitor_get_resource(const char* id) {
    if (!id) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < resource_count; i++) {
        if (strcmp(resources[i]->id, id) == 0) {
            return resources[i];
        }
    }
    
    return NULL;
}

// List all resources
int monitor_list_resources(resource_t** resources_out, unsigned int* count) {
    if (!resources_out || !count) {
        terminal_write("Error: Resources and count cannot be NULL\n");
        return -1;
    }
    
    *resources_out = resources;
    *count = resource_count;
    
    return 0;
}

// Set the threshold for a resource
int monitor_set_threshold(const char* id, unsigned int warning, unsigned int error, unsigned int critical) {
    if (!id) {
        terminal_write("Error: Resource ID cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Set the thresholds
    resource->threshold_warning = warning;
    resource->threshold_error = error;
    resource->threshold_critical = critical;
    
    terminal_write("Thresholds set for resource '");
    terminal_write(resource->name);
    terminal_write("'\n");
    
    return 0;
}

// Get the threshold for a resource
int monitor_get_threshold(const char* id, unsigned int* warning, unsigned int* error, unsigned int* critical) {
    if (!id || !warning || !error || !critical) {
        terminal_write("Error: Resource ID, warning, error, and critical cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Get the thresholds
    *warning = resource->threshold_warning;
    *error = resource->threshold_error;
    *critical = resource->threshold_critical;
    
    return 0;
}

// Set the monitoring interval for a resource
int monitor_set_interval(const char* id, unsigned int interval) {
    if (!id) {
        terminal_write("Error: Resource ID cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Set the interval
    resource->interval = interval;
    
    terminal_write("Interval set for resource '");
    terminal_write(resource->name);
    terminal_write("'\n");
    
    return 0;
}

// Get the monitoring interval for a resource
int monitor_get_interval(const char* id) {
    if (!id) {
        terminal_write("Error: Resource ID cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    return resource->interval;
}

// Get the current value of a resource
int monitor_get_value(const char* id, unsigned int* value) {
    if (!id || !value) {
        terminal_write("Error: Resource ID and value cannot be NULL\n");
        return -1;
    }
    
    // Find the resource
    resource_t* resource = monitor_get_resource(id);
    
    if (!resource) {
        terminal_write("Error: Resource '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Update the resource value
    // In a real system, we would get the actual value from the system
    // For now, just simulate a random value
    unsigned int new_value = 0;
    
    switch (resource->type) {
        case RESOURCE_TYPE_CPU:
            new_value = 50; // 50% CPU usage
            break;
        
        case RESOURCE_TYPE_MEMORY:
            new_value = 60; // 60% memory usage
            break;
        
        case RESOURCE_TYPE_DISK:
            new_value = 70; // 70% disk usage
            break;
        
        case RESOURCE_TYPE_NETWORK:
            new_value = 1024 * 1024; // 1 MB/s network usage
            break;
        
        case RESOURCE_TYPE_PROCESS:
            new_value = 100; // 100 processes
            break;
        
        case RESOURCE_TYPE_SYSTEM:
            new_value = 1; // Load average of 1
            break;
        
        default:
            new_value = 0;
            break;
    }
    
    // Update the resource
    resource->current_value = new_value;
    
    // Update the peak value
    if (new_value > resource->peak_value) {
        resource->peak_value = new_value;
    }
    
    // Update the average value
    // In a real system, we would calculate a proper average
    resource->average_value = (resource->average_value + new_value) / 2;
    
    // Update the history
    if (resource->history) {
        // Shift the history values
        for (unsigned int i = resource->history_size - 1; i > 0; i--) {
            ((unsigned int*)resource->history)[i] = ((unsigned int*)resource->history)[i - 1];
        }
        
        // Add the new value
        ((unsigned int*)resource->history)[0] = new_value;
        
        // Update the history size if needed
        if (resource->history_size < resource->history_capacity) {
            resource->history_size++;
        }
    } else if (resource->history_capacity > 0) {
        // Allocate memory for the history
        resource->history = allocate_blocks((resource->history_capacity * sizeof(unsigned int) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
        
        if (resource->history) {
            // Add the new value
            ((unsigned int*)resource->history)[0] = new_value;
            resource->history_size = 1;
        }
    }
    
    // Set the last update time
    // In a real system, we would get the current date and time
    strcpy(resource->last_update, "2023-01-01 00:00:00");
    
    // Check if we need to create an alert
    if (resource->enabled) {
        if (new_value >= resource->threshold_critical) {
            monitor_add_alert(resource->id, ALERT_LEVEL_CRITICAL, "Critical threshold exceeded");
        } else if (new_value >= resource->threshold_error) {
            monitor_add_alert(resource->id, ALERT_LEVEL_ERROR, "Error threshold exceeded");
        } else if (new_value >= resource->threshold_warning) {
            monitor_add_alert(resource->id, ALERT_LEVEL_WARNING, "Warning threshold exceeded");
        }
    }
    
    *value = new_value;
    
    return 0;
}

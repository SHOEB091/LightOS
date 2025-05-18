/**
 * LightOS Performance
 * Performance monitor implementation
 */

#include "performance_monitor.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../kernel/process.h"
#include "../libc/string.h"

// Maximum number of performance events
#define MAX_PERFORMANCE_EVENTS 1024

// Performance counters
static performance_counter_t counters[PERF_COUNTER_COUNT];

// Performance events
static performance_event_t* events = NULL;
static int event_count = 0;
static int event_index = 0;

// Performance thresholds
static performance_threshold_t thresholds[PERF_COUNTER_COUNT];

// Performance monitor state
static int monitor_running = 0;

// Initialize the performance monitor
void performance_monitor_init() {
    terminal_write("Initializing performance monitor...\n");
    
    // Initialize counters
    counters[PERF_COUNTER_CPU_USAGE].type = PERF_COUNTER_CPU_USAGE;
    strcpy(counters[PERF_COUNTER_CPU_USAGE].name, "CPU Usage");
    counters[PERF_COUNTER_CPU_USAGE].value = 0;
    counters[PERF_COUNTER_CPU_USAGE].min = 0;
    counters[PERF_COUNTER_CPU_USAGE].max = 0;
    counters[PERF_COUNTER_CPU_USAGE].total = 0;
    counters[PERF_COUNTER_CPU_USAGE].count = 0;
    
    counters[PERF_COUNTER_MEMORY_USAGE].type = PERF_COUNTER_MEMORY_USAGE;
    strcpy(counters[PERF_COUNTER_MEMORY_USAGE].name, "Memory Usage");
    counters[PERF_COUNTER_MEMORY_USAGE].value = 0;
    counters[PERF_COUNTER_MEMORY_USAGE].min = 0;
    counters[PERF_COUNTER_MEMORY_USAGE].max = 0;
    counters[PERF_COUNTER_MEMORY_USAGE].total = 0;
    counters[PERF_COUNTER_MEMORY_USAGE].count = 0;
    
    counters[PERF_COUNTER_DISK_IO].type = PERF_COUNTER_DISK_IO;
    strcpy(counters[PERF_COUNTER_DISK_IO].name, "Disk I/O");
    counters[PERF_COUNTER_DISK_IO].value = 0;
    counters[PERF_COUNTER_DISK_IO].min = 0;
    counters[PERF_COUNTER_DISK_IO].max = 0;
    counters[PERF_COUNTER_DISK_IO].total = 0;
    counters[PERF_COUNTER_DISK_IO].count = 0;
    
    counters[PERF_COUNTER_NETWORK_IO].type = PERF_COUNTER_NETWORK_IO;
    strcpy(counters[PERF_COUNTER_NETWORK_IO].name, "Network I/O");
    counters[PERF_COUNTER_NETWORK_IO].value = 0;
    counters[PERF_COUNTER_NETWORK_IO].min = 0;
    counters[PERF_COUNTER_NETWORK_IO].max = 0;
    counters[PERF_COUNTER_NETWORK_IO].total = 0;
    counters[PERF_COUNTER_NETWORK_IO].count = 0;
    
    counters[PERF_COUNTER_PROCESS_COUNT].type = PERF_COUNTER_PROCESS_COUNT;
    strcpy(counters[PERF_COUNTER_PROCESS_COUNT].name, "Process Count");
    counters[PERF_COUNTER_PROCESS_COUNT].value = 0;
    counters[PERF_COUNTER_PROCESS_COUNT].min = 0;
    counters[PERF_COUNTER_PROCESS_COUNT].max = 0;
    counters[PERF_COUNTER_PROCESS_COUNT].total = 0;
    counters[PERF_COUNTER_PROCESS_COUNT].count = 0;
    
    counters[PERF_COUNTER_THREAD_COUNT].type = PERF_COUNTER_THREAD_COUNT;
    strcpy(counters[PERF_COUNTER_THREAD_COUNT].name, "Thread Count");
    counters[PERF_COUNTER_THREAD_COUNT].value = 0;
    counters[PERF_COUNTER_THREAD_COUNT].min = 0;
    counters[PERF_COUNTER_THREAD_COUNT].max = 0;
    counters[PERF_COUNTER_THREAD_COUNT].total = 0;
    counters[PERF_COUNTER_THREAD_COUNT].count = 0;
    
    counters[PERF_COUNTER_CONTEXT_SWITCHES].type = PERF_COUNTER_CONTEXT_SWITCHES;
    strcpy(counters[PERF_COUNTER_CONTEXT_SWITCHES].name, "Context Switches");
    counters[PERF_COUNTER_CONTEXT_SWITCHES].value = 0;
    counters[PERF_COUNTER_CONTEXT_SWITCHES].min = 0;
    counters[PERF_COUNTER_CONTEXT_SWITCHES].max = 0;
    counters[PERF_COUNTER_CONTEXT_SWITCHES].total = 0;
    counters[PERF_COUNTER_CONTEXT_SWITCHES].count = 0;
    
    counters[PERF_COUNTER_INTERRUPTS].type = PERF_COUNTER_INTERRUPTS;
    strcpy(counters[PERF_COUNTER_INTERRUPTS].name, "Interrupts");
    counters[PERF_COUNTER_INTERRUPTS].value = 0;
    counters[PERF_COUNTER_INTERRUPTS].min = 0;
    counters[PERF_COUNTER_INTERRUPTS].max = 0;
    counters[PERF_COUNTER_INTERRUPTS].total = 0;
    counters[PERF_COUNTER_INTERRUPTS].count = 0;
    
    counters[PERF_COUNTER_SYSTEM_CALLS].type = PERF_COUNTER_SYSTEM_CALLS;
    strcpy(counters[PERF_COUNTER_SYSTEM_CALLS].name, "System Calls");
    counters[PERF_COUNTER_SYSTEM_CALLS].value = 0;
    counters[PERF_COUNTER_SYSTEM_CALLS].min = 0;
    counters[PERF_COUNTER_SYSTEM_CALLS].max = 0;
    counters[PERF_COUNTER_SYSTEM_CALLS].total = 0;
    counters[PERF_COUNTER_SYSTEM_CALLS].count = 0;
    
    counters[PERF_COUNTER_PAGE_FAULTS].type = PERF_COUNTER_PAGE_FAULTS;
    strcpy(counters[PERF_COUNTER_PAGE_FAULTS].name, "Page Faults");
    counters[PERF_COUNTER_PAGE_FAULTS].value = 0;
    counters[PERF_COUNTER_PAGE_FAULTS].min = 0;
    counters[PERF_COUNTER_PAGE_FAULTS].max = 0;
    counters[PERF_COUNTER_PAGE_FAULTS].total = 0;
    counters[PERF_COUNTER_PAGE_FAULTS].count = 0;
    
    counters[PERF_COUNTER_CACHE_HITS].type = PERF_COUNTER_CACHE_HITS;
    strcpy(counters[PERF_COUNTER_CACHE_HITS].name, "Cache Hits");
    counters[PERF_COUNTER_CACHE_HITS].value = 0;
    counters[PERF_COUNTER_CACHE_HITS].min = 0;
    counters[PERF_COUNTER_CACHE_HITS].max = 0;
    counters[PERF_COUNTER_CACHE_HITS].total = 0;
    counters[PERF_COUNTER_CACHE_HITS].count = 0;
    
    counters[PERF_COUNTER_CACHE_MISSES].type = PERF_COUNTER_CACHE_MISSES;
    strcpy(counters[PERF_COUNTER_CACHE_MISSES].name, "Cache Misses");
    counters[PERF_COUNTER_CACHE_MISSES].value = 0;
    counters[PERF_COUNTER_CACHE_MISSES].min = 0;
    counters[PERF_COUNTER_CACHE_MISSES].max = 0;
    counters[PERF_COUNTER_CACHE_MISSES].total = 0;
    counters[PERF_COUNTER_CACHE_MISSES].count = 0;
    
    // Initialize thresholds
    for (int i = 0; i < PERF_COUNTER_COUNT; i++) {
        thresholds[i].counter_type = i;
        thresholds[i].threshold = 0;
        thresholds[i].callback = NULL;
    }
    
    // Allocate memory for events
    events = (performance_event_t*)allocate_blocks((MAX_PERFORMANCE_EVENTS * sizeof(performance_event_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!events) {
        terminal_write("Error: Failed to allocate memory for performance events\n");
        return;
    }
    
    // Clear the events
    memset(events, 0, MAX_PERFORMANCE_EVENTS * sizeof(performance_event_t));
    event_count = 0;
    event_index = 0;
    
    monitor_running = 0;
    
    terminal_write("Performance monitor initialized\n");
}

// Start the performance monitor
void performance_monitor_start() {
    if (monitor_running) {
        return;
    }
    
    terminal_write("Starting performance monitor...\n");
    
    // Reset counters
    performance_monitor_reset();
    
    monitor_running = 1;
    
    terminal_write("Performance monitor started\n");
}

// Stop the performance monitor
void performance_monitor_stop() {
    if (!monitor_running) {
        return;
    }
    
    terminal_write("Stopping performance monitor...\n");
    
    monitor_running = 0;
    
    terminal_write("Performance monitor stopped\n");
}

// Check if the performance monitor is running
int performance_monitor_is_running() {
    return monitor_running;
}

// Update performance counters
void performance_monitor_update() {
    if (!monitor_running) {
        return;
    }
    
    // Update CPU usage
    // In a real system, we would read CPU usage from the hardware
    counters[PERF_COUNTER_CPU_USAGE].value = 50; // 50%
    
    // Update memory usage
    unsigned int total, used, free;
    memory_stats(&total, &used, &free);
    counters[PERF_COUNTER_MEMORY_USAGE].value = used;
    
    // Update process count
    // In a real system, we would count the number of processes
    counters[PERF_COUNTER_PROCESS_COUNT].value = 10;
    
    // Update thread count
    // In a real system, we would count the number of threads
    counters[PERF_COUNTER_THREAD_COUNT].value = 20;
    
    // Update other counters
    // ...
    
    // Update statistics for each counter
    for (int i = 0; i < PERF_COUNTER_COUNT; i++) {
        // Update min
        if (counters[i].count == 0 || counters[i].value < counters[i].min) {
            counters[i].min = counters[i].value;
        }
        
        // Update max
        if (counters[i].count == 0 || counters[i].value > counters[i].max) {
            counters[i].max = counters[i].value;
        }
        
        // Update total and count
        counters[i].total += counters[i].value;
        counters[i].count++;
        
        // Check threshold
        if (thresholds[i].threshold > 0 && counters[i].value >= thresholds[i].threshold && thresholds[i].callback) {
            thresholds[i].callback(&counters[i]);
        }
    }
}

// Reset performance counters
void performance_monitor_reset() {
    for (int i = 0; i < PERF_COUNTER_COUNT; i++) {
        counters[i].value = 0;
        counters[i].min = 0;
        counters[i].max = 0;
        counters[i].total = 0;
        counters[i].count = 0;
    }
    
    event_count = 0;
    event_index = 0;
}

// Print performance counters
void performance_monitor_print_counters() {
    terminal_write("Performance Counters:\n");
    terminal_write("---------------------\n");
    
    for (int i = 0; i < PERF_COUNTER_COUNT; i++) {
        terminal_write(counters[i].name);
        terminal_write(": ");
        
        // Convert value to string
        char value_str[32];
        unsigned long long value = counters[i].value;
        int idx = 0;
        
        if (value == 0) {
            value_str[idx++] = '0';
        } else {
            while (value > 0) {
                value_str[idx++] = '0' + (value % 10);
                value /= 10;
            }
        }
        
        value_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = value_str[j];
            value_str[j] = value_str[idx - j - 1];
            value_str[idx - j - 1] = tmp;
        }
        
        terminal_write(value_str);
        
        // Print average if count > 0
        if (counters[i].count > 0) {
            terminal_write(" (avg: ");
            
            // Convert average to string
            unsigned long long avg = counters[i].total / counters[i].count;
            idx = 0;
            
            if (avg == 0) {
                value_str[idx++] = '0';
            } else {
                while (avg > 0) {
                    value_str[idx++] = '0' + (avg % 10);
                    avg /= 10;
                }
            }
            
            value_str[idx] = '\0';
            
            // Reverse the string
            for (int j = 0; j < idx / 2; j++) {
                char tmp = value_str[j];
                value_str[j] = value_str[idx - j - 1];
                value_str[idx - j - 1] = tmp;
            }
            
            terminal_write(value_str);
            terminal_write(")");
        }
        
        terminal_write("\n");
    }
}

// Print performance events
void performance_monitor_print_events() {
    terminal_write("Performance Events:\n");
    terminal_write("------------------\n");
    
    if (event_count == 0) {
        terminal_write("No events recorded\n");
        return;
    }
    
    // Determine how many events to print
    int count = event_count;
    
    if (count > MAX_PERFORMANCE_EVENTS) {
        count = MAX_PERFORMANCE_EVENTS;
    }
    
    // Print the events
    for (int i = 0; i < count; i++) {
        int idx = (event_index - count + i + MAX_PERFORMANCE_EVENTS) % MAX_PERFORMANCE_EVENTS;
        
        // Print event type
        switch (events[idx].type) {
            case PERF_EVENT_PROCESS_CREATED:
                terminal_write("Process Created: ");
                break;
            case PERF_EVENT_PROCESS_TERMINATED:
                terminal_write("Process Terminated: ");
                break;
            case PERF_EVENT_THREAD_CREATED:
                terminal_write("Thread Created: ");
                break;
            case PERF_EVENT_THREAD_TERMINATED:
                terminal_write("Thread Terminated: ");
                break;
            case PERF_EVENT_MEMORY_ALLOCATED:
                terminal_write("Memory Allocated: ");
                break;
            case PERF_EVENT_MEMORY_FREED:
                terminal_write("Memory Freed: ");
                break;
            case PERF_EVENT_DISK_READ:
                terminal_write("Disk Read: ");
                break;
            case PERF_EVENT_DISK_WRITE:
                terminal_write("Disk Write: ");
                break;
            case PERF_EVENT_NETWORK_SEND:
                terminal_write("Network Send: ");
                break;
            case PERF_EVENT_NETWORK_RECEIVE:
                terminal_write("Network Receive: ");
                break;
            case PERF_EVENT_SYSTEM_CALL:
                terminal_write("System Call: ");
                break;
            case PERF_EVENT_INTERRUPT:
                terminal_write("Interrupt: ");
                break;
            default:
                terminal_write("Unknown Event: ");
                break;
        }
        
        // Print event description
        terminal_write(events[idx].description);
        terminal_write("\n");
    }
}

// Get a performance counter
performance_counter_t* performance_monitor_get_counter(performance_counter_type_t type) {
    if (type >= 0 && type < PERF_COUNTER_COUNT) {
        return &counters[type];
    }
    
    return NULL;
}

// Add a performance event
int performance_monitor_add_event(performance_event_type_t type, unsigned int process_id, unsigned int thread_id, unsigned long long value, const char* description) {
    if (!monitor_running) {
        return -1;
    }
    
    if (!events) {
        return -1;
    }
    
    // Add the event
    events[event_index].type = type;
    events[event_index].process_id = process_id;
    events[event_index].thread_id = thread_id;
    events[event_index].timestamp = 0; // In a real system, we would get the current time
    events[event_index].value = value;
    
    if (description) {
        strncpy(events[event_index].description, description, 63);
        events[event_index].description[63] = '\0';
    } else {
        events[event_index].description[0] = '\0';
    }
    
    // Update the event index and count
    event_index = (event_index + 1) % MAX_PERFORMANCE_EVENTS;
    
    if (event_count < MAX_PERFORMANCE_EVENTS) {
        event_count++;
    }
    
    return 0;
}

// Set a performance threshold
int performance_monitor_set_threshold(performance_counter_type_t counter_type, unsigned long long threshold, int (*callback)(performance_counter_t* counter)) {
    if (counter_type < 0 || counter_type >= PERF_COUNTER_COUNT) {
        return -1;
    }
    
    thresholds[counter_type].threshold = threshold;
    thresholds[counter_type].callback = callback;
    
    return 0;
}

// Clear a performance threshold
int performance_monitor_clear_threshold(performance_counter_type_t counter_type) {
    if (counter_type < 0 || counter_type >= PERF_COUNTER_COUNT) {
        return -1;
    }
    
    thresholds[counter_type].threshold = 0;
    thresholds[counter_type].callback = NULL;
    
    return 0;
}

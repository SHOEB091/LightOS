/**
 * LightOS Performance
 * Performance monitor header
 */

#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

// Performance counter types
typedef enum {
    PERF_COUNTER_CPU_USAGE,
    PERF_COUNTER_MEMORY_USAGE,
    PERF_COUNTER_DISK_IO,
    PERF_COUNTER_NETWORK_IO,
    PERF_COUNTER_PROCESS_COUNT,
    PERF_COUNTER_THREAD_COUNT,
    PERF_COUNTER_CONTEXT_SWITCHES,
    PERF_COUNTER_INTERRUPTS,
    PERF_COUNTER_SYSTEM_CALLS,
    PERF_COUNTER_PAGE_FAULTS,
    PERF_COUNTER_CACHE_HITS,
    PERF_COUNTER_CACHE_MISSES,
    PERF_COUNTER_COUNT
} performance_counter_type_t;

// Performance counter structure
typedef struct {
    performance_counter_type_t type;
    char name[32];
    unsigned long long value;
    unsigned long long min;
    unsigned long long max;
    unsigned long long total;
    unsigned int count;
} performance_counter_t;

// Performance event types
typedef enum {
    PERF_EVENT_PROCESS_CREATED,
    PERF_EVENT_PROCESS_TERMINATED,
    PERF_EVENT_THREAD_CREATED,
    PERF_EVENT_THREAD_TERMINATED,
    PERF_EVENT_MEMORY_ALLOCATED,
    PERF_EVENT_MEMORY_FREED,
    PERF_EVENT_DISK_READ,
    PERF_EVENT_DISK_WRITE,
    PERF_EVENT_NETWORK_SEND,
    PERF_EVENT_NETWORK_RECEIVE,
    PERF_EVENT_SYSTEM_CALL,
    PERF_EVENT_INTERRUPT,
    PERF_EVENT_COUNT
} performance_event_type_t;

// Performance event structure
typedef struct {
    performance_event_type_t type;
    unsigned int process_id;
    unsigned int thread_id;
    unsigned long long timestamp;
    unsigned long long value;
    char description[64];
} performance_event_t;

// Performance threshold structure
typedef struct {
    performance_counter_type_t counter_type;
    unsigned long long threshold;
    int (*callback)(performance_counter_t* counter);
} performance_threshold_t;

// Performance monitor functions
void performance_monitor_init();
void performance_monitor_start();
void performance_monitor_stop();
int performance_monitor_is_running();
void performance_monitor_update();
void performance_monitor_reset();
void performance_monitor_print_counters();
void performance_monitor_print_events();
performance_counter_t* performance_monitor_get_counter(performance_counter_type_t type);
int performance_monitor_add_event(performance_event_type_t type, unsigned int process_id, unsigned int thread_id, unsigned long long value, const char* description);
int performance_monitor_set_threshold(performance_counter_type_t counter_type, unsigned long long threshold, int (*callback)(performance_counter_t* counter));
int performance_monitor_clear_threshold(performance_counter_type_t counter_type);
int performance_monitor_save_to_file(const char* filename);
int performance_monitor_load_from_file(const char* filename);

#endif /* PERFORMANCE_MONITOR_H */

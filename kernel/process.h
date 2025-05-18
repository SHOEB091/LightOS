/**
 * LightOS Kernel
 * Process management header
 */

#ifndef PROCESS_H
#define PROCESS_H

// Process ID type
typedef int pid_t;

// Process state enumeration
typedef enum {
    PROCESS_STATE_UNUSED,
    PROCESS_STATE_READY,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_BLOCKED,
    PROCESS_STATE_TERMINATED
} process_state_t;

// Process priority enumeration
typedef enum {
    PROCESS_PRIORITY_LOW,
    PROCESS_PRIORITY_NORMAL,
    PROCESS_PRIORITY_HIGH,
    PROCESS_PRIORITY_KERNEL
} process_priority_t;

// Process context structure (CPU registers)
typedef struct {
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned int eip;
} process_context_t;

// Process structure
typedef struct {
    process_state_t state;
    pid_t pid;
    pid_t parent_pid;
    process_priority_t priority;
    void* stack;
    unsigned int stack_size;
    void* entry_point;
    const char* name;
    process_context_t context;
} process_t;

// Default stack size for processes (64KB)
#define PROCESS_STACK_SIZE 65536

// Process management functions
void process_init();
pid_t process_create(const char* name, void* entry_point, int priority);
void process_terminate(pid_t pid);
process_t* process_current();
void process_schedule();
void process_list();

#endif /* PROCESS_H */

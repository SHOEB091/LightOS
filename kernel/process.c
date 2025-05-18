/**
 * LightOS Kernel
 * Process management implementation
 */

#include "process.h"
#include "memory.h"

// Maximum number of processes
#define MAX_PROCESSES 256

// Process table
static process_t process_table[MAX_PROCESSES];
// Current running process ID
static pid_t current_pid = 0;
// Next available process ID
static pid_t next_pid = 1;

// Initialize process management
void process_init() {
    // Clear the process table
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].state = PROCESS_STATE_UNUSED;
        process_table[i].pid = 0;
    }
    
    // Create the kernel process (PID 0)
    process_table[0].state = PROCESS_STATE_RUNNING;
    process_table[0].pid = 0;
    process_table[0].parent_pid = 0;
    process_table[0].priority = PROCESS_PRIORITY_KERNEL;
    process_table[0].stack = 0; // Kernel has its own stack
    process_table[0].stack_size = 0;
    process_table[0].name = "kernel";
    
    // Set the current process to the kernel process
    current_pid = 0;
}

// Find an unused process slot
static int find_unused_process() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_STATE_UNUSED) {
            return i;
        }
    }
    
    return -1; // No free slots
}

// Create a new process
pid_t process_create(const char* name, void* entry_point, int priority) {
    int slot = find_unused_process();
    
    if (slot == -1) {
        return -1; // No free process slots
    }
    
    // Allocate stack for the new process
    void* stack = allocate_blocks(PROCESS_STACK_SIZE / MEMORY_BLOCK_SIZE);
    
    if (!stack) {
        return -1; // Out of memory
    }
    
    // Initialize the process
    process_table[slot].state = PROCESS_STATE_READY;
    process_table[slot].pid = next_pid++;
    process_table[slot].parent_pid = current_pid;
    process_table[slot].priority = priority;
    process_table[slot].stack = stack;
    process_table[slot].stack_size = PROCESS_STACK_SIZE;
    process_table[slot].entry_point = entry_point;
    process_table[slot].name = name;
    
    // Initialize the process context
    // Stack grows downward, so we start at the top
    unsigned int* stack_top = (unsigned int*)((unsigned int)stack + PROCESS_STACK_SIZE);
    
    // Push entry point (EIP)
    *(--stack_top) = (unsigned int)entry_point;
    
    // Push initial register values
    *(--stack_top) = 0; // EAX
    *(--stack_top) = 0; // ECX
    *(--stack_top) = 0; // EDX
    *(--stack_top) = 0; // EBX
    *(--stack_top) = 0; // ESP (will be set by context switch)
    *(--stack_top) = 0; // EBP
    *(--stack_top) = 0; // ESI
    *(--stack_top) = 0; // EDI
    
    // Save the stack pointer
    process_table[slot].context.esp = (unsigned int)stack_top;
    
    return process_table[slot].pid;
}

// Terminate a process
void process_terminate(pid_t pid) {
    // Find the process
    int slot = -1;
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid && process_table[i].state != PROCESS_STATE_UNUSED) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        return; // Process not found
    }
    
    // Free the process stack
    if (process_table[slot].stack) {
        free_blocks(process_table[slot].stack, process_table[slot].stack_size / MEMORY_BLOCK_SIZE);
    }
    
    // Mark the process as unused
    process_table[slot].state = PROCESS_STATE_UNUSED;
    process_table[slot].pid = 0;
    
    // If we terminated the current process, schedule a new one
    if (pid == current_pid) {
        process_schedule();
    }
}

// Get the current process
process_t* process_current() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == current_pid) {
            return &process_table[i];
        }
    }
    
    return NULL; // Should never happen
}

// Schedule the next process to run
void process_schedule() {
    // Simple round-robin scheduler
    int next_slot = -1;
    int start_slot = 0;
    
    // Find the current process slot
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == current_pid) {
            start_slot = (i + 1) % MAX_PROCESSES;
            break;
        }
    }
    
    // Find the next ready process
    for (int i = 0; i < MAX_PROCESSES; i++) {
        int slot = (start_slot + i) % MAX_PROCESSES;
        
        if (process_table[slot].state == PROCESS_STATE_READY) {
            next_slot = slot;
            break;
        }
    }
    
    // If no ready process found, continue with the current one
    if (next_slot == -1) {
        return;
    }
    
    // Switch to the new process
    pid_t old_pid = current_pid;
    current_pid = process_table[next_slot].pid;
    
    // Mark the new process as running
    process_table[next_slot].state = PROCESS_STATE_RUNNING;
    
    // Find the old process and mark it as ready
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == old_pid) {
            if (process_table[i].state == PROCESS_STATE_RUNNING) {
                process_table[i].state = PROCESS_STATE_READY;
            }
            break;
        }
    }
    
    // Perform the context switch
    process_context_switch(&process_table[next_slot].context);
}

// External assembly function for context switching
extern void process_context_switch(process_context_t* context);

// List all processes
void process_list() {
    // Print header
    terminal_write("PID  PPID  PRI  STATE     NAME\n");
    terminal_write("---- ----- ---- --------- ----------------\n");
    
    // Print each process
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state != PROCESS_STATE_UNUSED) {
            char buffer[80];
            
            // Format the process information
            // This is a simplified version without proper formatting
            terminal_write(buffer);
            terminal_write("\n");
        }
    }
}

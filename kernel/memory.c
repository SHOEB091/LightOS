/**
 * LightOS Kernel
 * Memory management implementation
 */

#include "memory.h"

// Memory map entry structure
typedef struct {
    unsigned int base_low;
    unsigned int base_high;
    unsigned int length_low;
    unsigned int length_high;
    unsigned int type;
} __attribute__((packed)) memory_map_entry;

// Bitmap for physical memory allocation
static unsigned int* physical_memory_bitmap = 0;
static unsigned int total_memory_blocks = 0;
static unsigned int used_memory_blocks = 0;

// Initialize memory management
void memory_init(unsigned int memory_size) {
    // Calculate the number of blocks needed to represent all memory
    total_memory_blocks = memory_size / MEMORY_BLOCK_SIZE;
    
    // Allocate space for the bitmap at a fixed address
    physical_memory_bitmap = (unsigned int*) MEMORY_BITMAP_ADDRESS;
    
    // Clear the bitmap (mark all memory as free)
    for (unsigned int i = 0; i < total_memory_blocks / 32; i++) {
        physical_memory_bitmap[i] = 0;
    }
    
    // Mark the first few blocks as used (for kernel, bitmap, etc.)
    unsigned int reserved_blocks = (MEMORY_RESERVED_END / MEMORY_BLOCK_SIZE);
    for (unsigned int i = 0; i < reserved_blocks; i++) {
        set_block(i);
    }
    
    used_memory_blocks = reserved_blocks;
}

// Set a specific block as used in the bitmap
void set_block(unsigned int block) {
    unsigned int idx = block / 32;
    unsigned int bit = block % 32;
    
    physical_memory_bitmap[idx] |= (1 << bit);
    used_memory_blocks++;
}

// Clear a specific block (mark as free) in the bitmap
void clear_block(unsigned int block) {
    unsigned int idx = block / 32;
    unsigned int bit = block % 32;
    
    physical_memory_bitmap[idx] &= ~(1 << bit);
    used_memory_blocks--;
}

// Test if a specific block is set (used)
int test_block(unsigned int block) {
    unsigned int idx = block / 32;
    unsigned int bit = block % 32;
    
    return (physical_memory_bitmap[idx] & (1 << bit)) != 0;
}

// Find the first free block
int find_first_free_block() {
    for (unsigned int i = 0; i < total_memory_blocks / 32; i++) {
        if (physical_memory_bitmap[i] != 0xFFFFFFFF) {
            // At least one bit is clear
            for (unsigned int j = 0; j < 32; j++) {
                if ((physical_memory_bitmap[i] & (1 << j)) == 0) {
                    return i * 32 + j;
                }
            }
        }
    }
    
    return -1; // No free blocks
}

// Find a sequence of free blocks
int find_free_blocks(unsigned int count) {
    if (count == 0) return -1;
    
    unsigned int free_count = 0;
    unsigned int first_free = 0;
    
    for (unsigned int i = 0; i < total_memory_blocks; i++) {
        if (!test_block(i)) {
            if (free_count == 0) {
                first_free = i;
            }
            
            free_count++;
            
            if (free_count == count) {
                return first_free;
            }
        } else {
            free_count = 0;
        }
    }
    
    return -1; // Not enough contiguous free blocks
}

// Allocate a block of memory
void* allocate_block() {
    int block = find_first_free_block();
    
    if (block == -1) {
        return 0; // Out of memory
    }
    
    set_block(block);
    
    return (void*) (block * MEMORY_BLOCK_SIZE);
}

// Allocate multiple contiguous blocks
void* allocate_blocks(unsigned int count) {
    int starting_block = find_free_blocks(count);
    
    if (starting_block == -1) {
        return 0; // Not enough contiguous memory
    }
    
    for (unsigned int i = 0; i < count; i++) {
        set_block(starting_block + i);
    }
    
    return (void*) (starting_block * MEMORY_BLOCK_SIZE);
}

// Free a block of memory
void free_block(void* address) {
    unsigned int block = ((unsigned int) address) / MEMORY_BLOCK_SIZE;
    
    clear_block(block);
}

// Free multiple blocks
void free_blocks(void* address, unsigned int count) {
    unsigned int block = ((unsigned int) address) / MEMORY_BLOCK_SIZE;
    
    for (unsigned int i = 0; i < count; i++) {
        clear_block(block + i);
    }
}

// Get memory usage statistics
void memory_stats(unsigned int* total, unsigned int* used, unsigned int* free) {
    if (total) *total = total_memory_blocks * MEMORY_BLOCK_SIZE;
    if (used) *used = used_memory_blocks * MEMORY_BLOCK_SIZE;
    if (free) *free = (total_memory_blocks - used_memory_blocks) * MEMORY_BLOCK_SIZE;
}

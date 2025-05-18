/**
 * LightOS Kernel
 * Memory management header
 */

#ifndef MEMORY_H
#define MEMORY_H

// Memory constants
#define MEMORY_BLOCK_SIZE 4096          // 4KB blocks
#define MEMORY_BITMAP_ADDRESS 0x100000  // 1MB (temporary location)
#define MEMORY_RESERVED_END 0x200000    // 2MB (reserved for kernel, etc.)

// Memory management functions
void memory_init(unsigned int memory_size);
void set_block(unsigned int block);
void clear_block(unsigned int block);
int test_block(unsigned int block);
int find_first_free_block();
int find_free_blocks(unsigned int count);
void* allocate_block();
void* allocate_blocks(unsigned int count);
void free_block(void* address);
void free_blocks(void* address, unsigned int count);
void memory_stats(unsigned int* total, unsigned int* used, unsigned int* free);

#endif /* MEMORY_H */

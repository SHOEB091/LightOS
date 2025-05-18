/**
 * LightOS Kernel
 * File system implementation
 */

#include "filesystem.h"
#include "memory.h"
#include "kernel.h"

// Maximum number of open files
#define MAX_OPEN_FILES 64

// File system root node
static fs_node_t* fs_root = NULL;

// Open file table
static file_descriptor_t open_files[MAX_OPEN_FILES];

// Initialize the file system
void fs_init() {
    // Clear the open file table
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        open_files[i].node = NULL;
        open_files[i].offset = 0;
        open_files[i].flags = 0;
    }
    
    // Create a root directory node (in-memory file system for now)
    fs_root = (fs_node_t*) allocate_block();
    
    if (fs_root) {
        fs_root->flags = FS_DIRECTORY;
        fs_root->inode = 0;
        fs_root->length = 0;
        fs_root->name[0] = '/';
        fs_root->name[1] = '\0';
        
        // Initialize directory operations
        fs_root->read = NULL;
        fs_root->write = NULL;
        fs_root->open = NULL;
        fs_root->close = NULL;
        fs_root->readdir = fs_readdir;
        fs_root->finddir = fs_finddir;
        
        // Initialize the root directory contents
        fs_root->impl = NULL; // No implementation-specific data yet
    }
}

// Read data from a file
unsigned int fs_read(file_descriptor_t* fd, unsigned int size, unsigned char* buffer) {
    if (!fd || !fd->node || !buffer) {
        return 0;
    }
    
    // Check if the node has a read function
    if (fd->node->read) {
        unsigned int bytes_read = fd->node->read(fd->node, fd->offset, size, buffer);
        fd->offset += bytes_read;
        return bytes_read;
    }
    
    return 0;
}

// Write data to a file
unsigned int fs_write(file_descriptor_t* fd, unsigned int size, unsigned char* buffer) {
    if (!fd || !fd->node || !buffer) {
        return 0;
    }
    
    // Check if the node has a write function
    if (fd->node->write) {
        unsigned int bytes_written = fd->node->write(fd->node, fd->offset, size, buffer);
        fd->offset += bytes_written;
        return bytes_written;
    }
    
    return 0;
}

// Open a file
file_descriptor_t* fs_open(const char* path, unsigned int flags) {
    // Find the file node
    fs_node_t* node = fs_namei(path);
    
    if (!node) {
        return NULL; // File not found
    }
    
    // Call the node's open function if it exists
    if (node->open) {
        node->open(node);
    }
    
    // Find a free file descriptor
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files[i].node == NULL) {
            open_files[i].node = node;
            open_files[i].offset = 0;
            open_files[i].flags = flags;
            return &open_files[i];
        }
    }
    
    return NULL; // No free file descriptors
}

// Close a file
void fs_close(file_descriptor_t* fd) {
    if (!fd || !fd->node) {
        return;
    }
    
    // Call the node's close function if it exists
    if (fd->node->close) {
        fd->node->close(fd->node);
    }
    
    // Clear the file descriptor
    fd->node = NULL;
    fd->offset = 0;
    fd->flags = 0;
}

// Read directory entries
struct dirent* fs_readdir(fs_node_t* node, unsigned int index) {
    // Check if the node is a directory
    if (!(node->flags & FS_DIRECTORY)) {
        return NULL;
    }
    
    // Check if the node has a readdir function
    if (node->readdir) {
        return node->readdir(node, index);
    }
    
    return NULL;
}

// Find a directory entry
fs_node_t* fs_finddir(fs_node_t* node, const char* name) {
    // Check if the node is a directory
    if (!(node->flags & FS_DIRECTORY)) {
        return NULL;
    }
    
    // Check if the node has a finddir function
    if (node->finddir) {
        return node->finddir(node, name);
    }
    
    return NULL;
}

// Resolve a path to a file node
fs_node_t* fs_namei(const char* path) {
    if (!path || !fs_root) {
        return NULL;
    }
    
    // Start at the root directory
    fs_node_t* current = fs_root;
    
    // Handle absolute paths
    if (path[0] == '/') {
        path++;
    }
    
    // Parse the path components
    char component[256];
    int i = 0;
    
    while (*path) {
        // Extract the next path component
        i = 0;
        while (*path && *path != '/') {
            component[i++] = *path++;
        }
        component[i] = '\0';
        
        // Skip empty components
        if (i == 0) {
            if (*path) path++;
            continue;
        }
        
        // Find the component in the current directory
        current = fs_finddir(current, component);
        
        if (!current) {
            return NULL; // Component not found
        }
        
        // Skip the separator
        if (*path) path++;
    }
    
    return current;
}

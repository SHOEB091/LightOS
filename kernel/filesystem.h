/**
 * LightOS Kernel
 * File system header
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

// File system node flags
#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x04
#define FS_BLOCKDEVICE 0x08
#define FS_PIPE        0x10
#define FS_SYMLINK     0x20
#define FS_MOUNTPOINT  0x40

// File open flags
#define O_RDONLY    0x01
#define O_WRONLY    0x02
#define O_RDWR      0x03
#define O_APPEND    0x04
#define O_CREAT     0x08
#define O_TRUNC     0x10
#define O_EXCL      0x20

// Directory entry structure
struct dirent {
    char name[256];
    unsigned int inode;
};

// Forward declaration for fs_node_t
typedef struct fs_node fs_node_t;

// Function types for file operations
typedef unsigned int (*read_type_t)(struct fs_node*, unsigned int, unsigned int, unsigned char*);
typedef unsigned int (*write_type_t)(struct fs_node*, unsigned int, unsigned int, unsigned char*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent* (*readdir_type_t)(struct fs_node*, unsigned int);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, const char*);

// File system node structure
struct fs_node {
    char name[256];
    unsigned int flags;
    unsigned int inode;
    unsigned int length;
    
    // File operations
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    
    // Implementation-specific data
    void* impl;
};

// File descriptor structure
typedef struct {
    fs_node_t* node;
    unsigned int offset;
    unsigned int flags;
} file_descriptor_t;

// File system functions
void fs_init();
unsigned int fs_read(file_descriptor_t* fd, unsigned int size, unsigned char* buffer);
unsigned int fs_write(file_descriptor_t* fd, unsigned int size, unsigned char* buffer);
file_descriptor_t* fs_open(const char* path, unsigned int flags);
void fs_close(file_descriptor_t* fd);
struct dirent* fs_readdir(fs_node_t* node, unsigned int index);
fs_node_t* fs_finddir(fs_node_t* node, const char* name);
fs_node_t* fs_namei(const char* path);

#endif /* FILESYSTEM_H */

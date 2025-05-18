/**
 * LightOS Kernel
 * Extended file system support header
 */

#ifndef FILESYSTEM_EXT_H
#define FILESYSTEM_EXT_H

#include "filesystem.h"

// File system types
typedef enum {
    FS_TYPE_UNKNOWN,
    FS_TYPE_EXT2,
    FS_TYPE_EXT3,
    FS_TYPE_EXT4,
    FS_TYPE_FAT16,
    FS_TYPE_FAT32,
    FS_TYPE_NTFS,
    FS_TYPE_ISO9660,
    FS_TYPE_BTRFS,
    FS_TYPE_XFS,
    FS_TYPE_ZFS,
    FS_TYPE_TMPFS
} fs_type_t;

// File system structure
typedef struct {
    char name[32];
    fs_type_t type;
    char device[32];
    char mount_point[256];
    unsigned int flags;
    unsigned long long total_size;
    unsigned long long free_size;
    
    // Function pointers for file system operations
    int (*mount)(struct filesystem* fs, const char* device, const char* mount_point, unsigned int flags);
    int (*unmount)(struct filesystem* fs);
    int (*read)(struct filesystem* fs, const char* path, void* buffer, unsigned int size, unsigned int offset);
    int (*write)(struct filesystem* fs, const char* path, const void* buffer, unsigned int size, unsigned int offset);
    int (*open)(struct filesystem* fs, const char* path, unsigned int flags);
    int (*close)(struct filesystem* fs, int fd);
    int (*mkdir)(struct filesystem* fs, const char* path, unsigned int mode);
    int (*rmdir)(struct filesystem* fs, const char* path);
    int (*unlink)(struct filesystem* fs, const char* path);
    int (*rename)(struct filesystem* fs, const char* old_path, const char* new_path);
    int (*stat)(struct filesystem* fs, const char* path, fs_stat_t* stat);
    int (*chmod)(struct filesystem* fs, const char* path, unsigned int mode);
    int (*chown)(struct filesystem* fs, const char* path, unsigned int uid, unsigned int gid);
    int (*truncate)(struct filesystem* fs, const char* path, unsigned int size);
    int (*sync)(struct filesystem* fs);
    
    // Private data for the file system
    void* private_data;
} filesystem_t;

// File system manager functions
void fs_manager_init();
int fs_register_filesystem(filesystem_t* fs);
int fs_unregister_filesystem(const char* name);
filesystem_t* fs_get_filesystem(const char* name);
int fs_mount(const char* fs_name, const char* device, const char* mount_point, unsigned int flags);
int fs_unmount(const char* mount_point);
void fs_list_filesystems();
void fs_list_mounts();

// Specific file system initialization
int ext2_init();
int ext3_init();
int ext4_init();
int fat16_init();
int fat32_init();
int ntfs_init();
int iso9660_init();
int btrfs_init();
int xfs_init();
int zfs_init();
int tmpfs_init();

#endif /* FILESYSTEM_EXT_H */

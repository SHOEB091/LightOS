/**
 * LightOS Kernel
 * Extended file system support implementation
 */

#include "filesystem_ext.h"
#include "kernel.h"
#include "memory.h"
#include "../libc/string.h"

// Maximum number of file systems
#define MAX_FILESYSTEMS 16

// Maximum number of mount points
#define MAX_MOUNTS 32

// File system array
static filesystem_t* filesystems[MAX_FILESYSTEMS];
static int filesystem_count = 0;

// Mount point array
typedef struct {
    char device[32];
    char mount_point[256];
    filesystem_t* fs;
    unsigned int flags;
} mount_t;

static mount_t mounts[MAX_MOUNTS];
static int mount_count = 0;

// Initialize the file system manager
void fs_manager_init() {
    terminal_write("Initializing file system manager...\n");
    
    // Clear the file system array
    for (int i = 0; i < MAX_FILESYSTEMS; i++) {
        filesystems[i] = NULL;
    }
    
    filesystem_count = 0;
    
    // Clear the mount array
    for (int i = 0; i < MAX_MOUNTS; i++) {
        mounts[i].device[0] = '\0';
        mounts[i].mount_point[0] = '\0';
        mounts[i].fs = NULL;
        mounts[i].flags = 0;
    }
    
    mount_count = 0;
    
    // Register built-in file systems
    ext2_init();
    ext3_init();
    ext4_init();
    fat16_init();
    fat32_init();
    ntfs_init();
    iso9660_init();
    btrfs_init();
    xfs_init();
    zfs_init();
    tmpfs_init();
    
    terminal_write("File system manager initialized\n");
    terminal_write_color("Registered ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert filesystem_count to string
    char count_str[16];
    int temp = filesystem_count;
    int i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write(count_str);
    terminal_write(" file systems\n");
}

// Register a file system
int fs_register_filesystem(filesystem_t* fs) {
    if (!fs) {
        return -1;
    }
    
    // Check if we have room for another file system
    if (filesystem_count >= MAX_FILESYSTEMS) {
        terminal_write("Error: Maximum number of file systems reached\n");
        return -1;
    }
    
    // Check if a file system with the same name already exists
    for (int i = 0; i < filesystem_count; i++) {
        if (strcmp(filesystems[i]->name, fs->name) == 0) {
            terminal_write("Error: File system with name '");
            terminal_write(fs->name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Allocate memory for the file system
    filesystem_t* new_fs = (filesystem_t*)allocate_block();
    
    if (!new_fs) {
        terminal_write("Error: Failed to allocate memory for file system\n");
        return -1;
    }
    
    // Copy the file system data
    memcpy(new_fs, fs, sizeof(filesystem_t));
    
    // Add the file system to the array
    filesystems[filesystem_count++] = new_fs;
    
    terminal_write("Registered file system: ");
    terminal_write(fs->name);
    terminal_write("\n");
    
    return 0;
}

// Unregister a file system
int fs_unregister_filesystem(const char* name) {
    if (!name) {
        return -1;
    }
    
    // Find the file system
    for (int i = 0; i < filesystem_count; i++) {
        if (strcmp(filesystems[i]->name, name) == 0) {
            // Check if the file system is mounted
            for (int j = 0; j < mount_count; j++) {
                if (mounts[j].fs == filesystems[i]) {
                    terminal_write("Error: Cannot unregister file system '");
                    terminal_write(name);
                    terminal_write("' because it is mounted\n");
                    return -1;
                }
            }
            
            // Free the file system memory
            free_block(filesystems[i]);
            
            // Remove the file system from the array by shifting all subsequent file systems
            for (int j = i; j < filesystem_count - 1; j++) {
                filesystems[j] = filesystems[j + 1];
            }
            
            filesystems[--filesystem_count] = NULL;
            
            terminal_write("Unregistered file system: ");
            terminal_write(name);
            terminal_write("\n");
            
            return 0;
        }
    }
    
    terminal_write("Error: File system '");
    terminal_write(name);
    terminal_write("' not found\n");
    
    return -1;
}

// Get a file system by name
filesystem_t* fs_get_filesystem(const char* name) {
    if (!name) {
        return NULL;
    }
    
    // Find the file system
    for (int i = 0; i < filesystem_count; i++) {
        if (strcmp(filesystems[i]->name, name) == 0) {
            return filesystems[i];
        }
    }
    
    return NULL;
}

// Mount a file system
int fs_mount(const char* fs_name, const char* device, const char* mount_point, unsigned int flags) {
    if (!fs_name || !device || !mount_point) {
        return -1;
    }
    
    // Check if we have room for another mount
    if (mount_count >= MAX_MOUNTS) {
        terminal_write("Error: Maximum number of mounts reached\n");
        return -1;
    }
    
    // Check if the mount point is already in use
    for (int i = 0; i < mount_count; i++) {
        if (strcmp(mounts[i].mount_point, mount_point) == 0) {
            terminal_write("Error: Mount point '");
            terminal_write(mount_point);
            terminal_write("' is already in use\n");
            return -1;
        }
    }
    
    // Find the file system
    filesystem_t* fs = fs_get_filesystem(fs_name);
    
    if (!fs) {
        terminal_write("Error: File system '");
        terminal_write(fs_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Mount the file system
    if (!fs->mount) {
        terminal_write("Error: File system '");
        terminal_write(fs_name);
        terminal_write("' does not support mounting\n");
        return -1;
    }
    
    int result = fs->mount(fs, device, mount_point, flags);
    
    if (result != 0) {
        terminal_write("Error: Failed to mount '");
        terminal_write(device);
        terminal_write("' on '");
        terminal_write(mount_point);
        terminal_write("'\n");
        return -1;
    }
    
    // Add the mount to the array
    strcpy(mounts[mount_count].device, device);
    strcpy(mounts[mount_count].mount_point, mount_point);
    mounts[mount_count].fs = fs;
    mounts[mount_count].flags = flags;
    mount_count++;
    
    terminal_write("Mounted '");
    terminal_write(device);
    terminal_write("' on '");
    terminal_write(mount_point);
    terminal_write("' with file system '");
    terminal_write(fs_name);
    terminal_write("'\n");
    
    return 0;
}

// Unmount a file system
int fs_unmount(const char* mount_point) {
    if (!mount_point) {
        return -1;
    }
    
    // Find the mount
    for (int i = 0; i < mount_count; i++) {
        if (strcmp(mounts[i].mount_point, mount_point) == 0) {
            // Unmount the file system
            if (!mounts[i].fs->unmount) {
                terminal_write("Error: File system '");
                terminal_write(mounts[i].fs->name);
                terminal_write("' does not support unmounting\n");
                return -1;
            }
            
            int result = mounts[i].fs->unmount(mounts[i].fs);
            
            if (result != 0) {
                terminal_write("Error: Failed to unmount '");
                terminal_write(mount_point);
                terminal_write("'\n");
                return -1;
            }
            
            // Remove the mount from the array by shifting all subsequent mounts
            for (int j = i; j < mount_count - 1; j++) {
                strcpy(mounts[j].device, mounts[j + 1].device);
                strcpy(mounts[j].mount_point, mounts[j + 1].mount_point);
                mounts[j].fs = mounts[j + 1].fs;
                mounts[j].flags = mounts[j + 1].flags;
            }
            
            mounts[--mount_count].device[0] = '\0';
            mounts[mount_count].mount_point[0] = '\0';
            mounts[mount_count].fs = NULL;
            mounts[mount_count].flags = 0;
            
            terminal_write("Unmounted '");
            terminal_write(mount_point);
            terminal_write("'\n");
            
            return 0;
        }
    }
    
    terminal_write("Error: Mount point '");
    terminal_write(mount_point);
    terminal_write("' not found\n");
    
    return -1;
}

// List all registered file systems
void fs_list_filesystems() {
    terminal_write("Registered File Systems:\n");
    terminal_write("-----------------------\n");
    
    if (filesystem_count == 0) {
        terminal_write("No file systems registered\n");
        return;
    }
    
    for (int i = 0; i < filesystem_count; i++) {
        terminal_write(filesystems[i]->name);
        terminal_write(" (");
        
        // Print file system type
        switch (filesystems[i]->type) {
            case FS_TYPE_EXT2:
                terminal_write("ext2");
                break;
            case FS_TYPE_EXT3:
                terminal_write("ext3");
                break;
            case FS_TYPE_EXT4:
                terminal_write("ext4");
                break;
            case FS_TYPE_FAT16:
                terminal_write("fat16");
                break;
            case FS_TYPE_FAT32:
                terminal_write("fat32");
                break;
            case FS_TYPE_NTFS:
                terminal_write("ntfs");
                break;
            case FS_TYPE_ISO9660:
                terminal_write("iso9660");
                break;
            case FS_TYPE_BTRFS:
                terminal_write("btrfs");
                break;
            case FS_TYPE_XFS:
                terminal_write("xfs");
                break;
            case FS_TYPE_ZFS:
                terminal_write("zfs");
                break;
            case FS_TYPE_TMPFS:
                terminal_write("tmpfs");
                break;
            default:
                terminal_write("unknown");
                break;
        }
        
        terminal_write(")\n");
    }
}

// List all mounted file systems
void fs_list_mounts() {
    terminal_write("Mounted File Systems:\n");
    terminal_write("--------------------\n");
    
    if (mount_count == 0) {
        terminal_write("No file systems mounted\n");
        return;
    }
    
    for (int i = 0; i < mount_count; i++) {
        terminal_write(mounts[i].device);
        terminal_write(" on ");
        terminal_write(mounts[i].mount_point);
        terminal_write(" type ");
        terminal_write(mounts[i].fs->name);
        terminal_write("\n");
    }
}

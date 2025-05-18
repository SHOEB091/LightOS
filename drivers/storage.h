/**
 * LightOS Drivers
 * Storage driver header
 */

#ifndef STORAGE_H
#define STORAGE_H

// Storage device types
typedef enum {
    STORAGE_TYPE_UNKNOWN,
    STORAGE_TYPE_HDD,
    STORAGE_TYPE_SSD,
    STORAGE_TYPE_NVME,
    STORAGE_TYPE_USB,
    STORAGE_TYPE_SD,
    STORAGE_TYPE_CDROM,
    STORAGE_TYPE_FLOPPY
} storage_type_t;

// Storage device structure
typedef struct {
    char name[32];
    storage_type_t type;
    unsigned long long size;          // Size in bytes
    unsigned int sector_size;         // Sector size in bytes
    unsigned int read_only;           // 1 if read-only, 0 if read-write
    unsigned int removable;           // 1 if removable, 0 if fixed
    
    // Function pointers for device operations
    int (*read_sectors)(struct storage_device* dev, unsigned int start_sector, unsigned int sector_count, void* buffer);
    int (*write_sectors)(struct storage_device* dev, unsigned int start_sector, unsigned int sector_count, const void* buffer);
    int (*flush)(struct storage_device* dev);
    
    // Private data for the driver
    void* private_data;
} storage_device_t;

// Storage driver functions
void storage_init();
int storage_register_device(storage_device_t* device);
int storage_unregister_device(const char* name);
storage_device_t* storage_get_device(const char* name);
int storage_read_sectors(const char* device_name, unsigned int start_sector, unsigned int sector_count, void* buffer);
int storage_write_sectors(const char* device_name, unsigned int start_sector, unsigned int sector_count, const void* buffer);
int storage_flush(const char* device_name);
void storage_list_devices();

// Specific storage device detection
int detect_ata_devices();
int detect_nvme_devices();
int detect_usb_storage_devices();
int detect_sd_devices();

#endif /* STORAGE_H */

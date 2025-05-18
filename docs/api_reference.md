# LightOS API Reference

## Introduction

This document provides a comprehensive reference for the Application Programming Interfaces (APIs) available in the LightOS operating system. It covers kernel APIs, system libraries, and subsystem interfaces.

## Kernel APIs

### Display Management

#### Display Initialization

```c
int display_init();
```
Initializes the display subsystem.

**Returns:** 0 on success, -1 on failure.

```c
int display_set_mode(unsigned int width, unsigned int height, unsigned int bpp);
```
Sets the display mode.

**Parameters:**
- `width`: Display width in pixels.
- `height`: Display height in pixels.
- `bpp`: Bits per pixel (color depth).

**Returns:** 0 on success, -1 on failure.

```c
int display_get_mode(unsigned int* width, unsigned int* height, unsigned int* bpp);
```
Gets the current display mode.

**Parameters:**
- `width`: Pointer to store the display width.
- `height`: Pointer to store the display height.
- `bpp`: Pointer to store the bits per pixel.

**Returns:** 0 on success, -1 on failure.

#### HDMI Operations

```c
int hdmi_init();
```
Initializes the HDMI subsystem. Supports standard HDMI resolutions including 1920x1080p (Full HD).

**Returns:** 0 on success, -1 on failure.

```c
int hdmi_detect();
```
Detects connected HDMI displays.

**Returns:** Number of connected displays, or -1 on failure.

```c
int hdmi_is_resolution_supported(unsigned int display, unsigned int width, unsigned int height);
```
Checks if a specific resolution is supported by the HDMI display.

**Parameters:**
- `display`: Display index.
- `width`: Display width in pixels.
- `height`: Display height in pixels.

**Returns:** 1 if supported, 0 if not supported, -1 on failure.

```c
int hdmi_set_1080p(unsigned int display);
```
Convenience function to set the display to 1920x1080p (Full HD) resolution at 60Hz.

**Parameters:**
- `display`: Display index.

**Returns:** 0 on success, -1 on failure.

```c
int hdmi_get_edid(unsigned int display, unsigned char* edid, unsigned int size);
```
Gets the EDID data from an HDMI display.

**Parameters:**
- `display`: Display index.
- `edid`: Buffer to store the EDID data.
- `size`: Size of the buffer.

**Returns:** Size of EDID data, or -1 on failure.

```c
int hdmi_set_mode(unsigned int display, unsigned int width, unsigned int height, unsigned int refresh_rate);
```
Sets the mode for an HDMI display.

**Parameters:**
- `display`: Display index.
- `width`: Display width in pixels.
- `height`: Display height in pixels.
- `refresh_rate`: Refresh rate in Hz.

**Returns:** 0 on success, -1 on failure.

```c
int hdmi_enable_audio(unsigned int display, unsigned int channels, unsigned int sample_rate, unsigned int bits_per_sample);
```
Enables audio over HDMI.

**Parameters:**
- `display`: Display index.
- `channels`: Number of audio channels.
- `sample_rate`: Audio sample rate in Hz.
- `bits_per_sample`: Bits per audio sample.

**Returns:** 0 on success, -1 on failure.

```c
int hdmi_send_cec_command(unsigned int display, const unsigned char* command, unsigned int size);
```
Sends a CEC command to an HDMI display.

**Parameters:**
- `display`: Display index.
- `command`: CEC command buffer.
- `size`: Size of the command.

**Returns:** 0 on success, -1 on failure.

### Memory Management

#### Memory Allocation

```c
void* allocate_block();
```
Allocates a single memory block of size `MEMORY_BLOCK_SIZE`.

**Returns:** Pointer to the allocated memory block, or NULL if allocation fails.

```c
void* allocate_blocks(unsigned int count);
```
Allocates multiple memory blocks.

**Parameters:**
- `count`: Number of blocks to allocate.

**Returns:** Pointer to the allocated memory blocks, or NULL if allocation fails.

```c
void free_block(void* ptr);
```
Frees a single memory block.

**Parameters:**
- `ptr`: Pointer to the memory block to free.

```c
void free_blocks(void* ptr, unsigned int count);
```
Frees multiple memory blocks.

**Parameters:**
- `ptr`: Pointer to the memory blocks to free.
- `count`: Number of blocks to free.

#### Memory Mapping

```c
void* memory_map(unsigned int physical_address, unsigned int size);
```
Maps a physical memory region into virtual memory.

**Parameters:**
- `physical_address`: Physical address to map.
- `size`: Size of the region to map.

**Returns:** Virtual address of the mapped region, or NULL if mapping fails.

```c
void memory_unmap(void* virtual_address, unsigned int size);
```
Unmaps a virtual memory region.

**Parameters:**
- `virtual_address`: Virtual address to unmap.
- `size`: Size of the region to unmap.

### Process Management

#### Process Creation

```c
int process_create(const char* name, void (*entry_point)(void), unsigned int priority);
```
Creates a new process.

**Parameters:**
- `name`: Name of the process.
- `entry_point`: Entry point function for the process.
- `priority`: Process priority (0-255, lower is higher priority).

**Returns:** Process ID, or -1 if creation fails.

```c
int process_fork();
```
Forks the current process.

**Returns:** Child process ID in the parent, 0 in the child, or -1 if fork fails.

```c
int process_exec(const char* path, char* const argv[]);
```
Replaces the current process with a new program.

**Parameters:**
- `path`: Path to the executable.
- `argv`: Array of arguments.

**Returns:** -1 if exec fails (does not return on success).

#### Process Control

```c
int process_terminate(int pid);
```
Terminates a process.

**Parameters:**
- `pid`: Process ID to terminate.

**Returns:** 0 on success, -1 on failure.

```c
int process_wait(int pid, int* status);
```
Waits for a process to terminate.

**Parameters:**
- `pid`: Process ID to wait for.
- `status`: Pointer to store the exit status.

**Returns:** 0 on success, -1 on failure.

```c
int process_set_priority(int pid, unsigned int priority);
```
Sets the priority of a process.

**Parameters:**
- `pid`: Process ID.
- `priority`: New priority (0-255, lower is higher priority).

**Returns:** 0 on success, -1 on failure.

#### Process Information

```c
int process_get_id();
```
Gets the ID of the current process.

**Returns:** Process ID.

```c
int process_get_parent_id();
```
Gets the ID of the parent process.

**Returns:** Parent process ID.

```c
int process_get_state(int pid, process_state_t* state);
```
Gets the state of a process.

**Parameters:**
- `pid`: Process ID.
- `state`: Pointer to store the process state.

**Returns:** 0 on success, -1 on failure.

### Filesystem

#### File Operations

```c
int filesystem_open(const char* path, int flags);
```
Opens a file.

**Parameters:**
- `path`: Path to the file.
- `flags`: Open flags (O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, O_APPEND).

**Returns:** File descriptor, or -1 on failure.

```c
int filesystem_close(int fd);
```
Closes a file.

**Parameters:**
- `fd`: File descriptor.

**Returns:** 0 on success, -1 on failure.

```c
int filesystem_read(int fd, void* buffer, unsigned int size);
```
Reads from a file.

**Parameters:**
- `fd`: File descriptor.
- `buffer`: Buffer to read into.
- `size`: Number of bytes to read.

**Returns:** Number of bytes read, or -1 on failure.

```c
int filesystem_write(int fd, const void* buffer, unsigned int size);
```
Writes to a file.

**Parameters:**
- `fd`: File descriptor.
- `buffer`: Buffer to write from.
- `size`: Number of bytes to write.

**Returns:** Number of bytes written, or -1 on failure.

```c
int filesystem_seek(int fd, int offset, int whence);
```
Seeks to a position in a file.

**Parameters:**
- `fd`: File descriptor.
- `offset`: Offset in bytes.
- `whence`: SEEK_SET, SEEK_CUR, or SEEK_END.

**Returns:** New position, or -1 on failure.

#### Directory Operations

```c
int filesystem_create_directory(const char* path);
```
Creates a directory.

**Parameters:**
- `path`: Path to the directory.

**Returns:** 0 on success, -1 on failure.

```c
int filesystem_remove_directory(const char* path);
```
Removes a directory.

**Parameters:**
- `path`: Path to the directory.

**Returns:** 0 on success, -1 on failure.

```c
int filesystem_list_directory(const char* path, char*** entries, unsigned int* count);
```
Lists the contents of a directory.

**Parameters:**
- `path`: Path to the directory.
- `entries`: Pointer to store the array of entry names.
- `count`: Pointer to store the number of entries.

**Returns:** 0 on success, -1 on failure.

#### File Information

```c
int filesystem_get_file_info(const char* path, file_info_t* info);
```
Gets information about a file.

**Parameters:**
- `path`: Path to the file.
- `info`: Pointer to store the file information.

**Returns:** 0 on success, -1 on failure.

```c
int filesystem_file_exists(const char* path);
```
Checks if a file exists.

**Parameters:**
- `path`: Path to the file.

**Returns:** 1 if the file exists, 0 if it does not exist, -1 on error.

### Networking

#### Socket Operations

```c
int network_socket(int domain, int type, int protocol);
```
Creates a socket.

**Parameters:**
- `domain`: Socket domain (AF_INET, AF_INET6, AF_UNIX).
- `type`: Socket type (SOCK_STREAM, SOCK_DGRAM, SOCK_RAW).
- `protocol`: Protocol (IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP).

**Returns:** Socket descriptor, or -1 on failure.

```c
int network_bind(int sockfd, const struct sockaddr* addr, unsigned int addrlen);
```
Binds a socket to an address.

**Parameters:**
- `sockfd`: Socket descriptor.
- `addr`: Address to bind to.
- `addrlen`: Length of the address.

**Returns:** 0 on success, -1 on failure.

```c
int network_listen(int sockfd, int backlog);
```
Listens for connections on a socket.

**Parameters:**
- `sockfd`: Socket descriptor.
- `backlog`: Maximum length of the queue of pending connections.

**Returns:** 0 on success, -1 on failure.

```c
int network_accept(int sockfd, struct sockaddr* addr, unsigned int* addrlen);
```
Accepts a connection on a socket.

**Parameters:**
- `sockfd`: Socket descriptor.
- `addr`: Pointer to store the address of the connecting entity.
- `addrlen`: Pointer to the length of the address.

**Returns:** New socket descriptor, or -1 on failure.

```c
int network_connect(int sockfd, const struct sockaddr* addr, unsigned int addrlen);
```
Connects a socket to an address.

**Parameters:**
- `sockfd`: Socket descriptor.
- `addr`: Address to connect to.
- `addrlen`: Length of the address.

**Returns:** 0 on success, -1 on failure.

```c
int network_send(int sockfd, const void* buffer, unsigned int length, int flags);
```
Sends data on a socket.

**Parameters:**
- `sockfd`: Socket descriptor.
- `buffer`: Buffer containing the data to send.
- `length`: Length of the data.
- `flags`: Flags.

**Returns:** Number of bytes sent, or -1 on failure.

```c
int network_recv(int sockfd, void* buffer, unsigned int length, int flags);
```
Receives data from a socket.

**Parameters:**
- `sockfd`: Socket descriptor.
- `buffer`: Buffer to store the received data.
- `length`: Length of the buffer.
- `flags`: Flags.

**Returns:** Number of bytes received, or -1 on failure.

```c
int network_close(int sockfd);
```
Closes a socket.

**Parameters:**
- `sockfd`: Socket descriptor.

**Returns:** 0 on success, -1 on failure.

#### Network Configuration

```c
int network_get_interface_info(const char* interface, network_interface_info_t* info);
```
Gets information about a network interface.

**Parameters:**
- `interface`: Interface name.
- `info`: Pointer to store the interface information.

**Returns:** 0 on success, -1 on failure.

```c
int network_set_interface_address(const char* interface, const char* address, const char* netmask);
```
Sets the address of a network interface.

**Parameters:**
- `interface`: Interface name.
- `address`: IP address.
- `netmask`: Network mask.

**Returns:** 0 on success, -1 on failure.

```c
int network_add_route(const char* destination, const char* gateway, const char* netmask, const char* interface);
```
Adds a route to the routing table.

**Parameters:**
- `destination`: Destination network.
- `gateway`: Gateway address.
- `netmask`: Network mask.
- `interface`: Interface name.

**Returns:** 0 on success, -1 on failure.

## System Libraries

### libc

The standard C library provides POSIX-compatible functions:
- String manipulation
- Memory operations
- Input/output
- File operations
- Process control
- Time and date
- Mathematics
- Error handling

### libnet

The networking library provides high-level networking functions:
- HTTP client and server
- FTP client
- SMTP client
- DNS resolution
- WebSocket client and server
- SSL/TLS support

### libgui

The GUI library provides functions for creating graphical user interfaces:
- Window management
- Widget toolkit
- Event handling
- Drawing and rendering
- Font handling
- Image loading and manipulation

### libsecurity

The security library provides cryptographic and security functions:
- Encryption and decryption
- Hashing
- Digital signatures
- Certificate management
- Random number generation
- Password handling

## Subsystem APIs

### Security Manager

```c
int security_add_user(const char* username, const char* password, unsigned int uid, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin);
```
Adds a user to the system.

**Parameters:**
- `username`: Username.
- `password`: Password.
- `uid`: User ID.
- `gid`: Group ID.
- `home_directory`: Home directory path.
- `shell`: Shell path.
- `admin`: Admin flag (1 for admin, 0 for regular user).

**Returns:** 0 on success, -1 on failure.

```c
int security_remove_user(const char* username);
```
Removes a user from the system.

**Parameters:**
- `username`: Username.

**Returns:** 0 on success, -1 on failure.

### Package Manager

```c
int package_install(const char* name, const char* version);
```
Installs a package.

**Parameters:**
- `name`: Package name.
- `version`: Package version (can be NULL for latest version).

**Returns:** 0 on success, -1 on failure.

```c
int package_remove(const char* name);
```
Removes a package.

**Parameters:**
- `name`: Package name.

**Returns:** 0 on success, -1 on failure.

### Update Manager

```c
int update_check();
```
Checks for updates.

**Returns:** 0 on success, -1 on failure.

```c
int update_install(const char* id);
```
Installs an update.

**Parameters:**
- `id`: Update ID.

**Returns:** 0 on success, -1 on failure.

### Backup Manager

```c
int backup_create(const char* name, const char* description, backup_type_t type, const char* source_path, const char* destination_path);
```
Creates a backup.

**Parameters:**
- `name`: Backup name.
- `description`: Backup description.
- `type`: Backup type.
- `source_path`: Source path.
- `destination_path`: Destination path.

**Returns:** 0 on success, -1 on failure.

```c
int backup_restore(const char* id, const char* destination_path);
```
Restores a backup.

**Parameters:**
- `id`: Backup ID.
- `destination_path`: Destination path.

**Returns:** 0 on success, -1 on failure.

### Monitor Manager

```c
int monitor_add_resource(const char* name, resource_type_t type, const char* description, unsigned int interval);
```
Adds a resource to monitor.

**Parameters:**
- `name`: Resource name.
- `type`: Resource type.
- `description`: Resource description.
- `interval`: Monitoring interval in seconds.

**Returns:** 0 on success, -1 on failure.

```c
int monitor_get_value(const char* id, unsigned int* value);
```
Gets the current value of a resource.

**Parameters:**
- `id`: Resource ID.
- `value`: Pointer to store the value.

**Returns:** 0 on success, -1 on failure.

### Firewall

```c
int firewall_add_rule(const char* chain_id, const char* name, const char* description, firewall_action_t action, firewall_direction_t direction, firewall_protocol_t protocol, const char* source_address, const char* source_mask, unsigned int source_port_start, unsigned int source_port_end, const char* destination_address, const char* destination_mask, unsigned int destination_port_start, unsigned int destination_port_end, unsigned int priority);
```
Adds a firewall rule.

**Parameters:**
- `chain_id`: Chain ID.
- `name`: Rule name.
- `description`: Rule description.
- `action`: Rule action.
- `direction`: Rule direction.
- `protocol`: Rule protocol.
- `source_address`: Source address.
- `source_mask`: Source mask.
- `source_port_start`: Source port start.
- `source_port_end`: Source port end.
- `destination_address`: Destination address.
- `destination_mask`: Destination mask.
- `destination_port_start`: Destination port start.
- `destination_port_end`: Destination port end.
- `priority`: Rule priority.

**Returns:** 0 on success, -1 on failure.

```c
int firewall_remove_rule(const char* chain_id, const char* rule_id);
```
Removes a firewall rule.

**Parameters:**
- `chain_id`: Chain ID.
- `rule_id`: Rule ID.

**Returns:** 0 on success, -1 on failure.

### Cryptography

```c
int crypto_generate_key(key_type_t type, encryption_algorithm_t algorithm, unsigned int size, key_t* key);
```
Generates a cryptographic key.

**Parameters:**
- `type`: Key type.
- `algorithm`: Encryption algorithm.
- `size`: Key size in bits.
- `key`: Pointer to store the key.

**Returns:** 0 on success, -1 on failure.

```c
int crypto_encrypt(const char* data, unsigned int data_size, const key_t* key, encryption_mode_t mode, const char* iv, char* encrypted_data, unsigned int* encrypted_data_size);
```
Encrypts data.

**Parameters:**
- `data`: Data to encrypt.
- `data_size`: Size of the data.
- `key`: Encryption key.
- `mode`: Encryption mode.
- `iv`: Initialization vector.
- `encrypted_data`: Buffer to store the encrypted data.
- `encrypted_data_size`: Pointer to the size of the encrypted data buffer (input: buffer size, output: actual size).

**Returns:** 0 on success, -1 on failure.

## Error Handling

Most API functions return an integer status code:
- 0 indicates success
- -1 indicates failure

After a failure, you can get the error code and message:

```c
int error_get_code();
```
Gets the error code of the last operation.

**Returns:** Error code.

```c
const char* error_get_message();
```
Gets the error message of the last operation.

**Returns:** Error message.

## Version Information

```c
const char* system_get_version();
```
Gets the system version.

**Returns:** System version string.

```c
const char* system_get_kernel_version();
```
Gets the kernel version.

**Returns:** Kernel version string.

## Further Resources

For more detailed information, refer to:
- [LightOS Developer Guide](developer_guide.md)
- [LightOS User Manual](lightos_manual.md)
- [LightOS Website](https://lightos.org)
- [GitHub Repository](https://github.com/lightos/lightos)

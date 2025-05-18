/**
 * LightOS Virtualization
 * Virtual Machine Manager implementation
 */

#include "vm_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../kernel/process.h"
#include "../libc/string.h"

// Maximum number of VMs
#define MAX_VMS 64

// Maximum number of disks per VM
#define MAX_DISKS_PER_VM 16

// Maximum number of network interfaces per VM
#define MAX_NETWORK_INTERFACES_PER_VM 8

// VM array
static vm_t* vms[MAX_VMS];
static unsigned int vm_count = 0;

// Initialize the VM manager
void vm_manager_init() {
    terminal_write("Initializing VM manager...\n");
    
    // Clear the VM array
    for (int i = 0; i < MAX_VMS; i++) {
        vms[i] = NULL;
    }
    
    vm_count = 0;
    
    terminal_write("VM manager initialized\n");
}

// Create a VM
int vm_create(const char* name, vm_type_t type, unsigned int vcpus, unsigned long long memory) {
    if (!name) {
        terminal_write("Error: VM name cannot be NULL\n");
        return -1;
    }
    
    // Check if a VM with the same name already exists
    for (unsigned int i = 0; i < vm_count; i++) {
        if (strcmp(vms[i]->name, name) == 0) {
            terminal_write("Error: VM with name '");
            terminal_write(name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Check if we have room for another VM
    if (vm_count >= MAX_VMS) {
        terminal_write("Error: Maximum number of VMs reached\n");
        return -1;
    }
    
    // Allocate memory for the VM
    vm_t* vm = (vm_t*)allocate_block();
    
    if (!vm) {
        terminal_write("Error: Failed to allocate memory for VM\n");
        return -1;
    }
    
    // Initialize the VM
    strcpy(vm->name, name);
    vm->type = type;
    vm->state = VM_STATE_STOPPED;
    vm->vcpus = vcpus;
    vm->memory = memory;
    
    // Allocate memory for disks
    vm->disks = (vm_disk_t*)allocate_blocks((MAX_DISKS_PER_VM * sizeof(vm_disk_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!vm->disks) {
        terminal_write("Error: Failed to allocate memory for VM disks\n");
        free_block(vm);
        return -1;
    }
    
    vm->disk_count = 0;
    
    // Allocate memory for network interfaces
    vm->network_interfaces = (vm_network_interface_t*)allocate_blocks((MAX_NETWORK_INTERFACES_PER_VM * sizeof(vm_network_interface_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!vm->network_interfaces) {
        terminal_write("Error: Failed to allocate memory for VM network interfaces\n");
        free_blocks(vm->disks, (MAX_DISKS_PER_VM * sizeof(vm_disk_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
        free_block(vm);
        return -1;
    }
    
    vm->network_interface_count = 0;
    vm->custom_config = NULL;
    vm->private_data = NULL;
    
    // Add the VM to the array
    vms[vm_count++] = vm;
    
    terminal_write("Created VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Destroy a VM
int vm_destroy(const char* name) {
    if (!name) {
        terminal_write("Error: VM name cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    unsigned int index = 0;
    int found = 0;
    
    for (index = 0; index < vm_count; index++) {
        if (strcmp(vms[index]->name, name) == 0) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is running
    if (vms[index]->state == VM_STATE_RUNNING || vms[index]->state == VM_STATE_PAUSED) {
        terminal_write("Error: Cannot destroy a running or paused VM\n");
        return -1;
    }
    
    // Free the VM resources
    free_blocks(vms[index]->disks, (MAX_DISKS_PER_VM * sizeof(vm_disk_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    free_blocks(vms[index]->network_interfaces, (MAX_NETWORK_INTERFACES_PER_VM * sizeof(vm_network_interface_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (vms[index]->custom_config) {
        free_block(vms[index]->custom_config);
    }
    
    free_block(vms[index]);
    
    // Remove the VM from the array by shifting all subsequent VMs
    for (unsigned int i = index; i < vm_count - 1; i++) {
        vms[i] = vms[i + 1];
    }
    
    vms[--vm_count] = NULL;
    
    terminal_write("Destroyed VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Start a VM
int vm_start(const char* name) {
    if (!name) {
        terminal_write("Error: VM name cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    vm_t* vm = NULL;
    
    for (unsigned int i = 0; i < vm_count; i++) {
        if (strcmp(vms[i]->name, name) == 0) {
            vm = vms[i];
            break;
        }
    }
    
    if (!vm) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is already running
    if (vm->state == VM_STATE_RUNNING) {
        terminal_write("VM '");
        terminal_write(name);
        terminal_write("' is already running\n");
        return 0;
    }
    
    // Check if the VM has at least one disk
    if (vm->disk_count == 0) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' has no disks\n");
        return -1;
    }
    
    // Check if the VM has at least one boot disk
    int has_boot_disk = 0;
    
    for (unsigned int i = 0; i < vm->disk_count; i++) {
        if (vm->disks[i].boot) {
            has_boot_disk = 1;
            break;
        }
    }
    
    if (!has_boot_disk) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' has no boot disk\n");
        return -1;
    }
    
    // Start the VM based on its type
    switch (vm->type) {
        case VM_TYPE_KVM:
            // In a real system, we would use KVM to start the VM
            break;
        
        case VM_TYPE_QEMU:
            // In a real system, we would use QEMU to start the VM
            break;
        
        case VM_TYPE_VIRTUALBOX:
            // In a real system, we would use VirtualBox to start the VM
            break;
        
        case VM_TYPE_VMWARE:
            // In a real system, we would use VMware to start the VM
            break;
        
        case VM_TYPE_XEN:
            // In a real system, we would use Xen to start the VM
            break;
        
        case VM_TYPE_HYPER_V:
            // In a real system, we would use Hyper-V to start the VM
            break;
        
        case VM_TYPE_CUSTOM:
            // In a real system, we would use a custom hypervisor to start the VM
            break;
        
        default:
            terminal_write("Error: Unknown VM type\n");
            return -1;
    }
    
    // Update the VM state
    vm->state = VM_STATE_RUNNING;
    
    terminal_write("Started VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Stop a VM
int vm_stop(const char* name) {
    if (!name) {
        terminal_write("Error: VM name cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    vm_t* vm = NULL;
    
    for (unsigned int i = 0; i < vm_count; i++) {
        if (strcmp(vms[i]->name, name) == 0) {
            vm = vms[i];
            break;
        }
    }
    
    if (!vm) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is already stopped
    if (vm->state == VM_STATE_STOPPED) {
        terminal_write("VM '");
        terminal_write(name);
        terminal_write("' is already stopped\n");
        return 0;
    }
    
    // Stop the VM based on its type
    switch (vm->type) {
        case VM_TYPE_KVM:
            // In a real system, we would use KVM to stop the VM
            break;
        
        case VM_TYPE_QEMU:
            // In a real system, we would use QEMU to stop the VM
            break;
        
        case VM_TYPE_VIRTUALBOX:
            // In a real system, we would use VirtualBox to stop the VM
            break;
        
        case VM_TYPE_VMWARE:
            // In a real system, we would use VMware to stop the VM
            break;
        
        case VM_TYPE_XEN:
            // In a real system, we would use Xen to stop the VM
            break;
        
        case VM_TYPE_HYPER_V:
            // In a real system, we would use Hyper-V to stop the VM
            break;
        
        case VM_TYPE_CUSTOM:
            // In a real system, we would use a custom hypervisor to stop the VM
            break;
        
        default:
            terminal_write("Error: Unknown VM type\n");
            return -1;
    }
    
    // Update the VM state
    vm->state = VM_STATE_STOPPED;
    
    terminal_write("Stopped VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Pause a VM
int vm_pause(const char* name) {
    if (!name) {
        terminal_write("Error: VM name cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    vm_t* vm = NULL;
    
    for (unsigned int i = 0; i < vm_count; i++) {
        if (strcmp(vms[i]->name, name) == 0) {
            vm = vms[i];
            break;
        }
    }
    
    if (!vm) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is running
    if (vm->state != VM_STATE_RUNNING) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' is not running\n");
        return -1;
    }
    
    // Pause the VM based on its type
    switch (vm->type) {
        case VM_TYPE_KVM:
            // In a real system, we would use KVM to pause the VM
            break;
        
        case VM_TYPE_QEMU:
            // In a real system, we would use QEMU to pause the VM
            break;
        
        case VM_TYPE_VIRTUALBOX:
            // In a real system, we would use VirtualBox to pause the VM
            break;
        
        case VM_TYPE_VMWARE:
            // In a real system, we would use VMware to pause the VM
            break;
        
        case VM_TYPE_XEN:
            // In a real system, we would use Xen to pause the VM
            break;
        
        case VM_TYPE_HYPER_V:
            // In a real system, we would use Hyper-V to pause the VM
            break;
        
        case VM_TYPE_CUSTOM:
            // In a real system, we would use a custom hypervisor to pause the VM
            break;
        
        default:
            terminal_write("Error: Unknown VM type\n");
            return -1;
    }
    
    // Update the VM state
    vm->state = VM_STATE_PAUSED;
    
    terminal_write("Paused VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Resume a VM
int vm_resume(const char* name) {
    if (!name) {
        terminal_write("Error: VM name cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    vm_t* vm = NULL;
    
    for (unsigned int i = 0; i < vm_count; i++) {
        if (strcmp(vms[i]->name, name) == 0) {
            vm = vms[i];
            break;
        }
    }
    
    if (!vm) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is paused
    if (vm->state != VM_STATE_PAUSED) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' is not paused\n");
        return -1;
    }
    
    // Resume the VM based on its type
    switch (vm->type) {
        case VM_TYPE_KVM:
            // In a real system, we would use KVM to resume the VM
            break;
        
        case VM_TYPE_QEMU:
            // In a real system, we would use QEMU to resume the VM
            break;
        
        case VM_TYPE_VIRTUALBOX:
            // In a real system, we would use VirtualBox to resume the VM
            break;
        
        case VM_TYPE_VMWARE:
            // In a real system, we would use VMware to resume the VM
            break;
        
        case VM_TYPE_XEN:
            // In a real system, we would use Xen to resume the VM
            break;
        
        case VM_TYPE_HYPER_V:
            // In a real system, we would use Hyper-V to resume the VM
            break;
        
        case VM_TYPE_CUSTOM:
            // In a real system, we would use a custom hypervisor to resume the VM
            break;
        
        default:
            terminal_write("Error: Unknown VM type\n");
            return -1;
    }
    
    // Update the VM state
    vm->state = VM_STATE_RUNNING;
    
    terminal_write("Resumed VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Get a VM by name
vm_t* vm_get(const char* name) {
    if (!name) {
        return NULL;
    }
    
    // Find the VM
    for (unsigned int i = 0; i < vm_count; i++) {
        if (strcmp(vms[i]->name, name) == 0) {
            return vms[i];
        }
    }
    
    return NULL;
}

// Get the state of a VM
vm_state_t vm_get_state(const char* name) {
    vm_t* vm = vm_get(name);
    
    if (!vm) {
        return VM_STATE_ERROR;
    }
    
    return vm->state;
}

// Add a disk to a VM
int vm_add_disk(const char* name, const char* path, vm_disk_type_t type, unsigned long long size, unsigned int boot, unsigned int read_only) {
    if (!name || !path) {
        terminal_write("Error: VM name and disk path cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    vm_t* vm = vm_get(name);
    
    if (!vm) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is stopped
    if (vm->state != VM_STATE_STOPPED) {
        terminal_write("Error: Cannot add disk to a running or paused VM\n");
        return -1;
    }
    
    // Check if we have room for another disk
    if (vm->disk_count >= MAX_DISKS_PER_VM) {
        terminal_write("Error: Maximum number of disks reached for VM '");
        terminal_write(name);
        terminal_write("'\n");
        return -1;
    }
    
    // Check if a disk with the same path already exists
    for (unsigned int i = 0; i < vm->disk_count; i++) {
        if (strcmp(vm->disks[i].path, path) == 0) {
            terminal_write("Error: Disk with path '");
            terminal_write(path);
            terminal_write("' already exists for VM '");
            terminal_write(name);
            terminal_write("'\n");
            return -1;
        }
    }
    
    // Add the disk
    strcpy(vm->disks[vm->disk_count].path, path);
    vm->disks[vm->disk_count].type = type;
    vm->disks[vm->disk_count].size = size;
    vm->disks[vm->disk_count].boot = boot;
    vm->disks[vm->disk_count].read_only = read_only;
    
    vm->disk_count++;
    
    terminal_write("Added disk '");
    terminal_write(path);
    terminal_write("' to VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Add a network interface to a VM
int vm_add_network_interface(const char* name, const char* interface_name, vm_network_type_t type, const char* mac_address, const char* bridge) {
    if (!name || !interface_name) {
        terminal_write("Error: VM name and interface name cannot be NULL\n");
        return -1;
    }
    
    // Find the VM
    vm_t* vm = vm_get(name);
    
    if (!vm) {
        terminal_write("Error: VM '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the VM is stopped
    if (vm->state != VM_STATE_STOPPED) {
        terminal_write("Error: Cannot add network interface to a running or paused VM\n");
        return -1;
    }
    
    // Check if we have room for another network interface
    if (vm->network_interface_count >= MAX_NETWORK_INTERFACES_PER_VM) {
        terminal_write("Error: Maximum number of network interfaces reached for VM '");
        terminal_write(name);
        terminal_write("'\n");
        return -1;
    }
    
    // Check if a network interface with the same name already exists
    for (unsigned int i = 0; i < vm->network_interface_count; i++) {
        if (strcmp(vm->network_interfaces[i].name, interface_name) == 0) {
            terminal_write("Error: Network interface with name '");
            terminal_write(interface_name);
            terminal_write("' already exists for VM '");
            terminal_write(name);
            terminal_write("'\n");
            return -1;
        }
    }
    
    // Add the network interface
    strcpy(vm->network_interfaces[vm->network_interface_count].name, interface_name);
    vm->network_interfaces[vm->network_interface_count].type = type;
    
    if (mac_address) {
        strcpy(vm->network_interfaces[vm->network_interface_count].mac_address, mac_address);
    } else {
        // Generate a random MAC address
        vm_generate_mac_address(vm->network_interfaces[vm->network_interface_count].mac_address, 18);
    }
    
    if (bridge) {
        strcpy(vm->network_interfaces[vm->network_interface_count].bridge, bridge);
    } else {
        vm->network_interfaces[vm->network_interface_count].bridge[0] = '\0';
    }
    
    vm->network_interface_count++;
    
    terminal_write("Added network interface '");
    terminal_write(interface_name);
    terminal_write("' to VM '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

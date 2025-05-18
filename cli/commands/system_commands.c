/**
 * LightOS CLI
 * System Commands implementation
 */

#include "system_commands.h"
#include "../../kernel/kernel.h"
#include "../../package/package_manager.h"
#include "../../system/update_manager.h"
#include "../../system/backup_manager.h"
#include "../../system/monitor_manager.h"
#include "../../libc/string.h"

// Register system commands
void register_system_commands() {
    cli_register_command("system", system_command, "System management commands");
    cli_register_command("package", package_command, "Package management commands");
    cli_register_command("update", update_command, "System update commands");
    cli_register_command("backup", backup_command, "Backup and restore commands");
    cli_register_command("monitor", monitor_command, "System monitoring commands");
}

// System command handler
int system_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: system <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  info                                  Show system information\n");
        terminal_write("  status                                Show system status\n");
        terminal_write("  reboot                                Reboot the system\n");
        terminal_write("  shutdown                              Shut down the system\n");
        terminal_write("  package                               Package management commands\n");
        terminal_write("  update                                System update commands\n");
        terminal_write("  backup                                Backup and restore commands\n");
        terminal_write("  monitor                               System monitoring commands\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "info") == 0) {
        terminal_write("System Information:\n");
        terminal_write("  Name: LightOS\n");
        terminal_write("  Version: 1.0.0\n");
        terminal_write("  Kernel: LightOS Kernel 1.0.0\n");
        terminal_write("  Architecture: x86_64\n");
        terminal_write("  Hostname: lightos\n");
        terminal_write("  Uptime: ");
        // In a real system, we would get the actual uptime
        terminal_write("0 days, 0 hours, 5 minutes\n");
        
        return 0;
    }
    else if (strcmp(command, "status") == 0) {
        terminal_write("System Status:\n");
        terminal_write("  CPU Usage: ");
        // In a real system, we would get the actual CPU usage
        terminal_write("5%\n");
        
        terminal_write("  Memory Usage: ");
        // In a real system, we would get the actual memory usage
        terminal_write("128 MB / 1024 MB (12.5%)\n");
        
        terminal_write("  Disk Usage: ");
        // In a real system, we would get the actual disk usage
        terminal_write("1 GB / 10 GB (10%)\n");
        
        terminal_write("  Network Usage: ");
        // In a real system, we would get the actual network usage
        terminal_write("0 B/s in, 0 B/s out\n");
        
        terminal_write("  Processes: ");
        // In a real system, we would get the actual process count
        terminal_write("10\n");
        
        terminal_write("  Load Average: ");
        // In a real system, we would get the actual load average
        terminal_write("0.05, 0.10, 0.15\n");
        
        return 0;
    }
    else if (strcmp(command, "reboot") == 0) {
        terminal_write("Rebooting system...\n");
        // In a real system, we would reboot the system
        return 0;
    }
    else if (strcmp(command, "shutdown") == 0) {
        terminal_write("Shutting down system...\n");
        // In a real system, we would shut down the system
        return 0;
    }
    else if (strcmp(command, "package") == 0) {
        return package_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "update") == 0) {
        return update_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "backup") == 0) {
        return backup_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "monitor") == 0) {
        return monitor_command(argc - 1, argv + 1);
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Package command handler
int package_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: package <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  install <name> [version]              Install a package\n");
        terminal_write("  remove <name>                         Remove a package\n");
        terminal_write("  upgrade <name>                        Upgrade a package\n");
        terminal_write("  upgrade-all                           Upgrade all packages\n");
        terminal_write("  list                                  List all packages\n");
        terminal_write("  list-installed                        List installed packages\n");
        terminal_write("  list-available                        List available packages\n");
        terminal_write("  list-upgradable                       List upgradable packages\n");
        terminal_write("  search <query>                        Search for packages\n");
        terminal_write("  show <name>                           Show package information\n");
        terminal_write("  verify <name>                         Verify package integrity\n");
        terminal_write("  clean                                 Clean package cache\n");
        terminal_write("  autoremove                            Remove unused packages\n");
        terminal_write("  add-repository <name> <url> <dist> <component> <arch>  Add a repository\n");
        terminal_write("  remove-repository <name>              Remove a repository\n");
        terminal_write("  list-repositories                     List all repositories\n");
        terminal_write("  update-repository <name>              Update a repository\n");
        terminal_write("  update-all-repositories               Update all repositories\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "install") == 0) {
        if (argc < 3) {
            terminal_write("Usage: package install <name> [version]\n");
            return -1;
        }
        
        const char* name = argv[2];
        const char* version = argc > 3 ? argv[3] : NULL;
        
        return package_install(name, version);
    }
    else if (strcmp(command, "remove") == 0) {
        if (argc < 3) {
            terminal_write("Usage: package remove <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return package_remove(name);
    }
    else if (strcmp(command, "upgrade") == 0) {
        if (argc < 3) {
            terminal_write("Usage: package upgrade <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return package_upgrade(name);
    }
    else if (strcmp(command, "upgrade-all") == 0) {
        return package_upgrade_all();
    }
    else if (strcmp(command, "list") == 0) {
        package_t* packages[1000];
        unsigned int count = 0;
        
        if (package_list(packages, &count) != 0) {
            terminal_write("Error: Failed to list packages\n");
            return -1;
        }
        
        terminal_write("Packages:\n");
        
        if (count == 0) {
            terminal_write("  No packages found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(packages[i]->name);
            terminal_write(" (");
            terminal_write(packages[i]->version);
            terminal_write(", ");
            terminal_write(packages[i]->installed ? "installed" : "not installed");
            terminal_write(")\n");
        }
        
        return 0;
    }
    else if (strcmp(command, "list-installed") == 0) {
        package_t* packages[1000];
        unsigned int count = 0;
        
        if (package_list_installed(packages, &count) != 0) {
            terminal_write("Error: Failed to list installed packages\n");
            return -1;
        }
        
        terminal_write("Installed Packages:\n");
        
        if (count == 0) {
            terminal_write("  No installed packages found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(packages[i]->name);
            terminal_write(" (");
            terminal_write(packages[i]->version);
            terminal_write(")\n");
        }
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Update command handler
int update_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: update <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  check                                 Check for updates\n");
        terminal_write("  download <id>                         Download an update\n");
        terminal_write("  install <id>                          Install an update\n");
        terminal_write("  revert <id>                           Revert an update\n");
        terminal_write("  list                                  List all updates\n");
        terminal_write("  list-available                        List available updates\n");
        terminal_write("  list-installed                        List installed updates\n");
        terminal_write("  show <id>                             Show update information\n");
        terminal_write("  set-auto-check <enabled>              Set auto-check for updates\n");
        terminal_write("  set-auto-download <enabled>           Set auto-download for updates\n");
        terminal_write("  set-auto-install <enabled>            Set auto-install for updates\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "check") == 0) {
        return update_check();
    }
    else if (strcmp(command, "download") == 0) {
        if (argc < 3) {
            terminal_write("Usage: update download <id>\n");
            return -1;
        }
        
        const char* id = argv[2];
        
        return update_download(id);
    }
    else if (strcmp(command, "install") == 0) {
        if (argc < 3) {
            terminal_write("Usage: update install <id>\n");
            return -1;
        }
        
        const char* id = argv[2];
        
        return update_install(id);
    }
    else if (strcmp(command, "revert") == 0) {
        if (argc < 3) {
            terminal_write("Usage: update revert <id>\n");
            return -1;
        }
        
        const char* id = argv[2];
        
        return update_revert(id);
    }
    else if (strcmp(command, "list") == 0) {
        update_t* updates[100];
        unsigned int count = 0;
        
        if (update_list(updates, &count) != 0) {
            terminal_write("Error: Failed to list updates\n");
            return -1;
        }
        
        terminal_write("Updates:\n");
        
        if (count == 0) {
            terminal_write("  No updates found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(updates[i]->name);
            terminal_write(" (ID: ");
            terminal_write(updates[i]->id);
            terminal_write(", Version: ");
            terminal_write(updates[i]->version);
            terminal_write(", State: ");
            
            switch (updates[i]->state) {
                case UPDATE_STATE_AVAILABLE:
                    terminal_write("Available");
                    break;
                
                case UPDATE_STATE_DOWNLOADING:
                    terminal_write("Downloading");
                    break;
                
                case UPDATE_STATE_DOWNLOADED:
                    terminal_write("Downloaded");
                    break;
                
                case UPDATE_STATE_INSTALLING:
                    terminal_write("Installing");
                    break;
                
                case UPDATE_STATE_INSTALLED:
                    terminal_write("Installed");
                    break;
                
                case UPDATE_STATE_FAILED:
                    terminal_write("Failed");
                    break;
                
                case UPDATE_STATE_REVERTED:
                    terminal_write("Reverted");
                    break;
                
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(")\n");
        }
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Backup command handler
int backup_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: backup <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  create <name> [description] <type> <source> [destination]  Create a backup\n");
        terminal_write("  restore <id> [destination]            Restore a backup\n");
        terminal_write("  delete <id>                           Delete a backup\n");
        terminal_write("  list                                  List all backups\n");
        terminal_write("  show <id>                             Show backup information\n");
        terminal_write("  verify <id>                           Verify backup integrity\n");
        terminal_write("  extract <id> <destination> [pattern]  Extract files from a backup\n");
        terminal_write("  list-files <id>                       List files in a backup\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "create") == 0) {
        if (argc < 5) {
            terminal_write("Usage: backup create <name> [description] <type> <source> [destination]\n");
            return -1;
        }
        
        const char* name = argv[2];
        const char* description = NULL;
        const char* type_str = NULL;
        const char* source = NULL;
        const char* destination = NULL;
        
        if (argc >= 6) {
            description = argv[3];
            type_str = argv[4];
            source = argv[5];
            destination = argc > 6 ? argv[6] : NULL;
        } else {
            type_str = argv[3];
            source = argv[4];
            destination = argc > 5 ? argv[5] : NULL;
        }
        
        // Convert type string to enum
        backup_type_t type;
        if (strcmp(type_str, "full") == 0) {
            type = BACKUP_TYPE_FULL;
        } else if (strcmp(type_str, "incremental") == 0) {
            type = BACKUP_TYPE_INCREMENTAL;
        } else if (strcmp(type_str, "differential") == 0) {
            type = BACKUP_TYPE_DIFFERENTIAL;
        } else {
            terminal_write("Error: Invalid backup type\n");
            return -1;
        }
        
        return backup_create(name, description, type, source, destination);
    }
    else if (strcmp(command, "restore") == 0) {
        if (argc < 3) {
            terminal_write("Usage: backup restore <id> [destination]\n");
            return -1;
        }
        
        const char* id = argv[2];
        const char* destination = argc > 3 ? argv[3] : NULL;
        
        return backup_restore(id, destination);
    }
    else if (strcmp(command, "delete") == 0) {
        if (argc < 3) {
            terminal_write("Usage: backup delete <id>\n");
            return -1;
        }
        
        const char* id = argv[2];
        
        return backup_delete(id);
    }
    else if (strcmp(command, "list") == 0) {
        backup_t* backups[100];
        unsigned int count = 0;
        
        if (backup_list(backups, &count) != 0) {
            terminal_write("Error: Failed to list backups\n");
            return -1;
        }
        
        terminal_write("Backups:\n");
        
        if (count == 0) {
            terminal_write("  No backups found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(backups[i]->name);
            terminal_write(" (ID: ");
            terminal_write(backups[i]->id);
            terminal_write(", Type: ");
            
            switch (backups[i]->type) {
                case BACKUP_TYPE_FULL:
                    terminal_write("Full");
                    break;
                
                case BACKUP_TYPE_INCREMENTAL:
                    terminal_write("Incremental");
                    break;
                
                case BACKUP_TYPE_DIFFERENTIAL:
                    terminal_write("Differential");
                    break;
                
                case BACKUP_TYPE_CUSTOM:
                    terminal_write("Custom");
                    break;
                
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", Created: ");
            terminal_write(backups[i]->creation_date);
            terminal_write(")\n");
        }
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Monitor command handler
int monitor_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: monitor <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  add-resource <name> <type> <description> <interval>  Add a resource to monitor\n");
        terminal_write("  remove-resource <id>                  Remove a resource\n");
        terminal_write("  enable-resource <id>                  Enable a resource\n");
        terminal_write("  disable-resource <id>                 Disable a resource\n");
        terminal_write("  list-resources                        List all resources\n");
        terminal_write("  set-threshold <id> <warning> <error> <critical>  Set resource thresholds\n");
        terminal_write("  get-value <id>                        Get the current value of a resource\n");
        terminal_write("  list-alerts                           List all alerts\n");
        terminal_write("  acknowledge-alert <id>                Acknowledge an alert\n");
        terminal_write("  resolve-alert <id>                    Resolve an alert\n");
        terminal_write("  clear-alerts                          Clear all alerts\n");
        terminal_write("  cpu                                   Show CPU information\n");
        terminal_write("  memory                                Show memory information\n");
        terminal_write("  disk                                  Show disk information\n");
        terminal_write("  network                               Show network information\n");
        terminal_write("  process                               Show process information\n");
        terminal_write("  system                                Show system information\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "list-resources") == 0) {
        resource_t* resources[100];
        unsigned int count = 0;
        
        if (monitor_list_resources(resources, &count) != 0) {
            terminal_write("Error: Failed to list resources\n");
            return -1;
        }
        
        terminal_write("Monitored Resources:\n");
        
        if (count == 0) {
            terminal_write("  No resources found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(resources[i]->name);
            terminal_write(" (ID: ");
            terminal_write(resources[i]->id);
            terminal_write(", Type: ");
            
            switch (resources[i]->type) {
                case RESOURCE_TYPE_CPU:
                    terminal_write("CPU");
                    break;
                
                case RESOURCE_TYPE_MEMORY:
                    terminal_write("Memory");
                    break;
                
                case RESOURCE_TYPE_DISK:
                    terminal_write("Disk");
                    break;
                
                case RESOURCE_TYPE_NETWORK:
                    terminal_write("Network");
                    break;
                
                case RESOURCE_TYPE_PROCESS:
                    terminal_write("Process");
                    break;
                
                case RESOURCE_TYPE_SYSTEM:
                    terminal_write("System");
                    break;
                
                case RESOURCE_TYPE_CUSTOM:
                    terminal_write("Custom");
                    break;
                
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", Value: ");
            
            char value_str[16];
            sprintf(value_str, "%u", resources[i]->current_value);
            terminal_write(value_str);
            terminal_write(" ");
            terminal_write(resources[i]->unit);
            
            terminal_write(", State: ");
            terminal_write(resources[i]->enabled ? "Enabled" : "Disabled");
            terminal_write(")\n");
        }
        
        return 0;
    }
    else if (strcmp(command, "cpu") == 0) {
        char info[1024];
        
        if (monitor_get_cpu_info(info, sizeof(info)) != 0) {
            terminal_write("Error: Failed to get CPU information\n");
            return -1;
        }
        
        terminal_write("CPU Information:\n");
        terminal_write(info);
        terminal_write("\n");
        
        return 0;
    }
    else if (strcmp(command, "memory") == 0) {
        char info[1024];
        
        if (monitor_get_memory_info(info, sizeof(info)) != 0) {
            terminal_write("Error: Failed to get memory information\n");
            return -1;
        }
        
        terminal_write("Memory Information:\n");
        terminal_write(info);
        terminal_write("\n");
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

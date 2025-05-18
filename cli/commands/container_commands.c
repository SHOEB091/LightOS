/**
 * LightOS CLI
 * Container Commands implementation
 */

#include "container_commands.h"
#include "../../kernel/kernel.h"
#include "../../containerization/container_manager.h"
#include "../../libc/string.h"

// Register container commands
void register_container_commands() {
    cli_register_command("container", container_command, "Container management commands");
    cli_register_command("docker", docker_command, "Docker container management commands");
    cli_register_command("lxc", lxc_command, "LXC container management commands");
}

// Container command handler
int container_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: container <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  create <name> <image> [command]  Create a new container\n");
        terminal_write("  destroy <name>                   Destroy a container\n");
        terminal_write("  start <name>                     Start a container\n");
        terminal_write("  stop <name>                      Stop a container\n");
        terminal_write("  pause <name>                     Pause a container\n");
        terminal_write("  resume <name>                    Resume a paused container\n");
        terminal_write("  restart <name>                   Restart a container\n");
        terminal_write("  list                             List all containers\n");
        terminal_write("  exec <name> <command>            Execute a command in a container\n");
        terminal_write("  logs <name>                      Show container logs\n");
        terminal_write("  stats <name>                     Show container stats\n");
        terminal_write("  volume <subcommand> [options]    Manage container volumes\n");
        terminal_write("  network <subcommand> [options]   Manage container networks\n");
        terminal_write("  image <subcommand> [options]     Manage container images\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "create") == 0) {
        if (argc < 4) {
            terminal_write("Usage: container create <name> <image> [command]\n");
            return -1;
        }
        
        const char* name = argv[2];
        const char* image = argv[3];
        const char* cmd = argc > 4 ? argv[4] : NULL;
        
        return container_create(name, CONTAINER_TYPE_DOCKER, image, cmd);
    }
    else if (strcmp(command, "destroy") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container destroy <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return container_destroy(name);
    }
    else if (strcmp(command, "start") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container start <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return container_start(name);
    }
    else if (strcmp(command, "stop") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container stop <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return container_stop(name);
    }
    else if (strcmp(command, "pause") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container pause <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return container_pause(name);
    }
    else if (strcmp(command, "resume") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container resume <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return container_resume(name);
    }
    else if (strcmp(command, "restart") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container restart <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return container_restart(name);
    }
    else if (strcmp(command, "list") == 0) {
        container_t* containers[100];
        unsigned int count = 0;
        
        if (container_list(containers, &count) != 0) {
            terminal_write("Error: Failed to list containers\n");
            return -1;
        }
        
        terminal_write("CONTAINER ID\tNAME\t\tIMAGE\t\tSTATUS\n");
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write(containers[i]->id);
            terminal_write("\t");
            terminal_write(containers[i]->name);
            terminal_write("\t\t");
            terminal_write(containers[i]->image);
            terminal_write("\t\t");
            
            switch (containers[i]->state) {
                case CONTAINER_STATE_CREATED:
                    terminal_write("Created");
                    break;
                case CONTAINER_STATE_RUNNING:
                    terminal_write("Running");
                    break;
                case CONTAINER_STATE_PAUSED:
                    terminal_write("Paused");
                    break;
                case CONTAINER_STATE_STOPPED:
                    terminal_write("Stopped");
                    break;
                case CONTAINER_STATE_EXITED:
                    terminal_write("Exited");
                    break;
                case CONTAINER_STATE_ERROR:
                    terminal_write("Error");
                    break;
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write("\n");
        }
        
        return 0;
    }
    else if (strcmp(command, "exec") == 0) {
        if (argc < 4) {
            terminal_write("Usage: container exec <name> <command>\n");
            return -1;
        }
        
        const char* name = argv[2];
        const char* cmd = argv[3];
        char output[1024];
        
        if (container_exec(name, cmd, output, sizeof(output)) != 0) {
            terminal_write("Error: Failed to execute command\n");
            return -1;
        }
        
        terminal_write(output);
        terminal_write("\n");
        
        return 0;
    }
    else if (strcmp(command, "logs") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container logs <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        char logs[4096];
        
        if (container_logs(name, logs, sizeof(logs)) != 0) {
            terminal_write("Error: Failed to get logs\n");
            return -1;
        }
        
        terminal_write(logs);
        
        return 0;
    }
    else if (strcmp(command, "stats") == 0) {
        if (argc < 3) {
            terminal_write("Usage: container stats <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        unsigned int cpu_usage;
        unsigned long long memory_usage;
        unsigned long long network_rx;
        unsigned long long network_tx;
        
        if (container_stats(name, &cpu_usage, &memory_usage, &network_rx, &network_tx) != 0) {
            terminal_write("Error: Failed to get stats\n");
            return -1;
        }
        
        terminal_write("CPU Usage: ");
        // Convert cpu_usage to string
        char cpu_str[16];
        sprintf(cpu_str, "%u%%", cpu_usage);
        terminal_write(cpu_str);
        terminal_write("\n");
        
        terminal_write("Memory Usage: ");
        // Convert memory_usage to string
        char mem_str[32];
        sprintf(mem_str, "%llu MB", memory_usage / (1024 * 1024));
        terminal_write(mem_str);
        terminal_write("\n");
        
        terminal_write("Network RX: ");
        // Convert network_rx to string
        char rx_str[32];
        sprintf(rx_str, "%llu KB", network_rx / 1024);
        terminal_write(rx_str);
        terminal_write("\n");
        
        terminal_write("Network TX: ");
        // Convert network_tx to string
        char tx_str[32];
        sprintf(tx_str, "%llu KB", network_tx / 1024);
        terminal_write(tx_str);
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

// Docker command handler
int docker_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: docker <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  run <image> [command]            Run a container\n");
        terminal_write("  ps                               List containers\n");
        terminal_write("  images                           List images\n");
        terminal_write("  pull <image>                     Pull an image\n");
        terminal_write("  rmi <image>                      Remove an image\n");
        terminal_write("  network <subcommand> [options]   Manage networks\n");
        terminal_write("  volume <subcommand> [options]    Manage volumes\n");
        return 0;
    }
    
    // For now, just forward to the container command
    return container_command(argc, argv);
}

// LXC command handler
int lxc_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: lxc <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  create <name> <template>         Create a container\n");
        terminal_write("  destroy <name>                   Destroy a container\n");
        terminal_write("  start <name>                     Start a container\n");
        terminal_write("  stop <name>                      Stop a container\n");
        terminal_write("  list                             List containers\n");
        terminal_write("  snapshot <name> <snapshot>       Create a snapshot\n");
        terminal_write("  restore <name> <snapshot>        Restore a snapshot\n");
        return 0;
    }
    
    // For now, just forward to the container command
    return container_command(argc, argv);
}

/**
 * LightOS CLI
 * Command Line Interface implementation
 */

#include "cli.h"
#include "../kernel/kernel.h"
#include "../drivers/keyboard.h"
#include "../gui/gui.h"
#include "../server/server.h"
#include "../languages/language_manager.h"
#include "../languages/framework_manager.h"
#include "../drivers/driver_manager.h"
#include "../containerization/container_manager.h"
#include "../accessibility/accessibility_manager.h"
#include "../localization/localization_manager.h"
#include "../iot/iot_manager.h"
#include "../mobile/mobile_manager.h"
#include "commands/container_commands.h"
#include "commands/accessibility_commands.h"
#include "commands/iot_commands.h"
#include "commands/mobile_commands.h"
#include "commands/security_commands.h"
#include "commands/system_commands.h"

// Maximum command length
#define MAX_COMMAND_LENGTH 256

// Maximum number of arguments
#define MAX_ARGS 16

// Command history
#define HISTORY_SIZE 10
static char command_history[HISTORY_SIZE][MAX_COMMAND_LENGTH];
static int history_count = 0;
static int history_index = 0;

// Current working directory
static char current_directory[256] = "/";

// Initialize the CLI
void cli_init() {
    // Clear the command history
    for (int i = 0; i < HISTORY_SIZE; i++) {
        command_history[i][0] = '\0';
    }

    history_count = 0;
    history_index = 0;

    // Set the current directory
    current_directory[0] = '/';
    current_directory[1] = '\0';

    // Register container commands
    register_container_commands();

    // Register accessibility commands
    register_accessibility_commands();

    // Register IoT commands
    register_iot_commands();

    // Register mobile commands
    register_mobile_commands();

    // Register security commands
    register_security_commands();

    // Register system commands
    register_system_commands();
}

// Run the CLI
void cli_run() {
    char command[MAX_COMMAND_LENGTH];

    // Display welcome message
    terminal_clear();
    terminal_write("LightOS Command Line Interface\n");
    terminal_write("-----------------------------\n");
    terminal_write("Type 'help' for a list of commands.\n");
    terminal_write("\n");

    // Main command loop
    while (1) {
        // Display prompt
        terminal_write(current_directory);
        terminal_write("$ ");

        // Read command
        cli_read_command(command, MAX_COMMAND_LENGTH);

        // Add to history if not empty
        if (command[0] != '\0') {
            cli_add_to_history(command);
        }

        // Execute command
        if (command[0] != '\0') {
            cli_execute_command(command);
        }
    }
}

// Read a command from the keyboard
void cli_read_command(char* buffer, int max_length) {
    int i = 0;
    char c;

    // Clear the buffer
    buffer[0] = '\0';

    while (i < max_length - 1) {
        c = keyboard_read();

        if (c == '\n') {
            // End of line
            terminal_put_char('\n');
            buffer[i] = '\0';
            return;
        } else if (c == '\b') {
            // Backspace
            if (i > 0) {
                i--;
                terminal_put_char('\b');
                terminal_put_char(' ');
                terminal_put_char('\b');
            }
        } else if (c == 0x1B) {
            // Escape sequence (arrow keys, etc.)
            // In a real system, we would handle these
        } else {
            // Regular character
            buffer[i++] = c;
            terminal_put_char(c);
        }
    }

    // Buffer full
    buffer[i] = '\0';
}

// Add a command to the history
void cli_add_to_history(const char* command) {
    // Shift history if full
    if (history_count == HISTORY_SIZE) {
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            int j;
            for (j = 0; j < MAX_COMMAND_LENGTH - 1 && command_history[i+1][j] != '\0'; j++) {
                command_history[i][j] = command_history[i+1][j];
            }
            command_history[i][j] = '\0';
        }
        history_count--;
    }

    // Add the new command
    int i;
    for (i = 0; i < MAX_COMMAND_LENGTH - 1 && command[i] != '\0'; i++) {
        command_history[history_count][i] = command[i];
    }
    command_history[history_count][i] = '\0';

    history_count++;
    history_index = history_count;
}

// Parse a command into arguments
int cli_parse_command(char* command, char* args[], int max_args) {
    int arg_count = 0;
    int in_quotes = 0;
    char* p = command;

    // Skip leading whitespace
    while (*p && (*p == ' ' || *p == '\t')) {
        p++;
    }

    if (*p == '\0') {
        return 0; // Empty command
    }

    // Parse arguments
    args[arg_count++] = p;

    while (*p && arg_count < max_args) {
        if (*p == '"') {
            // Toggle quotes
            in_quotes = !in_quotes;

            // Remove the quote
            int i = 0;
            while (p[i]) {
                p[i] = p[i+1];
                i++;
            }

            continue;
        }

        if (*p == ' ' && !in_quotes) {
            // End of argument
            *p = '\0';
            p++;

            // Skip whitespace
            while (*p && (*p == ' ' || *p == '\t')) {
                p++;
            }

            if (*p) {
                args[arg_count++] = p;
            }
        } else {
            p++;
        }
    }

    return arg_count;
}

// Execute a command
void cli_execute_command(char* command) {
    char* args[MAX_ARGS];
    int arg_count = cli_parse_command(command, args, MAX_ARGS);

    if (arg_count == 0) {
        return; // Empty command
    }

    // Handle built-in commands
    if (cli_strcmp(args[0], "help") == 0) {
        cli_cmd_help();
    } else if (cli_strcmp(args[0], "clear") == 0) {
        cli_cmd_clear();
    } else if (cli_strcmp(args[0], "echo") == 0) {
        cli_cmd_echo(arg_count, args);
    } else if (cli_strcmp(args[0], "ls") == 0) {
        cli_cmd_ls(arg_count, args);
    } else if (cli_strcmp(args[0], "cd") == 0) {
        cli_cmd_cd(arg_count, args);
    } else if (cli_strcmp(args[0], "pwd") == 0) {
        cli_cmd_pwd();
    } else if (cli_strcmp(args[0], "server") == 0) {
        cli_cmd_server(arg_count, args);
    } else if (cli_strcmp(args[0], "gui") == 0) {
        cli_cmd_gui();
    } else if (cli_strcmp(args[0], "exit") == 0) {
        cli_cmd_exit();
    } else if (cli_strcmp(args[0], "lang") == 0) {
        cli_cmd_lang(arg_count, args);
    } else if (cli_strcmp(args[0], "framework") == 0) {
        cli_cmd_framework(arg_count, args);
    } else if (cli_strcmp(args[0], "device") == 0) {
        cli_cmd_device(arg_count, args);
    } else if (cli_strcmp(args[0], "container") == 0) {
        container_command(arg_count, args);
    } else if (cli_strcmp(args[0], "docker") == 0) {
        docker_command(arg_count, args);
    } else if (cli_strcmp(args[0], "lxc") == 0) {
        lxc_command(arg_count, args);
    } else if (cli_strcmp(args[0], "accessibility") == 0) {
        accessibility_command(arg_count, args);
    } else if (cli_strcmp(args[0], "screen-reader") == 0) {
        screen_reader_command(arg_count, args);
    } else if (cli_strcmp(args[0], "magnifier") == 0) {
        magnifier_command(arg_count, args);
    } else if (cli_strcmp(args[0], "high-contrast") == 0) {
        high_contrast_command(arg_count, args);
    } else if (cli_strcmp(args[0], "keyboard-helper") == 0) {
        keyboard_helper_command(arg_count, args);
    } else if (cli_strcmp(args[0], "iot") == 0) {
        iot_command(arg_count, args);
    } else if (cli_strcmp(args[0], "mqtt") == 0) {
        mqtt_command(arg_count, args);
    } else if (cli_strcmp(args[0], "coap") == 0) {
        coap_command(arg_count, args);
    } else if (cli_strcmp(args[0], "zigbee") == 0) {
        zigbee_command(arg_count, args);
    } else if (cli_strcmp(args[0], "sensor") == 0) {
        sensor_command(arg_count, args);
    } else if (cli_strcmp(args[0], "actuator") == 0) {
        actuator_command(arg_count, args);
    } else if (cli_strcmp(args[0], "mobile") == 0) {
        mobile_command(arg_count, args);
    } else if (cli_strcmp(args[0], "android") == 0) {
        android_command(arg_count, args);
    } else if (cli_strcmp(args[0], "ios") == 0) {
        ios_command(arg_count, args);
    } else if (cli_strcmp(args[0], "mobile-sync") == 0) {
        mobile_sync_command(arg_count, args);
    } else if (cli_strcmp(args[0], "security") == 0) {
        security_command(arg_count, args);
    } else if (cli_strcmp(args[0], "user") == 0) {
        user_command(arg_count, args);
    } else if (cli_strcmp(args[0], "group") == 0) {
        group_command(arg_count, args);
    } else if (cli_strcmp(args[0], "firewall") == 0) {
        firewall_command(arg_count, args);
    } else if (cli_strcmp(args[0], "crypto") == 0) {
        crypto_command(arg_count, args);
    } else if (cli_strcmp(args[0], "system") == 0) {
        system_command(arg_count, args);
    } else if (cli_strcmp(args[0], "package") == 0) {
        package_command(arg_count, args);
    } else if (cli_strcmp(args[0], "update") == 0) {
        update_command(arg_count, args);
    } else if (cli_strcmp(args[0], "backup") == 0) {
        backup_command(arg_count, args);
    } else if (cli_strcmp(args[0], "monitor") == 0) {
        monitor_command(arg_count, args);
    } else {
        // Unknown command
        terminal_write("Unknown command: ");
        terminal_write(args[0]);
        terminal_write("\n");
    }
}

// String comparison
int cli_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Help command
void cli_cmd_help() {
    terminal_write("Available commands:\n");
    terminal_write("  help       - Display this help message\n");
    terminal_write("  clear      - Clear the screen\n");
    terminal_write("  echo [text]- Display text\n");
    terminal_write("  ls [path]  - List directory contents\n");
    terminal_write("  cd [path]  - Change directory\n");
    terminal_write("  pwd        - Print working directory\n");
    terminal_write("  server     - Manage the server\n");
    terminal_write("    start    - Start the server\n");
    terminal_write("    stop     - Stop the server\n");
    terminal_write("    status   - Show server status\n");
    terminal_write("  gui        - Switch to GUI mode\n");
    terminal_write("  exit       - Exit the CLI\n");
    terminal_write("  lang       - Manage programming languages\n");
    terminal_write("    list     - List available languages\n");
    terminal_write("    install  - Install a language\n");
    terminal_write("    uninstall- Uninstall a language\n");
    terminal_write("    update   - Update a language\n");
    terminal_write("    default  - Set default language\n");
    terminal_write("    run      - Run a script\n");
    terminal_write("    repl     - Start a language REPL\n");
    terminal_write("  framework  - Manage programming frameworks\n");
    terminal_write("    list     - List available frameworks\n");
    terminal_write("    install  - Install a framework\n");
    terminal_write("    uninstall- Uninstall a framework\n");
    terminal_write("    update   - Update a framework\n");
    terminal_write("    create   - Create a project using a framework\n");
    terminal_write("    run      - Run a framework project\n");
    terminal_write("    build    - Build a framework project\n");
    terminal_write("  device     - Manage hardware devices\n");
    terminal_write("    list     - List all detected devices\n");
    terminal_write("    info     - Show detailed device information\n");
    terminal_write("    enable   - Enable a device\n");
    terminal_write("    disable  - Disable a device\n");
    terminal_write("    drivers  - List available device drivers\n");
    terminal_write("  container  - Container management commands\n");
    terminal_write("    create   - Create a new container\n");
    terminal_write("    destroy  - Destroy a container\n");
    terminal_write("    start    - Start a container\n");
    terminal_write("    stop     - Stop a container\n");
    terminal_write("    list     - List all containers\n");
    terminal_write("  docker     - Docker container management commands\n");
    terminal_write("  lxc        - LXC container management commands\n");
    terminal_write("  accessibility - Accessibility settings and features\n");
    terminal_write("    enable   - Enable an accessibility feature\n");
    terminal_write("    disable  - Disable an accessibility feature\n");
    terminal_write("    status   - Show the status of all accessibility features\n");
    terminal_write("  screen-reader - Screen reader settings and control\n");
    terminal_write("  magnifier  - Screen magnifier settings and control\n");
    terminal_write("  high-contrast - High contrast mode settings and control\n");
    terminal_write("  keyboard-helper - Keyboard accessibility settings and control\n");
    terminal_write("  iot        - IoT device management commands\n");
    terminal_write("    add      - Add an IoT device\n");
    terminal_write("    remove   - Remove an IoT device\n");
    terminal_write("    connect  - Connect to an IoT device\n");
    terminal_write("    list     - List all IoT devices\n");
    terminal_write("  mqtt       - MQTT protocol commands\n");
    terminal_write("  coap       - CoAP protocol commands\n");
    terminal_write("  zigbee     - Zigbee protocol commands\n");
    terminal_write("  sensor     - Sensor device commands\n");
    terminal_write("  actuator   - Actuator device commands\n");
    terminal_write("  mobile     - Mobile device management commands\n");
    terminal_write("    detect   - Detect connected mobile devices\n");
    terminal_write("    connect  - Connect to a mobile device\n");
    terminal_write("    list     - List all mobile devices\n");
    terminal_write("  android    - Android device management commands\n");
    terminal_write("  ios        - iOS device management commands\n");
    terminal_write("  mobile-sync - Mobile device synchronization commands\n");
    terminal_write("  security   - Security management commands\n");
    terminal_write("    user     - User management commands\n");
    terminal_write("    group    - Group management commands\n");
    terminal_write("    firewall - Firewall management commands\n");
    terminal_write("    crypto   - Cryptography commands\n");
    terminal_write("  system     - System management commands\n");
    terminal_write("    package  - Package management commands\n");
    terminal_write("    update   - System update commands\n");
    terminal_write("    backup   - Backup and restore commands\n");
    terminal_write("    monitor  - System monitoring commands\n");
}

// Clear command
void cli_cmd_clear() {
    terminal_clear();
}

// Echo command
void cli_cmd_echo(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        terminal_write(argv[i]);
        if (i < argc - 1) {
            terminal_put_char(' ');
        }
    }
    terminal_put_char('\n');
}

// List directory command
void cli_cmd_ls(int argc, char* argv[]) {
    // In a real system, we would list the directory contents
    // For now, just show some dummy files

    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 bin\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 boot\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 dev\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 etc\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 home\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 lib\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 mnt\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 proc\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 root\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 sbin\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 tmp\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 usr\n");
    terminal_write("drwxr-xr-x  2 root root  4096 Jan  1 00:00 var\n");
}

// Change directory command
void cli_cmd_cd(int argc, char* argv[]) {
    if (argc < 2) {
        // Change to root directory
        current_directory[0] = '/';
        current_directory[1] = '\0';
        return;
    }

    // In a real system, we would validate the path
    // For now, just set it

    if (argv[1][0] == '/') {
        // Absolute path
        int i;
        for (i = 0; i < 255 && argv[1][i] != '\0'; i++) {
            current_directory[i] = argv[1][i];
        }
        current_directory[i] = '\0';
    } else {
        // Relative path
        // This is a simplified implementation
        int len = 0;
        while (current_directory[len]) {
            len++;
        }

        if (current_directory[len-1] != '/') {
            current_directory[len++] = '/';
        }

        int i;
        for (i = 0; i < 255 - len && argv[1][i] != '\0'; i++) {
            current_directory[len+i] = argv[1][i];
        }
        current_directory[len+i] = '\0';
    }

    // Ensure the directory ends with a slash
    int len = 0;
    while (current_directory[len]) {
        len++;
    }

    if (len > 1 && current_directory[len-1] != '/') {
        current_directory[len] = '/';
        current_directory[len+1] = '\0';
    }
}

// Print working directory command
void cli_cmd_pwd() {
    terminal_write(current_directory);
    terminal_put_char('\n');
}

// Server command
void cli_cmd_server(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: server [start|stop|status]\n");
        return;
    }

    if (cli_strcmp(argv[1], "start") == 0) {
        if (server_is_running()) {
            terminal_write("Server is already running\n");
        } else {
            if (server_start() == 0) {
                terminal_write("Server started\n");
            } else {
                terminal_write("Failed to start server\n");
            }
        }
    } else if (cli_strcmp(argv[1], "stop") == 0) {
        if (!server_is_running()) {
            terminal_write("Server is not running\n");
        } else {
            server_stop();
            terminal_write("Server stopped\n");
        }
    } else if (cli_strcmp(argv[1], "status") == 0) {
        server_print_status();
    } else {
        terminal_write("Unknown server command: ");
        terminal_write(argv[1]);
        terminal_write("\n");
    }
}

// GUI command
void cli_cmd_gui() {
    terminal_write("Switching to GUI mode...\n");
    gui_switch_mode();
}

// Exit command
void cli_cmd_exit() {
    terminal_write("Exiting CLI...\n");
    // In a real system, we would exit the CLI
    // For now, just clear the screen
    terminal_clear();
}

// Language command
void cli_cmd_lang(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: lang [list|install|uninstall|update|default|run|repl]\n");
        return;
    }

    if (cli_strcmp(argv[1], "list") == 0) {
        cli_cmd_lang_list();
    } else if (cli_strcmp(argv[1], "install") == 0) {
        cli_cmd_lang_install(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "uninstall") == 0) {
        cli_cmd_lang_uninstall(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "update") == 0) {
        cli_cmd_lang_update(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "default") == 0) {
        cli_cmd_lang_set_default(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "run") == 0) {
        cli_cmd_lang_run(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "repl") == 0) {
        cli_cmd_lang_repl(argc - 1, argv + 1);
    } else {
        terminal_write("Unknown lang command: ");
        terminal_write(argv[1]);
        terminal_write("\n");
    }
}

// Framework command
void cli_cmd_framework(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: framework [list|install|uninstall|update|create|run|build]\n");
        return;
    }

    if (cli_strcmp(argv[1], "list") == 0) {
        cli_cmd_framework_list(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "install") == 0) {
        cli_cmd_framework_install(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "uninstall") == 0) {
        cli_cmd_framework_uninstall(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "update") == 0) {
        cli_cmd_framework_update(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "create") == 0) {
        cli_cmd_framework_create(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "run") == 0) {
        cli_cmd_framework_run(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "build") == 0) {
        cli_cmd_framework_build(argc - 1, argv + 1);
    } else {
        terminal_write("Unknown framework command: ");
        terminal_write(argv[1]);
        terminal_write("\n");
    }
}

// List available languages
void cli_cmd_lang_list() {
    language_manager_print_languages();
}

// Install a language
void cli_cmd_lang_install(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: lang install <language>\n");
        return;
    }

    // Find the language by name
    Language* language = language_manager_get_language_by_name(argv[1]);

    if (!language) {
        terminal_write("Unknown language: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (language->is_installed) {
        terminal_write("Language already installed: ");
        terminal_write(language->name);
        terminal_write("\n");
        return;
    }

    // Install the language
    if (language_manager_install_language(language->type) == 0) {
        terminal_write("Language installed: ");
        terminal_write(language->name);
        terminal_write("\n");
    } else {
        terminal_write("Failed to install language: ");
        terminal_write(language->name);
        terminal_write("\n");
    }
}

// Uninstall a language
void cli_cmd_lang_uninstall(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: lang uninstall <language>\n");
        return;
    }

    // Find the language by name
    Language* language = language_manager_get_language_by_name(argv[1]);

    if (!language) {
        terminal_write("Unknown language: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!language->is_installed) {
        terminal_write("Language not installed: ");
        terminal_write(language->name);
        terminal_write("\n");
        return;
    }

    // Uninstall the language
    if (language_manager_uninstall_language(language->type) == 0) {
        terminal_write("Language uninstalled: ");
        terminal_write(language->name);
        terminal_write("\n");
    } else {
        terminal_write("Failed to uninstall language: ");
        terminal_write(language->name);
        terminal_write("\n");
    }
}

// Update a language
void cli_cmd_lang_update(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: lang update <language> [version]\n");
        return;
    }

    // Find the language by name
    Language* language = language_manager_get_language_by_name(argv[1]);

    if (!language) {
        terminal_write("Unknown language: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!language->is_installed) {
        terminal_write("Language not installed: ");
        terminal_write(language->name);
        terminal_write("\n");
        return;
    }

    // Update to the latest version if no version specified
    if (argc < 3) {
        terminal_write("Updating to latest version...\n");

        // In a real system, we would determine the latest version
        LanguageVersion latest_version = language->current_version;
        latest_version.patch++; // Simulate a newer version

        if (language_manager_update_language(language->type, latest_version) == 0) {
            terminal_write("Language updated: ");
            terminal_write(language->name);
            terminal_write(" to version ");
            terminal_write(language_version_to_string(latest_version));
            terminal_write("\n");
        } else {
            terminal_write("Failed to update language: ");
            terminal_write(language->name);
            terminal_write("\n");
        }
    } else {
        // Parse the version string
        // This is a simplified implementation
        int major = 0, minor = 0, patch = 0;

        // Parse "major.minor.patch"
        char* version_str = argv[2];
        char* part = version_str;

        // Parse major
        while (*part && *part != '.') {
            if (*part >= '0' && *part <= '9') {
                major = major * 10 + (*part - '0');
            }
            part++;
        }

        // Parse minor
        if (*part == '.') {
            part++;
            while (*part && *part != '.') {
                if (*part >= '0' && *part <= '9') {
                    minor = minor * 10 + (*part - '0');
                }
                part++;
            }
        }

        // Parse patch
        if (*part == '.') {
            part++;
            while (*part) {
                if (*part >= '0' && *part <= '9') {
                    patch = patch * 10 + (*part - '0');
                }
                part++;
            }
        }

        LanguageVersion version = language_version_create(major, minor, patch, NULL);

        if (language_manager_update_language(language->type, version) == 0) {
            terminal_write("Language updated: ");
            terminal_write(language->name);
            terminal_write(" to version ");
            terminal_write(language_version_to_string(version));
            terminal_write("\n");
        } else {
            terminal_write("Failed to update language: ");
            terminal_write(language->name);
            terminal_write("\n");
        }
    }
}

// Set default language
void cli_cmd_lang_set_default(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: lang default <language>\n");
        return;
    }

    // Find the language by name
    Language* language = language_manager_get_language_by_name(argv[1]);

    if (!language) {
        terminal_write("Unknown language: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!language->is_installed) {
        terminal_write("Language not installed: ");
        terminal_write(language->name);
        terminal_write("\n");
        return;
    }

    // Set as default
    if (language_manager_set_default_language(language->type) == 0) {
        terminal_write("Default language set to: ");
        terminal_write(language->name);
        terminal_write("\n");
    } else {
        terminal_write("Failed to set default language: ");
        terminal_write(language->name);
        terminal_write("\n");
    }
}

// Run a script
void cli_cmd_lang_run(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: lang run <file> [args...]\n");
        return;
    }

    // Get the file extension
    char* file = argv[1];
    char* extension = NULL;

    // Find the last dot in the filename
    char* p = file;
    while (*p) {
        if (*p == '.') {
            extension = p;
        }
        p++;
    }

    if (!extension) {
        terminal_write("Error: File has no extension\n");
        return;
    }

    // Find the language by extension
    Language* language = language_manager_get_language_by_extension(extension);

    if (!language) {
        terminal_write("Error: Unknown file extension: ");
        terminal_write(extension);
        terminal_write("\n");
        return;
    }

    if (!language->is_installed) {
        terminal_write("Error: Language not installed: ");
        terminal_write(language->name);
        terminal_write("\n");
        return;
    }

    // Run the script
    if (language->run) {
        terminal_write("Running ");
        terminal_write(language->name);
        terminal_write(" script: ");
        terminal_write(file);
        terminal_write("\n");

        if (language->run(file) != 0) {
            terminal_write("Error running script\n");
        }
    } else {
        terminal_write("Error: Language does not support running scripts: ");
        terminal_write(language->name);
        terminal_write("\n");
    }
}

// Start a language REPL
void cli_cmd_lang_repl(int argc, char* argv[]) {
    if (argc < 2) {
        // Use the default language
        Language* language = language_manager_get_language(language_manager->default_language);

        if (!language->is_installed) {
            terminal_write("Error: Default language not installed: ");
            terminal_write(language->name);
            terminal_write("\n");
            return;
        }

        if (language->run_repl) {
            terminal_write("Starting ");
            terminal_write(language->name);
            terminal_write(" REPL...\n");

            if (language->run_repl() != 0) {
                terminal_write("Error starting REPL\n");
            }
        } else {
            terminal_write("Error: Default language does not support REPL: ");
            terminal_write(language->name);
            terminal_write("\n");
        }
    } else {
        // Find the language by name
        Language* language = language_manager_get_language_by_name(argv[1]);

        if (!language) {
            terminal_write("Unknown language: ");
            terminal_write(argv[1]);
            terminal_write("\n");
            return;
        }

        if (!language->is_installed) {
            terminal_write("Error: Language not installed: ");
            terminal_write(language->name);
            terminal_write("\n");
            return;
        }

        if (language->run_repl) {
            terminal_write("Starting ");
            terminal_write(language->name);
            terminal_write(" REPL...\n");

            if (language->run_repl() != 0) {
                terminal_write("Error starting REPL\n");
            }
        } else {
            terminal_write("Error: Language does not support REPL: ");
            terminal_write(language->name);
            terminal_write("\n");
        }
    }
}

// List available frameworks
void cli_cmd_framework_list(int argc, char* argv[]) {
    if (argc < 2) {
        // List all frameworks
        framework_manager_print_frameworks();
    } else {
        // List frameworks for a specific language
        Language* language = language_manager_get_language_by_name(argv[1]);

        if (!language) {
            terminal_write("Unknown language: ");
            terminal_write(argv[1]);
            terminal_write("\n");
            return;
        }

        framework_manager_print_frameworks_for_language(language->type);
    }
}

// Install a framework
void cli_cmd_framework_install(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: framework install <framework> [version]\n");
        return;
    }

    // Find the framework by name
    Framework* framework = framework_manager_get_framework(argv[1]);

    if (!framework) {
        terminal_write("Unknown framework: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (framework->is_installed) {
        terminal_write("Framework already installed: ");
        terminal_write(framework->name);
        terminal_write("\n");
        return;
    }

    // Install the framework
    FrameworkVersion version = framework->current_version;

    // Parse version if provided
    if (argc >= 3) {
        // Parse "major.minor.patch"
        int major = 0, minor = 0, patch = 0;
        char* version_str = argv[2];
        char* part = version_str;

        // Parse major
        while (*part && *part != '.') {
            if (*part >= '0' && *part <= '9') {
                major = major * 10 + (*part - '0');
            }
            part++;
        }

        // Parse minor
        if (*part == '.') {
            part++;
            while (*part && *part != '.') {
                if (*part >= '0' && *part <= '9') {
                    minor = minor * 10 + (*part - '0');
                }
                part++;
            }
        }

        // Parse patch
        if (*part == '.') {
            part++;
            while (*part) {
                if (*part >= '0' && *part <= '9') {
                    patch = patch * 10 + (*part - '0');
                }
                part++;
            }
        }

        version = framework_version_create(major, minor, patch, NULL);
    }

    if (framework_manager_install_framework(framework->name, version) == 0) {
        terminal_write("Framework installed: ");
        terminal_write(framework->name);
        terminal_write(" (");
        terminal_write(framework_version_to_string(version));
        terminal_write(")\n");
    } else {
        terminal_write("Failed to install framework: ");
        terminal_write(framework->name);
        terminal_write("\n");
    }
}

// Uninstall a framework
void cli_cmd_framework_uninstall(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: framework uninstall <framework>\n");
        return;
    }

    // Find the framework by name
    Framework* framework = framework_manager_get_framework(argv[1]);

    if (!framework) {
        terminal_write("Unknown framework: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework->name);
        terminal_write("\n");
        return;
    }

    // Uninstall the framework
    if (framework_manager_uninstall_framework(framework->name) == 0) {
        terminal_write("Framework uninstalled: ");
        terminal_write(framework->name);
        terminal_write("\n");
    } else {
        terminal_write("Failed to uninstall framework: ");
        terminal_write(framework->name);
        terminal_write("\n");
    }
}

// Update a framework
void cli_cmd_framework_update(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: framework update <framework> [version]\n");
        return;
    }

    // Find the framework by name
    Framework* framework = framework_manager_get_framework(argv[1]);

    if (!framework) {
        terminal_write("Unknown framework: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework->name);
        terminal_write("\n");
        return;
    }

    // Update to the latest version if no version specified
    if (argc < 3) {
        terminal_write("Updating to latest version...\n");

        // In a real system, we would determine the latest version
        FrameworkVersion latest_version = framework->current_version;
        latest_version.patch++; // Simulate a newer version

        if (framework_manager_update_framework(framework->name, latest_version) == 0) {
            terminal_write("Framework updated: ");
            terminal_write(framework->name);
            terminal_write(" to version ");
            terminal_write(framework_version_to_string(latest_version));
            terminal_write("\n");
        } else {
            terminal_write("Failed to update framework: ");
            terminal_write(framework->name);
            terminal_write("\n");
        }
    } else {
        // Parse the version string
        int major = 0, minor = 0, patch = 0;

        // Parse "major.minor.patch"
        char* version_str = argv[2];
        char* part = version_str;

        // Parse major
        while (*part && *part != '.') {
            if (*part >= '0' && *part <= '9') {
                major = major * 10 + (*part - '0');
            }
            part++;
        }

        // Parse minor
        if (*part == '.') {
            part++;
            while (*part && *part != '.') {
                if (*part >= '0' && *part <= '9') {
                    minor = minor * 10 + (*part - '0');
                }
                part++;
            }
        }

        // Parse patch
        if (*part == '.') {
            part++;
            while (*part) {
                if (*part >= '0' && *part <= '9') {
                    patch = patch * 10 + (*part - '0');
                }
                part++;
            }
        }

        FrameworkVersion version = framework_version_create(major, minor, patch, NULL);

        if (framework_manager_update_framework(framework->name, version) == 0) {
            terminal_write("Framework updated: ");
            terminal_write(framework->name);
            terminal_write(" to version ");
            terminal_write(framework_version_to_string(version));
            terminal_write("\n");
        } else {
            terminal_write("Failed to update framework: ");
            terminal_write(framework->name);
            terminal_write("\n");
        }
    }
}

// Create a project using a framework
void cli_cmd_framework_create(int argc, char* argv[]) {
    if (argc < 3) {
        terminal_write("Usage: framework create <framework> <project_name> [options]\n");
        return;
    }

    // Find the framework by name
    Framework* framework = framework_manager_get_framework(argv[1]);

    if (!framework) {
        terminal_write("Unknown framework: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework->name);
        terminal_write("\n");
        return;
    }

    // Get the project name
    const char* project_name = argv[2];

    // Get options if provided
    const char* options = NULL;
    if (argc >= 4) {
        options = argv[3];
    }

    // Create the project
    if (framework_manager_create_project(framework->name, project_name, options) == 0) {
        terminal_write("Project created: ");
        terminal_write(project_name);
        terminal_write(" using ");
        terminal_write(framework->name);
        terminal_write("\n");
    } else {
        terminal_write("Failed to create project: ");
        terminal_write(project_name);
        terminal_write("\n");
    }
}

// Run a framework project
void cli_cmd_framework_run(int argc, char* argv[]) {
    if (argc < 3) {
        terminal_write("Usage: framework run <framework> <project_path>\n");
        return;
    }

    // Find the framework by name
    Framework* framework = framework_manager_get_framework(argv[1]);

    if (!framework) {
        terminal_write("Unknown framework: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework->name);
        terminal_write("\n");
        return;
    }

    // Get the project path
    const char* project_path = argv[2];

    // Run the project
    if (framework_manager_run_project(framework->name, project_path) == 0) {
        terminal_write("Project running: ");
        terminal_write(project_path);
        terminal_write("\n");
    } else {
        terminal_write("Failed to run project: ");
        terminal_write(project_path);
        terminal_write("\n");
    }
}

// Build a framework project
void cli_cmd_framework_build(int argc, char* argv[]) {
    if (argc < 3) {
        terminal_write("Usage: framework build <framework> <project_path>\n");
        return;
    }

    // Find the framework by name
    Framework* framework = framework_manager_get_framework(argv[1]);

    if (!framework) {
        terminal_write("Unknown framework: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework->name);
        terminal_write("\n");
        return;
    }

    // Get the project path
    const char* project_path = argv[2];

    // Build the project
    if (framework_manager_build_project(framework->name, project_path) == 0) {
        terminal_write("Project built: ");
        terminal_write(project_path);
        terminal_write("\n");
    } else {
        terminal_write("Failed to build project: ");
        terminal_write(project_path);
        terminal_write("\n");
    }
}

// Device command
void cli_cmd_device(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: device [list|info|enable|disable|drivers]\n");
        return;
    }

    if (cli_strcmp(argv[1], "list") == 0) {
        cli_cmd_device_list(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "info") == 0) {
        cli_cmd_device_info(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "enable") == 0) {
        cli_cmd_device_enable(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "disable") == 0) {
        cli_cmd_device_disable(argc - 1, argv + 1);
    } else if (cli_strcmp(argv[1], "drivers") == 0) {
        cli_cmd_device_drivers(argc - 1, argv + 1);
    } else {
        terminal_write("Unknown device command: ");
        terminal_write(argv[1]);
        terminal_write("\n");
    }
}

// List all detected devices
void cli_cmd_device_list(int argc, char* argv[]) {
    // Call the driver manager to print all devices
    driver_manager_print_devices();
}

// Show detailed device information
void cli_cmd_device_info(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: device info <device_name>\n");
        return;
    }

    // Get the device by name
    Device* device = driver_manager_get_device_by_name(argv[1]);

    if (!device) {
        terminal_write("Device not found: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    // Print detailed device information
    terminal_write("Device Information:\n");
    terminal_write("------------------\n");

    terminal_write("Name: ");
    terminal_write(device->name);
    terminal_write("\n");

    terminal_write("Description: ");
    terminal_write(device->description);
    terminal_write("\n");

    terminal_write("Manufacturer: ");
    terminal_write(device->manufacturer);
    terminal_write("\n");

    terminal_write("Model: ");
    terminal_write(device->model);
    terminal_write("\n");

    terminal_write("Type: ");
    switch (device->type) {
        case DEVICE_KEYBOARD:
            terminal_write("Keyboard");
            break;
        case DEVICE_MOUSE:
            terminal_write("Mouse");
            break;
        case DEVICE_STORAGE:
            terminal_write("Storage");
            break;
        case DEVICE_DISPLAY:
            terminal_write("Display");
            break;
        case DEVICE_NETWORK:
            terminal_write("Network");
            break;
        case DEVICE_AUDIO:
            terminal_write("Audio");
            break;
        case DEVICE_PRINTER:
            terminal_write("Printer");
            break;
        case DEVICE_SCANNER:
            terminal_write("Scanner");
            break;
        case DEVICE_WEBCAM:
            terminal_write("Webcam");
            break;
        case DEVICE_GAMEPAD:
            terminal_write("Gamepad");
            break;
        case DEVICE_TOUCHSCREEN:
            terminal_write("Touchscreen");
            break;
        case DEVICE_BIOMETRIC:
            terminal_write("Biometric");
            break;
        default:
            terminal_write("Unknown");
            break;
    }
    terminal_write("\n");

    terminal_write("Bus: ");
    switch (device->bus) {
        case BUS_PCI:
            terminal_write("PCI");
            break;
        case BUS_USB:
            terminal_write("USB");
            break;
        case BUS_PS2:
            terminal_write("PS/2");
            break;
        case BUS_SATA:
            terminal_write("SATA");
            break;
        case BUS_IDE:
            terminal_write("IDE");
            break;
        case BUS_NVME:
            terminal_write("NVMe");
            break;
        case BUS_BLUETOOTH:
            terminal_write("Bluetooth");
            break;
        case BUS_WIRELESS:
            terminal_write("Wireless");
            break;
        default:
            terminal_write("Unknown");
            break;
    }
    terminal_write("\n");

    terminal_write("Status: ");
    switch (device->status) {
        case STATUS_DETECTED:
            terminal_write_color("DETECTED", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
            break;
        case STATUS_DRIVER_LOADED:
            terminal_write_color("DRIVER LOADED", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            break;
        case STATUS_ACTIVE:
            terminal_write_color("ACTIVE", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            break;
        case STATUS_ERROR:
            terminal_write_color("ERROR", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            break;
        case STATUS_DISABLED:
            terminal_write_color("DISABLED", VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
            break;
        default:
            terminal_write_color("UNKNOWN", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            break;
    }
    terminal_write("\n");

    terminal_write("Driver: ");
    if (device->driver) {
        Driver* driver = (Driver*)device->driver;
        terminal_write(driver->name);
        terminal_write(" (");
        terminal_write(driver->version);
        terminal_write(")");
    } else {
        terminal_write("None");
    }
    terminal_write("\n");
}

// Enable a device
void cli_cmd_device_enable(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: device enable <device_name>\n");
        return;
    }

    // Get the device by name
    Device* device = driver_manager_get_device_by_name(argv[1]);

    if (!device) {
        terminal_write("Device not found: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    // Check if the device is already enabled
    if (device->status == STATUS_ACTIVE) {
        terminal_write("Device is already enabled: ");
        terminal_write(device->name);
        terminal_write("\n");
        return;
    }

    // Load a driver for the device if needed
    if (!device->driver) {
        if (driver_manager_load_driver(device) != 0) {
            terminal_write("Failed to load driver for device: ");
            terminal_write(device->name);
            terminal_write("\n");
            return;
        }
    }

    // Enable the device
    device->status = STATUS_ACTIVE;

    terminal_write("Device enabled: ");
    terminal_write(device->name);
    terminal_write("\n");
}

// Disable a device
void cli_cmd_device_disable(int argc, char* argv[]) {
    if (argc < 2) {
        terminal_write("Usage: device disable <device_name>\n");
        return;
    }

    // Get the device by name
    Device* device = driver_manager_get_device_by_name(argv[1]);

    if (!device) {
        terminal_write("Device not found: ");
        terminal_write(argv[1]);
        terminal_write("\n");
        return;
    }

    // Check if the device is already disabled
    if (device->status == STATUS_DISABLED) {
        terminal_write("Device is already disabled: ");
        terminal_write(device->name);
        terminal_write("\n");
        return;
    }

    // Disable the device
    device->status = STATUS_DISABLED;

    terminal_write("Device disabled: ");
    terminal_write(device->name);
    terminal_write("\n");
}

// List available device drivers
void cli_cmd_device_drivers(int argc, char* argv[]) {
    // Call the driver manager to print all drivers
    driver_manager_print_drivers();
}

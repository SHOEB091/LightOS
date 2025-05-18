/**
 * LightOS Init
 * System initialization implementation
 */

#include "init.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../kernel/process.h"
#include "../kernel/filesystem.h"
#include "../drivers/keyboard.h"
#include "../networking/network.h"
#include "../server/server.h"
#include "../gui/gui.h"
#include "../cli/cli.h"
#include "../languages/language_manager.h"
#include "../languages/framework_manager.h"
#include "../drivers/driver_manager.h"
#include "../containerization/container_manager.h"
#include "../accessibility/accessibility_manager.h"
#include "../localization/localization_manager.h"
#include "../iot/iot_manager.h"
#include "../mobile/mobile_manager.h"

// Boot splash screen
static const char* boot_splash[] = {
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
};

// Initialize the system
void init_system() {
    // Display boot splash
    init_display_splash();

    // Initialize memory management
    terminal_write("Initializing memory management...\n");
    memory_init(64 * 1024 * 1024); // 64MB of RAM

    // Initialize process management
    terminal_write("Initializing process management...\n");
    process_init();

    // Initialize file system
    terminal_write("Initializing file system...\n");
    fs_init();

    // Initialize device drivers
    terminal_write("Initializing device drivers...\n");
    driver_manager_init();
    keyboard_init();

    // Initialize networking
    terminal_write("Initializing networking...\n");
    network_init();

    // Initialize server
    terminal_write("Initializing server...\n");
    server_init();

    // Initialize GUI
    terminal_write("Initializing GUI...\n");
    gui_init();

    // Initialize programming languages
    terminal_write("Initializing programming languages...\n");
    language_manager_init();

    // Initialize programming frameworks
    terminal_write("Initializing programming frameworks...\n");
    framework_manager_init();

    // Initialize container manager
    terminal_write("Initializing container manager...\n");
    container_manager_init();

    // Initialize accessibility manager
    terminal_write("Initializing accessibility manager...\n");
    accessibility_manager_init();

    // Initialize localization manager
    terminal_write("Initializing localization manager...\n");
    localization_manager_init();

    // Initialize IoT manager
    terminal_write("Initializing IoT manager...\n");
    iot_manager_init();

    // Initialize mobile manager
    terminal_write("Initializing mobile manager...\n");
    mobile_manager_init();

    // Initialize CLI
    terminal_write("Initializing CLI...\n");
    cli_init();

    // System initialization complete
    terminal_write("\nSystem initialization complete.\n");
    terminal_write("Starting CLI...\n\n");

    // Start the CLI
    cli_run();
}

// Display boot splash screen
void init_display_splash() {
    terminal_clear();

    // Display the splash screen
    for (int i = 0; i < 25; i++) {
        terminal_write(boot_splash[i]);
        terminal_write("\n");
    }

    // Display loading message
    terminal_write("\n\n");
    terminal_write_color("                      LightOS - A Fast, Resource-Efficient OS                      ", VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
    terminal_write("\n\n");
    terminal_write_color("                              System Loading...                              ", VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    terminal_write("\n\n");

    // Simulate loading
    for (int i = 0; i < 50; i++) {
        terminal_put_char('.');
        // In a real system, we would add a delay here
    }

    terminal_write("\n\n");
    terminal_clear();
}

// Start the system in server mode
void init_server_mode() {
    // Initialize the system
    init_system();

    // Start the server
    server_start();

    // Enter CLI mode
    cli_run();
}

/**
 * LightOS GUI
 * System Tray implementation
 */

#include "system_tray.h"
#include "gui.h"
#include "window_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "../drivers/network_driver.h"

// Maximum number of system tray icons
#define MAX_SYSTEM_TRAY_ICONS 16

// System tray window
static int system_tray_window_id = 0;

// System tray icons
static system_tray_icon_t system_tray_icons[MAX_SYSTEM_TRAY_ICONS];
static int system_tray_icon_count = 0;

// Next icon ID
static int next_icon_id = 1;

// System tray dimensions
static int system_tray_width = 200;
static int system_tray_height = 30;

// System tray position
static int system_tray_x = 0;
static int system_tray_y = 0;

// Notification window
static int notification_window_id = 0;

// System tray menu
static int system_tray_menu_window_id = 0;
static int system_tray_menu_item_count = 0;

// System tray event handler
static int system_tray_event_handler(window_event_t* event);

// Initialize the system tray
void system_tray_init() {
    terminal_write("Initializing system tray...\n");
    
    // Clear the icon array
    memset(system_tray_icons, 0, sizeof(system_tray_icons));
    system_tray_icon_count = 0;
    next_icon_id = 1;
    
    // Get the screen dimensions
    int screen_width = gui_get_screen_width();
    int screen_height = gui_get_screen_height();
    
    // Calculate the system tray position
    system_tray_x = screen_width - system_tray_width;
    system_tray_y = screen_height - system_tray_height;
    
    // Create the system tray window
    system_tray_window_id = window_create("System Tray", system_tray_x, system_tray_y, system_tray_width, system_tray_height, 
                                         WINDOW_FLAG_VISIBLE | WINDOW_FLAG_BORDER);
    
    if (system_tray_window_id == 0) {
        terminal_write("Error: Failed to create system tray window\n");
        return;
    }
    
    // Set the window event handler
    window_set_event_handler(system_tray_window_id, system_tray_event_handler);
    
    // Set the window background color
    window_set_background_color(system_tray_window_id, 0xE0E0E0);
    
    // Add built-in system tray icons
    system_tray_add_clock();
    system_tray_add_network();
    system_tray_add_volume();
    system_tray_add_battery();
    system_tray_add_notifications();
    
    terminal_write("System tray initialized\n");
}

// Add an icon to the system tray
int system_tray_add_icon(const char* name, const char* tooltip, void* icon_data, int icon_width, int icon_height, int (*click_handler)(int button)) {
    if (!name || !icon_data || icon_width <= 0 || icon_height <= 0) {
        return 0;
    }
    
    // Check if we have room for another icon
    if (system_tray_icon_count >= MAX_SYSTEM_TRAY_ICONS) {
        terminal_write("Error: Maximum number of system tray icons reached\n");
        return 0;
    }
    
    // Add the icon
    system_tray_icons[system_tray_icon_count].id = next_icon_id++;
    strncpy(system_tray_icons[system_tray_icon_count].name, name, 31);
    system_tray_icons[system_tray_icon_count].name[31] = '\0';
    
    if (tooltip) {
        strncpy(system_tray_icons[system_tray_icon_count].tooltip, tooltip, 63);
        system_tray_icons[system_tray_icon_count].tooltip[63] = '\0';
    } else {
        system_tray_icons[system_tray_icon_count].tooltip[0] = '\0';
    }
    
    system_tray_icons[system_tray_icon_count].icon_data = icon_data;
    system_tray_icons[system_tray_icon_count].icon_width = icon_width;
    system_tray_icons[system_tray_icon_count].icon_height = icon_height;
    system_tray_icons[system_tray_icon_count].click_handler = click_handler;
    system_tray_icons[system_tray_icon_count].user_data = NULL;
    
    int icon_id = system_tray_icons[system_tray_icon_count].id;
    system_tray_icon_count++;
    
    // Redraw the system tray
    system_tray_redraw();
    
    return icon_id;
}

// Remove an icon from the system tray
int system_tray_remove_icon(int icon_id) {
    // Find the icon
    int index = -1;
    
    for (int i = 0; i < system_tray_icon_count; i++) {
        if (system_tray_icons[i].id == icon_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // Remove the icon by shifting all subsequent icons
    for (int i = index; i < system_tray_icon_count - 1; i++) {
        system_tray_icons[i] = system_tray_icons[i + 1];
    }
    
    system_tray_icon_count--;
    
    // Redraw the system tray
    system_tray_redraw();
    
    return 0;
}

// Update an icon in the system tray
int system_tray_update_icon(int icon_id, void* icon_data, int icon_width, int icon_height) {
    if (!icon_data || icon_width <= 0 || icon_height <= 0) {
        return -1;
    }
    
    // Find the icon
    int index = -1;
    
    for (int i = 0; i < system_tray_icon_count; i++) {
        if (system_tray_icons[i].id == icon_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // Update the icon
    system_tray_icons[index].icon_data = icon_data;
    system_tray_icons[index].icon_width = icon_width;
    system_tray_icons[index].icon_height = icon_height;
    
    // Redraw the system tray
    system_tray_redraw();
    
    return 0;
}

// Update an icon's tooltip
int system_tray_update_tooltip(int icon_id, const char* tooltip) {
    if (!tooltip) {
        return -1;
    }
    
    // Find the icon
    int index = -1;
    
    for (int i = 0; i < system_tray_icon_count; i++) {
        if (system_tray_icons[i].id == icon_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // Update the tooltip
    strncpy(system_tray_icons[index].tooltip, tooltip, 63);
    system_tray_icons[index].tooltip[63] = '\0';
    
    return 0;
}

// Set user data for an icon
int system_tray_set_user_data(int icon_id, void* user_data) {
    // Find the icon
    int index = -1;
    
    for (int i = 0; i < system_tray_icon_count; i++) {
        if (system_tray_icons[i].id == icon_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // Set the user data
    system_tray_icons[index].user_data = user_data;
    
    return 0;
}

// Get user data for an icon
void* system_tray_get_user_data(int icon_id) {
    // Find the icon
    int index = -1;
    
    for (int i = 0; i < system_tray_icon_count; i++) {
        if (system_tray_icons[i].id == icon_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return NULL;
    }
    
    return system_tray_icons[index].user_data;
}

// Show a notification
int system_tray_show_notification(const char* title, const char* message, int timeout) {
    if (!title || !message) {
        return -1;
    }
    
    // Hide any existing notification
    system_tray_hide_notification();
    
    // Get the screen dimensions
    int screen_width = gui_get_screen_width();
    
    // Create the notification window
    notification_window_id = window_create("Notification", screen_width - 300, 50, 250, 100, 
                                          WINDOW_FLAG_VISIBLE | WINDOW_FLAG_BORDER);
    
    if (notification_window_id == 0) {
        terminal_write("Error: Failed to create notification window\n");
        return -1;
    }
    
    // Set the window background color
    window_set_background_color(notification_window_id, 0xF0F0F0);
    
    // Draw the notification
    window_draw_text(notification_window_id, 10, 10, title, 0x000000);
    window_draw_line(notification_window_id, 10, 30, 240, 30, 0xA0A0A0);
    window_draw_text(notification_window_id, 10, 40, message, 0x000000);
    
    // Set a timer to hide the notification
    if (timeout > 0) {
        // In a real system, we would set a timer here
    }
    
    return 0;
}

// Hide the notification
int system_tray_hide_notification() {
    if (notification_window_id == 0) {
        return 0;
    }
    
    // Destroy the notification window
    window_destroy(notification_window_id);
    notification_window_id = 0;
    
    return 0;
}

// Check if the system tray is visible
int system_tray_is_visible() {
    if (system_tray_window_id == 0) {
        return 0;
    }
    
    window_t* window = window_get(system_tray_window_id);
    
    if (!window) {
        return 0;
    }
    
    return (window->flags & WINDOW_FLAG_VISIBLE) != 0;
}

// Show the system tray
int system_tray_show() {
    if (system_tray_window_id == 0) {
        return -1;
    }
    
    window_show(system_tray_window_id);
    
    return 0;
}

// Hide the system tray
int system_tray_hide() {
    if (system_tray_window_id == 0) {
        return -1;
    }
    
    window_hide(system_tray_window_id);
    
    return 0;
}

// Redraw the system tray
static void system_tray_redraw() {
    if (system_tray_window_id == 0) {
        return;
    }
    
    // Clear the window
    window_fill_rect(system_tray_window_id, 0, 0, system_tray_width, system_tray_height, 0xE0E0E0);
    
    // Draw the icons
    int x = 5;
    
    for (int i = 0; i < system_tray_icon_count; i++) {
        // Draw the icon
        window_draw_image(system_tray_window_id, x, (system_tray_height - system_tray_icons[i].icon_height) / 2, 
                         system_tray_icons[i].icon_width, system_tray_icons[i].icon_height, 
                         system_tray_icons[i].icon_data);
        
        x += system_tray_icons[i].icon_width + 5;
    }
}

// System tray event handler
static int system_tray_event_handler(window_event_t* event) {
    if (!event) {
        return -1;
    }
    
    switch (event->type) {
        case WINDOW_EVENT_MOUSE_DOWN:
            // Check if an icon was clicked
            int x = event->data.mouse.x;
            int icon_x = 5;
            
            for (int i = 0; i < system_tray_icon_count; i++) {
                if (x >= icon_x && x < icon_x + system_tray_icons[i].icon_width) {
                    // Call the icon's click handler
                    if (system_tray_icons[i].click_handler) {
                        system_tray_icons[i].click_handler(event->data.mouse.buttons);
                    }
                    
                    break;
                }
                
                icon_x += system_tray_icons[i].icon_width + 5;
            }
            break;
        
        default:
            break;
    }
    
    return 0;
}

// Add a clock to the system tray
void system_tray_add_clock() {
    // In a real system, we would create a clock icon and update it periodically
    // For now, just add a placeholder
    
    // Create a simple clock icon (16x16 pixels)
    unsigned int* icon_data = (unsigned int*)allocate_block();
    
    if (!icon_data) {
        terminal_write("Error: Failed to allocate memory for clock icon\n");
        return;
    }
    
    // Fill the icon with a clock-like pattern
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int dx = x - 8;
            int dy = y - 8;
            int dist = dx * dx + dy * dy;
            
            if (dist < 64) {
                icon_data[y * 16 + x] = 0xFFFFFF;
            } else {
                icon_data[y * 16 + x] = 0x000000;
            }
        }
    }
    
    // Add the icon to the system tray
    system_tray_add_icon("Clock", "Current time", icon_data, 16, 16, NULL);
}

// Add a network icon to the system tray
void system_tray_add_network() {
    // In a real system, we would create a network icon and update it based on network status
    // For now, just add a placeholder
    
    // Create a simple network icon (16x16 pixels)
    unsigned int* icon_data = (unsigned int*)allocate_block();
    
    if (!icon_data) {
        terminal_write("Error: Failed to allocate memory for network icon\n");
        return;
    }
    
    // Fill the icon with a network-like pattern
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            if (y > 12 - x / 2) {
                icon_data[y * 16 + x] = 0x00FF00;
            } else {
                icon_data[y * 16 + x] = 0x000000;
            }
        }
    }
    
    // Add the icon to the system tray
    system_tray_add_icon("Network", "Network status", icon_data, 16, 16, NULL);
}

// Add a volume icon to the system tray
void system_tray_add_volume() {
    // In a real system, we would create a volume icon and update it based on volume level
    // For now, just add a placeholder
    
    // Create a simple volume icon (16x16 pixels)
    unsigned int* icon_data = (unsigned int*)allocate_block();
    
    if (!icon_data) {
        terminal_write("Error: Failed to allocate memory for volume icon\n");
        return;
    }
    
    // Fill the icon with a volume-like pattern
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            if (x < 8 && y >= 4 && y < 12) {
                icon_data[y * 16 + x] = 0x0000FF;
            } else if (x >= 8 && y >= 8 - (x - 8) && y < 8 + (x - 8)) {
                icon_data[y * 16 + x] = 0x0000FF;
            } else {
                icon_data[y * 16 + x] = 0x000000;
            }
        }
    }
    
    // Add the icon to the system tray
    system_tray_add_icon("Volume", "Volume level", icon_data, 16, 16, NULL);
}

// Add a battery icon to the system tray
void system_tray_add_battery() {
    // In a real system, we would create a battery icon and update it based on battery level
    // For now, just add a placeholder
    
    // Create a simple battery icon (16x16 pixels)
    unsigned int* icon_data = (unsigned int*)allocate_block();
    
    if (!icon_data) {
        terminal_write("Error: Failed to allocate memory for battery icon\n");
        return;
    }
    
    // Fill the icon with a battery-like pattern
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            if (x >= 2 && x < 14 && y >= 4 && y < 12) {
                if (x < 10) {
                    icon_data[y * 16 + x] = 0x00FF00;
                } else {
                    icon_data[y * 16 + x] = 0x000000;
                }
            } else if (x >= 14 && x < 16 && y >= 6 && y < 10) {
                icon_data[y * 16 + x] = 0x000000;
            } else {
                icon_data[y * 16 + x] = 0x000000;
            }
        }
    }
    
    // Add the icon to the system tray
    system_tray_add_icon("Battery", "Battery level", icon_data, 16, 16, NULL);
}

// Add a notifications icon to the system tray
void system_tray_add_notifications() {
    // In a real system, we would create a notifications icon and update it based on notification count
    // For now, just add a placeholder
    
    // Create a simple notifications icon (16x16 pixels)
    unsigned int* icon_data = (unsigned int*)allocate_block();
    
    if (!icon_data) {
        terminal_write("Error: Failed to allocate memory for notifications icon\n");
        return;
    }
    
    // Fill the icon with a notification-like pattern
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int dx = x - 8;
            int dy = y - 8;
            int dist = dx * dx + dy * dy;
            
            if (dist < 36) {
                icon_data[y * 16 + x] = 0xFF0000;
            } else {
                icon_data[y * 16 + x] = 0x000000;
            }
        }
    }
    
    // Add the icon to the system tray
    system_tray_add_icon("Notifications", "Notifications", icon_data, 16, 16, NULL);
}

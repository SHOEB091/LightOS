/**
 * LightOS GUI
 * Admin Panel implementation
 */

#include "admin_panel.h"
#include "gui.h"
#include "window_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../kernel/filesystem.h"
#include "../drivers/driver_manager.h"
#include "../networking/network.h"
#include "../server/server.h"
#include "../package-manager/package_manager.h"
#include "../security/security_manager.h"
#include "../performance/performance_monitor.h"
#include "../libc/string.h"

// Admin panel window
static int admin_window_id = 0;

// Current section
static admin_section_t current_section = ADMIN_SECTION_DASHBOARD;

// Section buttons
static int section_buttons[10] = {0};

// Content area
static int content_area_x = 200;
static int content_area_y = 50;
static int content_area_width = 600;
static int content_area_height = 500;

// Admin panel event handler
static int admin_panel_event_handler(window_event_t* event);

// Initialize the admin panel
void admin_panel_init() {
    terminal_write("Initializing admin panel...\n");
    
    // Initialize sections
    admin_dashboard_init();
    admin_users_init();
    admin_system_init();
    admin_network_init();
    admin_storage_init();
    admin_packages_init();
    admin_services_init();
    admin_security_init();
    admin_logs_init();
    admin_about_init();
    
    terminal_write("Admin panel initialized\n");
}

// Show the admin panel
int admin_panel_show() {
    // If the admin panel is already visible, just return
    if (admin_window_id != 0) {
        window_show(admin_window_id);
        return 0;
    }
    
    // Create the admin panel window
    admin_window_id = window_create("LightOS Admin Panel", 100, 50, 800, 600, 
                                   WINDOW_FLAG_VISIBLE | WINDOW_FLAG_BORDER | 
                                   WINDOW_FLAG_TITLEBAR | WINDOW_FLAG_RESIZABLE);
    
    if (admin_window_id == 0) {
        terminal_write("Error: Failed to create admin panel window\n");
        return -1;
    }
    
    // Set the window event handler
    window_set_event_handler(admin_window_id, admin_panel_event_handler);
    
    // Set the window background color
    window_set_background_color(admin_window_id, 0xF0F0F0);
    
    // Create section buttons
    int button_y = 50;
    
    section_buttons[ADMIN_SECTION_DASHBOARD] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Dashboard");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_USERS] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Users");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_SYSTEM] = window_create_button(admin_window_id, 10, button_y, 180, 30, "System");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_NETWORK] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Network");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_STORAGE] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Storage");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_PACKAGES] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Packages");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_SERVICES] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Services");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_SECURITY] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Security");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_LOGS] = window_create_button(admin_window_id, 10, button_y, 180, 30, "Logs");
    button_y += 40;
    
    section_buttons[ADMIN_SECTION_ABOUT] = window_create_button(admin_window_id, 10, button_y, 180, 30, "About");
    
    // Draw the sidebar
    window_fill_rect(admin_window_id, 0, 0, 200, 600, 0xE0E0E0);
    window_draw_line(admin_window_id, 200, 0, 200, 600, 0xA0A0A0);
    
    // Draw the header
    window_fill_rect(admin_window_id, 0, 0, 800, 40, 0x4080C0);
    window_draw_text(admin_window_id, 10, 10, "LightOS Admin Panel", 0xFFFFFF);
    
    // Show the current section
    admin_panel_set_section(current_section);
    
    return 0;
}

// Hide the admin panel
int admin_panel_hide() {
    if (admin_window_id == 0) {
        return 0;
    }
    
    // Hide the window
    window_hide(admin_window_id);
    
    return 0;
}

// Check if the admin panel is visible
int admin_panel_is_visible() {
    if (admin_window_id == 0) {
        return 0;
    }
    
    window_t* window = window_get(admin_window_id);
    
    if (!window) {
        return 0;
    }
    
    return (window->flags & WINDOW_FLAG_VISIBLE) != 0;
}

// Set the current section
int admin_panel_set_section(admin_section_t section) {
    if (section < ADMIN_SECTION_DASHBOARD || section > ADMIN_SECTION_ABOUT) {
        return -1;
    }
    
    // Hide the current section
    switch (current_section) {
        case ADMIN_SECTION_DASHBOARD:
            admin_dashboard_hide();
            break;
        case ADMIN_SECTION_USERS:
            admin_users_hide();
            break;
        case ADMIN_SECTION_SYSTEM:
            admin_system_hide();
            break;
        case ADMIN_SECTION_NETWORK:
            admin_network_hide();
            break;
        case ADMIN_SECTION_STORAGE:
            admin_storage_hide();
            break;
        case ADMIN_SECTION_PACKAGES:
            admin_packages_hide();
            break;
        case ADMIN_SECTION_SERVICES:
            admin_services_hide();
            break;
        case ADMIN_SECTION_SECURITY:
            admin_security_hide();
            break;
        case ADMIN_SECTION_LOGS:
            admin_logs_hide();
            break;
        case ADMIN_SECTION_ABOUT:
            admin_about_hide();
            break;
    }
    
    // Update the current section
    current_section = section;
    
    // Show the new section
    switch (current_section) {
        case ADMIN_SECTION_DASHBOARD:
            admin_dashboard_show();
            break;
        case ADMIN_SECTION_USERS:
            admin_users_show();
            break;
        case ADMIN_SECTION_SYSTEM:
            admin_system_show();
            break;
        case ADMIN_SECTION_NETWORK:
            admin_network_show();
            break;
        case ADMIN_SECTION_STORAGE:
            admin_storage_show();
            break;
        case ADMIN_SECTION_PACKAGES:
            admin_packages_show();
            break;
        case ADMIN_SECTION_SERVICES:
            admin_services_show();
            break;
        case ADMIN_SECTION_SECURITY:
            admin_security_show();
            break;
        case ADMIN_SECTION_LOGS:
            admin_logs_show();
            break;
        case ADMIN_SECTION_ABOUT:
            admin_about_show();
            break;
    }
    
    // Update the section title
    window_fill_rect(admin_window_id, 201, 41, 599, 30, 0xF0F0F0);
    
    const char* section_title = "";
    
    switch (current_section) {
        case ADMIN_SECTION_DASHBOARD:
            section_title = "Dashboard";
            break;
        case ADMIN_SECTION_USERS:
            section_title = "User Management";
            break;
        case ADMIN_SECTION_SYSTEM:
            section_title = "System Management";
            break;
        case ADMIN_SECTION_NETWORK:
            section_title = "Network Configuration";
            break;
        case ADMIN_SECTION_STORAGE:
            section_title = "Storage Management";
            break;
        case ADMIN_SECTION_PACKAGES:
            section_title = "Package Management";
            break;
        case ADMIN_SECTION_SERVICES:
            section_title = "Service Management";
            break;
        case ADMIN_SECTION_SECURITY:
            section_title = "Security Settings";
            break;
        case ADMIN_SECTION_LOGS:
            section_title = "System Logs";
            break;
        case ADMIN_SECTION_ABOUT:
            section_title = "About LightOS";
            break;
    }
    
    window_draw_text(admin_window_id, 220, 50, section_title, 0x000000);
    
    // Highlight the current section button
    for (int i = ADMIN_SECTION_DASHBOARD; i <= ADMIN_SECTION_ABOUT; i++) {
        if (i == current_section) {
            window_button_set_background_color(section_buttons[i], 0xA0C0E0);
        } else {
            window_button_set_background_color(section_buttons[i], 0xF0F0F0);
        }
    }
    
    return 0;
}

// Get the current section
admin_section_t admin_panel_get_section() {
    return current_section;
}

// Refresh the admin panel
int admin_panel_refresh() {
    if (admin_window_id == 0) {
        return -1;
    }
    
    // Refresh the current section
    switch (current_section) {
        case ADMIN_SECTION_DASHBOARD:
            admin_dashboard_refresh();
            break;
        case ADMIN_SECTION_USERS:
            admin_users_refresh();
            break;
        case ADMIN_SECTION_SYSTEM:
            admin_system_refresh();
            break;
        case ADMIN_SECTION_NETWORK:
            admin_network_refresh();
            break;
        case ADMIN_SECTION_STORAGE:
            admin_storage_refresh();
            break;
        case ADMIN_SECTION_PACKAGES:
            admin_packages_refresh();
            break;
        case ADMIN_SECTION_SERVICES:
            admin_services_refresh();
            break;
        case ADMIN_SECTION_SECURITY:
            admin_security_refresh();
            break;
        case ADMIN_SECTION_LOGS:
            admin_logs_refresh();
            break;
        case ADMIN_SECTION_ABOUT:
            admin_about_refresh();
            break;
    }
    
    return 0;
}

// Admin panel event handler
static int admin_panel_event_handler(window_event_t* event) {
    if (!event) {
        return -1;
    }
    
    switch (event->type) {
        case WINDOW_EVENT_CLOSE:
            // Hide the admin panel
            admin_panel_hide();
            break;
        
        case WINDOW_EVENT_BUTTON_CLICK:
            // Check if a section button was clicked
            for (int i = ADMIN_SECTION_DASHBOARD; i <= ADMIN_SECTION_ABOUT; i++) {
                if (event->data.button.id == section_buttons[i]) {
                    admin_panel_set_section(i);
                    break;
                }
            }
            break;
        
        default:
            break;
    }
    
    return 0;
}

// Dashboard section implementation
void admin_dashboard_init() {
    // Initialize dashboard widgets
}

void admin_dashboard_show() {
    if (admin_window_id == 0) {
        return;
    }
    
    // Clear the content area
    window_fill_rect(admin_window_id, content_area_x, content_area_y, content_area_width, content_area_height, 0xFFFFFF);
    
    // Draw the dashboard
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 20, "System Overview", 0x000000);
    
    // Draw system information
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 60, "CPU Usage:", 0x000000);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 60, 200, 20, 0xE0E0E0);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 60, 100, 20, 0x4080C0);
    window_draw_text(admin_window_id, content_area_x + 330, content_area_y + 60, "50%", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 90, "Memory Usage:", 0x000000);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 90, 200, 20, 0xE0E0E0);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 90, 80, 20, 0x4080C0);
    window_draw_text(admin_window_id, content_area_x + 330, content_area_y + 90, "40%", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 120, "Disk Usage:", 0x000000);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 120, 200, 20, 0xE0E0E0);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 120, 120, 20, 0x4080C0);
    window_draw_text(admin_window_id, content_area_x + 330, content_area_y + 120, "60%", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 150, "Network Usage:", 0x000000);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 150, 200, 20, 0xE0E0E0);
    window_fill_rect(admin_window_id, content_area_x + 120, content_area_y + 150, 30, 20, 0x4080C0);
    window_draw_text(admin_window_id, content_area_x + 330, content_area_y + 150, "15%", 0x000000);
    
    // Draw system status
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 200, "System Status", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 230, "Uptime:", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 120, content_area_y + 230, "2 days, 3 hours, 45 minutes", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 260, "Load Average:", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 120, content_area_y + 260, "0.52, 0.48, 0.45", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 290, "Processes:", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 120, content_area_y + 290, "87", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 320, "Users:", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 120, content_area_y + 320, "3", 0x000000);
    
    // Draw recent alerts
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 370, "Recent Alerts", 0x000000);
    
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 400, "- System update available", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 430, "- Disk space low on /var", 0xFF0000);
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 460, "- 3 failed login attempts", 0xFF0000);
}

void admin_dashboard_hide() {
    // Nothing to do
}

void admin_dashboard_refresh() {
    admin_dashboard_show();
}

// Users section implementation
void admin_users_init() {
    // Initialize users widgets
}

void admin_users_show() {
    if (admin_window_id == 0) {
        return;
    }
    
    // Clear the content area
    window_fill_rect(admin_window_id, content_area_x, content_area_y, content_area_width, content_area_height, 0xFFFFFF);
    
    // Draw the users section
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 20, "User Management", 0x000000);
    
    // Draw user list
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 60, "Users:", 0x000000);
    
    window_fill_rect(admin_window_id, content_area_x + 20, content_area_y + 90, 560, 30, 0xE0E0E0);
    window_draw_text(admin_window_id, content_area_x + 30, content_area_y + 95, "Username", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 150, content_area_y + 95, "UID", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 200, content_area_y + 95, "GID", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 250, content_area_y + 95, "Home Directory", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 400, content_area_y + 95, "Shell", 0x000000);
    
    window_fill_rect(admin_window_id, content_area_x + 20, content_area_y + 120, 560, 30, 0xFFFFFF);
    window_draw_text(admin_window_id, content_area_x + 30, content_area_y + 125, "root", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 150, content_area_y + 125, "0", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 200, content_area_y + 125, "0", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 250, content_area_y + 125, "/root", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 400, content_area_y + 125, "/bin/bash", 0x000000);
    
    window_fill_rect(admin_window_id, content_area_x + 20, content_area_y + 150, 560, 30, 0xF8F8F8);
    window_draw_text(admin_window_id, content_area_x + 30, content_area_y + 155, "user", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 150, content_area_y + 155, "1000", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 200, content_area_y + 155, "1000", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 250, content_area_y + 155, "/home/user", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 400, content_area_y + 155, "/bin/bash", 0x000000);
    
    // Draw user management buttons
    window_create_button(admin_window_id, content_area_x + 20, content_area_y + 200, 100, 30, "Add User");
    window_create_button(admin_window_id, content_area_x + 130, content_area_y + 200, 100, 30, "Edit User");
    window_create_button(admin_window_id, content_area_x + 240, content_area_y + 200, 100, 30, "Delete User");
    
    // Draw group list
    window_draw_text(admin_window_id, content_area_x + 20, content_area_y + 250, "Groups:", 0x000000);
    
    window_fill_rect(admin_window_id, content_area_x + 20, content_area_y + 280, 560, 30, 0xE0E0E0);
    window_draw_text(admin_window_id, content_area_x + 30, content_area_y + 285, "Group Name", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 150, content_area_y + 285, "GID", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 200, content_area_y + 285, "Members", 0x000000);
    
    window_fill_rect(admin_window_id, content_area_x + 20, content_area_y + 310, 560, 30, 0xFFFFFF);
    window_draw_text(admin_window_id, content_area_x + 30, content_area_y + 315, "root", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 150, content_area_y + 315, "0", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 200, content_area_y + 315, "root", 0x000000);
    
    window_fill_rect(admin_window_id, content_area_x + 20, content_area_y + 340, 560, 30, 0xF8F8F8);
    window_draw_text(admin_window_id, content_area_x + 30, content_area_y + 345, "user", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 150, content_area_y + 345, "1000", 0x000000);
    window_draw_text(admin_window_id, content_area_x + 200, content_area_y + 345, "user", 0x000000);
    
    // Draw group management buttons
    window_create_button(admin_window_id, content_area_x + 20, content_area_y + 390, 100, 30, "Add Group");
    window_create_button(admin_window_id, content_area_x + 130, content_area_y + 390, 100, 30, "Edit Group");
    window_create_button(admin_window_id, content_area_x + 240, content_area_y + 390, 100, 30, "Delete Group");
}

void admin_users_hide() {
    // Nothing to do
}

void admin_users_refresh() {
    admin_users_show();
}

/**
 * LightOS GUI
 * System Tray header
 */

#ifndef SYSTEM_TRAY_H
#define SYSTEM_TRAY_H

// System tray icon structure
typedef struct {
    int id;
    char name[32];
    char tooltip[64];
    void* icon_data;
    int icon_width;
    int icon_height;
    int (*click_handler)(int button);
    void* user_data;
} system_tray_icon_t;

// System tray functions
void system_tray_init();
int system_tray_add_icon(const char* name, const char* tooltip, void* icon_data, int icon_width, int icon_height, int (*click_handler)(int button));
int system_tray_remove_icon(int icon_id);
int system_tray_update_icon(int icon_id, void* icon_data, int icon_width, int icon_height);
int system_tray_update_tooltip(int icon_id, const char* tooltip);
int system_tray_set_user_data(int icon_id, void* user_data);
void* system_tray_get_user_data(int icon_id);
int system_tray_show_notification(const char* title, const char* message, int timeout);
int system_tray_hide_notification();
int system_tray_is_visible();
int system_tray_show();
int system_tray_hide();

// Built-in system tray icons
void system_tray_add_clock();
void system_tray_add_network();
void system_tray_add_volume();
void system_tray_add_battery();
void system_tray_add_notifications();

// System tray menu functions
int system_tray_menu_show(int icon_id, int x, int y);
int system_tray_menu_hide();
int system_tray_menu_add_item(const char* text, int (*callback)(void));
int system_tray_menu_add_separator();
int system_tray_menu_clear();

#endif /* SYSTEM_TRAY_H */

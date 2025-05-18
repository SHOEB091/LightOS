/**
 * LightOS GUI
 * Admin Panel header
 */

#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

// Admin panel sections
typedef enum {
    ADMIN_SECTION_DASHBOARD,
    ADMIN_SECTION_USERS,
    ADMIN_SECTION_SYSTEM,
    ADMIN_SECTION_NETWORK,
    ADMIN_SECTION_STORAGE,
    ADMIN_SECTION_PACKAGES,
    ADMIN_SECTION_SERVICES,
    ADMIN_SECTION_SECURITY,
    ADMIN_SECTION_LOGS,
    ADMIN_SECTION_ABOUT
} admin_section_t;

// Admin panel callbacks
typedef void (*admin_callback_t)(void* data);

// Admin panel functions
void admin_panel_init();
int admin_panel_show();
int admin_panel_hide();
int admin_panel_is_visible();
int admin_panel_set_section(admin_section_t section);
admin_section_t admin_panel_get_section();
int admin_panel_refresh();

// Dashboard section functions
void admin_dashboard_init();
void admin_dashboard_show();
void admin_dashboard_hide();
void admin_dashboard_refresh();

// Users section functions
void admin_users_init();
void admin_users_show();
void admin_users_hide();
void admin_users_refresh();
int admin_users_add(const char* username, const char* password, unsigned int uid, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin);
int admin_users_remove(const char* username);
int admin_users_modify(const char* username, const char* password, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin);
int admin_users_add_to_group(const char* username, const char* group_name);
int admin_users_remove_from_group(const char* username, const char* group_name);

// System section functions
void admin_system_init();
void admin_system_show();
void admin_system_hide();
void admin_system_refresh();
int admin_system_shutdown();
int admin_system_reboot();
int admin_system_update();
int admin_system_backup(const char* backup_file);
int admin_system_restore(const char* backup_file);

// Network section functions
void admin_network_init();
void admin_network_show();
void admin_network_hide();
void admin_network_refresh();
int admin_network_configure_interface(const char* interface_name, const char* ip_address, const char* netmask, const char* gateway);
int admin_network_interface_up(const char* interface_name);
int admin_network_interface_down(const char* interface_name);
int admin_network_add_route(const char* destination, const char* gateway, const char* netmask, const char* interface_name);
int admin_network_remove_route(const char* destination);

// Storage section functions
void admin_storage_init();
void admin_storage_show();
void admin_storage_hide();
void admin_storage_refresh();
int admin_storage_mount(const char* device, const char* mount_point, const char* fs_type, const char* options);
int admin_storage_unmount(const char* mount_point);
int admin_storage_format(const char* device, const char* fs_type);
int admin_storage_check(const char* device);
int admin_storage_resize(const char* device, unsigned long long size);

// Packages section functions
void admin_packages_init();
void admin_packages_show();
void admin_packages_hide();
void admin_packages_refresh();
int admin_packages_install(const char* package_name, const char* version);
int admin_packages_uninstall(const char* package_name);
int admin_packages_update(const char* package_name);
int admin_packages_upgrade_all();
int admin_packages_add_repository(const char* name, const char* url, const char* description);
int admin_packages_remove_repository(const char* name);
int admin_packages_update_repositories();

// Services section functions
void admin_services_init();
void admin_services_show();
void admin_services_hide();
void admin_services_refresh();
int admin_services_start(const char* service_name);
int admin_services_stop(const char* service_name);
int admin_services_restart(const char* service_name);
int admin_services_enable(const char* service_name);
int admin_services_disable(const char* service_name);
int admin_services_status(const char* service_name);

// Security section functions
void admin_security_init();
void admin_security_show();
void admin_security_hide();
void admin_security_refresh();
int admin_security_set_file_permission(const char* path, unsigned int uid, unsigned int gid, unsigned int mode);
int admin_security_add_firewall_rule(const char* rule);
int admin_security_remove_firewall_rule(int rule_id);
int admin_security_enable_audit(const char* audit_rule);
int admin_security_disable_audit(const char* audit_rule);
int admin_security_check_integrity();

// Logs section functions
void admin_logs_init();
void admin_logs_show();
void admin_logs_hide();
void admin_logs_refresh();
int admin_logs_view(const char* log_file);
int admin_logs_filter(const char* log_file, const char* filter);
int admin_logs_clear(const char* log_file);
int admin_logs_export(const char* log_file, const char* export_file);

// About section functions
void admin_about_init();
void admin_about_show();
void admin_about_hide();
void admin_about_refresh();

#endif /* ADMIN_PANEL_H */

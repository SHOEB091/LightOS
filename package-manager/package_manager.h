/**
 * LightOS Package Manager
 * Package manager header
 */

#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

// Package dependency structure
typedef struct {
    char name[64];
    char version[32];
    int optional;
} package_dependency_t;

// Package structure
typedef struct {
    char name[64];
    char version[32];
    char description[256];
    char maintainer[64];
    char license[32];
    char url[128];
    char install_path[256];
    unsigned int installed;
    unsigned int size;
    package_dependency_t* dependencies;
    int dependency_count;
} package_t;

// Package repository structure
typedef struct {
    char name[64];
    char url[128];
    char description[256];
    unsigned int enabled;
} package_repository_t;

// Package manager functions
void package_manager_init();
int package_install(const char* name, const char* version);
int package_uninstall(const char* name);
int package_update(const char* name);
int package_upgrade_all();
package_t* package_get_info(const char* name);
int package_list_installed(package_t** packages, int* count);
int package_list_available(package_t** packages, int* count);
int package_list_upgradable(package_t** packages, int* count);
int package_search(const char* query, package_t** results, int* count);
int package_add_repository(const char* name, const char* url, const char* description);
int package_remove_repository(const char* name);
int package_enable_repository(const char* name);
int package_disable_repository(const char* name);
int package_list_repositories(package_repository_t** repositories, int* count);
int package_update_repositories();
int package_verify(const char* name);
int package_clean_cache();
int package_get_installed_size(unsigned long long* size);
int package_get_available_size(unsigned long long* size);
int package_get_cache_size(unsigned long long* size);

#endif /* PACKAGE_MANAGER_H */

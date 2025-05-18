/**
 * LightOS Package
 * Package Manager header
 */

#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

// Package dependency type
typedef enum {
    PACKAGE_DEPENDENCY_REQUIRED,
    PACKAGE_DEPENDENCY_RECOMMENDED,
    PACKAGE_DEPENDENCY_SUGGESTED,
    PACKAGE_DEPENDENCY_CONFLICTS,
    PACKAGE_DEPENDENCY_REPLACES,
    PACKAGE_DEPENDENCY_PROVIDES
} package_dependency_type_t;

// Package dependency structure
typedef struct {
    char name[64];
    char version[32];
    package_dependency_type_t type;
} package_dependency_t;

// Package structure
typedef struct {
    char name[64];
    char version[32];
    char description[256];
    char maintainer[128];
    char architecture[32];
    char license[64];
    char homepage[128];
    char repository[128];
    unsigned long long size;
    unsigned long long installed_size;
    char* files;
    unsigned int file_count;
    package_dependency_t* dependencies;
    unsigned int dependency_count;
    unsigned int installed;
    char install_date[32];
    void* private_data;
} package_t;

// Repository structure
typedef struct {
    char name[64];
    char url[256];
    char distribution[64];
    char component[64];
    char architecture[32];
    char* packages;
    unsigned int package_count;
    unsigned int enabled;
    char last_update[32];
    void* private_data;
} repository_t;

// Package manager functions
void package_manager_init();
int package_install(const char* name, const char* version);
int package_remove(const char* name);
int package_upgrade(const char* name);
int package_upgrade_all();
package_t* package_get(const char* name);
int package_list(package_t** packages, unsigned int* count);
int package_list_installed(package_t** packages, unsigned int* count);
int package_list_available(package_t** packages, unsigned int* count);
int package_list_upgradable(package_t** packages, unsigned int* count);
int package_search(const char* query, package_t** packages, unsigned int* count);
int package_show(const char* name, char* info, unsigned int info_size);
int package_get_files(const char* name, char*** files, unsigned int* count);
int package_verify(const char* name);
int package_clean();
int package_autoremove();
int package_add_repository(const char* name, const char* url, const char* distribution, const char* component, const char* architecture);
int package_remove_repository(const char* name);
int package_enable_repository(const char* name);
int package_disable_repository(const char* name);
repository_t* package_get_repository(const char* name);
int package_list_repositories(repository_t** repositories, unsigned int* count);
int package_update_repository(const char* name);
int package_update_all_repositories();
int package_download(const char* name, const char* version, const char* destination);
int package_extract(const char* package_file, const char* destination);
int package_build(const char* source_directory, const char* destination);
int package_check_dependency(const char* name, const char* dependency_name, const char* dependency_version);
int package_resolve_dependencies(const char* name, package_dependency_t** dependencies, unsigned int* count);
int package_add_dependency(const char* name, const char* dependency_name, const char* dependency_version, package_dependency_type_t type);
int package_remove_dependency(const char* name, const char* dependency_name);
int package_get_dependencies(const char* name, package_dependency_t** dependencies, unsigned int* count);
int package_get_reverse_dependencies(const char* name, package_dependency_t** dependencies, unsigned int* count);
int package_lock(const char* name);
int package_unlock(const char* name);
int package_is_locked(const char* name);
int package_hold(const char* name);
int package_unhold(const char* name);
int package_is_held(const char* name);
int package_set_priority(const char* name, unsigned int priority);
int package_get_priority(const char* name);
int package_set_auto_installed(const char* name, unsigned int auto_installed);
int package_is_auto_installed(const char* name);
int package_get_changelog(const char* name, char* changelog, unsigned int changelog_size);
int package_get_maintainer_scripts(const char* name, char* scripts, unsigned int scripts_size);
int package_run_maintainer_script(const char* name, const char* script);
int package_verify_signature(const char* package_file);
int package_sign(const char* package_file, const char* key_file);
int package_import_key(const char* key_file);
int package_export_key(const char* key_id, const char* key_file);
int package_list_keys(char*** key_ids, unsigned int* count);
int package_remove_key(const char* key_id);
int package_create_config(const char* config_file);
int package_parse_config(const char* config_file);
int package_set_config(const char* key, const char* value);
int package_get_config(const char* key, char* value, unsigned int value_size);
int package_set_install_root(const char* root);
int package_get_install_root(char* root, unsigned int root_size);
int package_set_cache_directory(const char* directory);
int package_get_cache_directory(char* directory, unsigned int directory_size);
int package_set_log_file(const char* file);
int package_get_log_file(char* file, unsigned int file_size);
int package_set_log_level(unsigned int level);
int package_get_log_level();
int package_set_download_timeout(unsigned int timeout);
int package_get_download_timeout();
int package_set_download_retries(unsigned int retries);
int package_get_download_retries();
int package_set_proxy(const char* proxy);
int package_get_proxy(char* proxy, unsigned int proxy_size);
int package_set_gpg_home(const char* directory);
int package_get_gpg_home(char* directory, unsigned int directory_size);
int package_set_architecture(const char* architecture);
int package_get_architecture(char* architecture, unsigned int architecture_size);
int package_set_distribution(const char* distribution);
int package_get_distribution(char* distribution, unsigned int distribution_size);
int package_set_component(const char* component);
int package_get_component(char* component, unsigned int component_size);
int package_set_mirror(const char* mirror);
int package_get_mirror(char* mirror, unsigned int mirror_size);
int package_set_user_agent(const char* user_agent);
int package_get_user_agent(char* user_agent, unsigned int user_agent_size);
int package_set_compression(const char* compression);
int package_get_compression(char* compression, unsigned int compression_size);
int package_set_verify_signatures(unsigned int verify);
int package_get_verify_signatures();
int package_set_allow_untrusted(unsigned int allow);
int package_get_allow_untrusted();
int package_set_auto_update(unsigned int auto_update);
int package_get_auto_update();
int package_set_auto_upgrade(unsigned int auto_upgrade);
int package_get_auto_upgrade();
int package_set_auto_clean(unsigned int auto_clean);
int package_get_auto_clean();
int package_set_auto_remove(unsigned int auto_remove);
int package_get_auto_remove();
int package_set_force_overwrite(unsigned int force);
int package_get_force_overwrite();
int package_set_force_downgrade(unsigned int force);
int package_get_force_downgrade();
int package_set_force_reinstall(unsigned int force);
int package_get_force_reinstall();
int package_set_force_remove(unsigned int force);
int package_get_force_remove();
int package_set_force_depends(unsigned int force);
int package_get_force_depends();
int package_set_force_conflicts(unsigned int force);
int package_get_force_conflicts();
int package_set_force_breaks(unsigned int force);
int package_get_force_breaks();
int package_set_force_architecture(unsigned int force);
int package_get_force_architecture();
int package_set_force_version(unsigned int force);
int package_get_force_version();
int package_set_force_all(unsigned int force);
int package_get_force_all();

#endif /* PACKAGE_MANAGER_H */

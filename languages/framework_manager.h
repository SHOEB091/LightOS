/**
 * LightOS Framework Manager
 * Manages programming language frameworks
 */

#ifndef FRAMEWORK_MANAGER_H
#define FRAMEWORK_MANAGER_H

#include "language_manager.h"

// Framework version structure (same as language version)
typedef LanguageVersion FrameworkVersion;

// Framework structure
typedef struct {
    const char* name;
    const char* description;
    LanguageType language_type;
    FrameworkVersion current_version;
    FrameworkVersion* available_versions;
    int available_version_count;
    int is_installed;
    
    // Function pointers for framework operations
    int (*initialize)(void);
    int (*install)(FrameworkVersion version);
    int (*uninstall)(void);
    int (*update)(FrameworkVersion version);
    int (*create_project)(const char* project_name, const char* options);
    int (*run_project)(const char* project_path);
    int (*build_project)(const char* project_path);
} Framework;

// Framework repository structure
typedef struct {
    Framework* frameworks;
    int framework_count;
    int framework_capacity;
} FrameworkRepository;

// Global framework repository
extern FrameworkRepository* framework_repository;

// Framework manager functions
void framework_manager_init();
Framework* framework_manager_get_framework(const char* name);
Framework* framework_manager_get_frameworks_for_language(LanguageType language_type);
int framework_manager_install_framework(const char* name, FrameworkVersion version);
int framework_manager_uninstall_framework(const char* name);
int framework_manager_update_framework(const char* name, FrameworkVersion version);
int framework_manager_create_project(const char* framework_name, const char* project_name, const char* options);
int framework_manager_run_project(const char* framework_name, const char* project_path);
int framework_manager_build_project(const char* framework_name, const char* project_path);
void framework_manager_print_frameworks();
void framework_manager_print_frameworks_for_language(LanguageType language_type);

// Framework version functions (reuse language version functions)
#define framework_version_create language_version_create
#define framework_version_compare language_version_compare
#define framework_version_to_string language_version_to_string

// Framework registration
int framework_manager_register_framework(Framework framework);

// Java frameworks
int spring_framework_init();
int spring_boot_framework_init();

// JavaScript frameworks
int react_framework_init();
int angular_framework_init();
int vue_framework_init();
int node_express_framework_init();

// Python frameworks
int django_framework_init();
int flask_framework_init();
int fastapi_framework_init();

// C# frameworks
int aspnet_framework_init();
int dotnet_maui_framework_init();

// Go frameworks
int gin_framework_init();
int echo_framework_init();

#endif /* FRAMEWORK_MANAGER_H */

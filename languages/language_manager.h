/**
 * LightOS Language Manager
 * Manages built-in programming languages
 */

#ifndef LANGUAGE_MANAGER_H
#define LANGUAGE_MANAGER_H

#include "../kernel/kernel.h"
#include "../kernel/filesystem.h"

// Language types
typedef enum {
    LANG_C,
    LANG_CPP,
    LANG_PYTHON,
    LANG_JAVASCRIPT,
    LANG_GOLANG,
    LANG_DOTNET,
    LANG_JAVA,
    LANG_COUNT
} LanguageType;

// Language version structure
typedef struct {
    int major;
    int minor;
    int patch;
    char* build_string;
} LanguageVersion;

// Language structure
typedef struct {
    LanguageType type;
    const char* name;
    const char* file_extension;
    LanguageVersion current_version;
    LanguageVersion* available_versions;
    int available_version_count;
    int is_installed;
    int is_active;

    // Function pointers for language operations
    int (*initialize)(void);
    int (*compile)(const char* source_file, const char* output_file);
    int (*run)(const char* file);
    int (*run_repl)(void);
    int (*update)(LanguageVersion version);
} Language;

// Language manager structure
typedef struct {
    Language languages[LANG_COUNT];
    LanguageType default_language;
} LanguageManager;

// Global language manager
extern LanguageManager* language_manager;

// Language manager functions
void language_manager_init();
Language* language_manager_get_language(LanguageType type);
Language* language_manager_get_language_by_name(const char* name);
Language* language_manager_get_language_by_extension(const char* extension);
int language_manager_install_language(LanguageType type);
int language_manager_uninstall_language(LanguageType type);
int language_manager_update_language(LanguageType type, LanguageVersion version);
int language_manager_set_default_language(LanguageType type);
void language_manager_print_languages();

// Language version functions
LanguageVersion language_version_create(int major, int minor, int patch, const char* build_string);
int language_version_compare(LanguageVersion a, LanguageVersion b);
char* language_version_to_string(LanguageVersion version);

// Language-specific initialization functions
int c_language_init();
int cpp_language_init();
int python_language_init();
int javascript_language_init();
int golang_language_init();
int dotnet_language_init();
int java_language_init();

#endif /* LANGUAGE_MANAGER_H */

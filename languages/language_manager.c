/**
 * LightOS Language Manager
 * Implementation of the language manager
 */

#include "language_manager.h"
#include "../libc/string.h"
#include "../kernel/memory.h"
#include "java/java_language.h"

// Global language manager instance
LanguageManager* language_manager = NULL;

// Initialize the language manager
void language_manager_init() {
    // Allocate memory for the language manager
    language_manager = (LanguageManager*)allocate_block();

    if (!language_manager) {
        terminal_write("Failed to allocate memory for language manager\n");
        return;
    }

    // Set default language
    language_manager->default_language = LANG_C;

    // Initialize language structures

    // C Language
    language_manager->languages[LANG_C].type = LANG_C;
    language_manager->languages[LANG_C].name = "C";
    language_manager->languages[LANG_C].file_extension = ".c";
    language_manager->languages[LANG_C].current_version = language_version_create(11, 0, 0, "C11");
    language_manager->languages[LANG_C].available_versions = NULL;
    language_manager->languages[LANG_C].available_version_count = 0;
    language_manager->languages[LANG_C].is_installed = 1; // C is always installed
    language_manager->languages[LANG_C].is_active = 1;
    language_manager->languages[LANG_C].initialize = c_language_init;
    language_manager->languages[LANG_C].compile = NULL; // To be implemented
    language_manager->languages[LANG_C].run = NULL; // To be implemented
    language_manager->languages[LANG_C].run_repl = NULL; // To be implemented
    language_manager->languages[LANG_C].update = NULL; // To be implemented

    // C++ Language
    language_manager->languages[LANG_CPP].type = LANG_CPP;
    language_manager->languages[LANG_CPP].name = "C++";
    language_manager->languages[LANG_CPP].file_extension = ".cpp";
    language_manager->languages[LANG_CPP].current_version = language_version_create(17, 0, 0, "C++17");
    language_manager->languages[LANG_CPP].available_versions = NULL;
    language_manager->languages[LANG_CPP].available_version_count = 0;
    language_manager->languages[LANG_CPP].is_installed = 1; // C++ is always installed
    language_manager->languages[LANG_CPP].is_active = 1;
    language_manager->languages[LANG_CPP].initialize = cpp_language_init;
    language_manager->languages[LANG_CPP].compile = NULL; // To be implemented
    language_manager->languages[LANG_CPP].run = NULL; // To be implemented
    language_manager->languages[LANG_CPP].run_repl = NULL; // To be implemented
    language_manager->languages[LANG_CPP].update = NULL; // To be implemented

    // Python Language
    language_manager->languages[LANG_PYTHON].type = LANG_PYTHON;
    language_manager->languages[LANG_PYTHON].name = "Python";
    language_manager->languages[LANG_PYTHON].file_extension = ".py";
    language_manager->languages[LANG_PYTHON].current_version = language_version_create(3, 9, 0, "Python 3.9");
    language_manager->languages[LANG_PYTHON].available_versions = NULL;
    language_manager->languages[LANG_PYTHON].available_version_count = 0;
    language_manager->languages[LANG_PYTHON].is_installed = 1; // Python is installed by default
    language_manager->languages[LANG_PYTHON].is_active = 1;
    language_manager->languages[LANG_PYTHON].initialize = python_language_init;
    language_manager->languages[LANG_PYTHON].compile = NULL; // Python is interpreted
    language_manager->languages[LANG_PYTHON].run = NULL; // To be implemented
    language_manager->languages[LANG_PYTHON].run_repl = NULL; // To be implemented
    language_manager->languages[LANG_PYTHON].update = NULL; // To be implemented

    // JavaScript (Node.js) Language
    language_manager->languages[LANG_JAVASCRIPT].type = LANG_JAVASCRIPT;
    language_manager->languages[LANG_JAVASCRIPT].name = "JavaScript";
    language_manager->languages[LANG_JAVASCRIPT].file_extension = ".js";
    language_manager->languages[LANG_JAVASCRIPT].current_version = language_version_create(14, 15, 0, "Node.js 14.15.0");
    language_manager->languages[LANG_JAVASCRIPT].available_versions = NULL;
    language_manager->languages[LANG_JAVASCRIPT].available_version_count = 0;
    language_manager->languages[LANG_JAVASCRIPT].is_installed = 1; // JavaScript is installed by default
    language_manager->languages[LANG_JAVASCRIPT].is_active = 1;
    language_manager->languages[LANG_JAVASCRIPT].initialize = javascript_language_init;
    language_manager->languages[LANG_JAVASCRIPT].compile = NULL; // JavaScript is interpreted
    language_manager->languages[LANG_JAVASCRIPT].run = NULL; // To be implemented
    language_manager->languages[LANG_JAVASCRIPT].run_repl = NULL; // To be implemented
    language_manager->languages[LANG_JAVASCRIPT].update = NULL; // To be implemented

    // Go Language
    language_manager->languages[LANG_GOLANG].type = LANG_GOLANG;
    language_manager->languages[LANG_GOLANG].name = "Go";
    language_manager->languages[LANG_GOLANG].file_extension = ".go";
    language_manager->languages[LANG_GOLANG].current_version = language_version_create(1, 15, 0, "Go 1.15");
    language_manager->languages[LANG_GOLANG].available_versions = NULL;
    language_manager->languages[LANG_GOLANG].available_version_count = 0;
    language_manager->languages[LANG_GOLANG].is_installed = 1; // Go is installed by default
    language_manager->languages[LANG_GOLANG].is_active = 1;
    language_manager->languages[LANG_GOLANG].initialize = golang_language_init;
    language_manager->languages[LANG_GOLANG].compile = NULL; // To be implemented
    language_manager->languages[LANG_GOLANG].run = NULL; // To be implemented
    language_manager->languages[LANG_GOLANG].run_repl = NULL; // To be implemented
    language_manager->languages[LANG_GOLANG].update = NULL; // To be implemented

    // C# (.NET) Language
    language_manager->languages[LANG_DOTNET].type = LANG_DOTNET;
    language_manager->languages[LANG_DOTNET].name = "C#";
    language_manager->languages[LANG_DOTNET].file_extension = ".cs";
    language_manager->languages[LANG_DOTNET].current_version = language_version_create(9, 0, 0, ".NET 5.0");
    language_manager->languages[LANG_DOTNET].available_versions = NULL;
    language_manager->languages[LANG_DOTNET].available_version_count = 0;
    language_manager->languages[LANG_DOTNET].is_installed = 1; // C# is installed by default
    language_manager->languages[LANG_DOTNET].is_active = 1;
    language_manager->languages[LANG_DOTNET].initialize = dotnet_language_init;
    language_manager->languages[LANG_DOTNET].compile = NULL; // To be implemented
    language_manager->languages[LANG_DOTNET].run = NULL; // To be implemented
    language_manager->languages[LANG_DOTNET].run_repl = NULL; // To be implemented
    language_manager->languages[LANG_DOTNET].update = NULL; // To be implemented

    // Java Language
    language_manager->languages[LANG_JAVA].type = LANG_JAVA;
    language_manager->languages[LANG_JAVA].name = "Java";
    language_manager->languages[LANG_JAVA].file_extension = ".java";
    language_manager->languages[LANG_JAVA].current_version = language_version_create(11, 0, 2, "Java 11.0.2");
    language_manager->languages[LANG_JAVA].available_versions = NULL;
    language_manager->languages[LANG_JAVA].available_version_count = 0;
    language_manager->languages[LANG_JAVA].is_installed = 1; // Java is installed by default
    language_manager->languages[LANG_JAVA].is_active = 1;
    language_manager->languages[LANG_JAVA].initialize = java_language_init;
    language_manager->languages[LANG_JAVA].compile = java_language_compile;
    language_manager->languages[LANG_JAVA].run = java_language_run;
    language_manager->languages[LANG_JAVA].run_repl = java_language_run_repl;
    language_manager->languages[LANG_JAVA].update = java_language_update;

    // Initialize each language
    for (int i = 0; i < LANG_COUNT; i++) {
        if (language_manager->languages[i].is_installed &&
            language_manager->languages[i].initialize) {
            language_manager->languages[i].initialize();
        }
    }

    terminal_write("Language manager initialized\n");
}

// Get a language by type
Language* language_manager_get_language(LanguageType type) {
    if (type < 0 || type >= LANG_COUNT) {
        return NULL;
    }

    return &language_manager->languages[type];
}

// Get a language by name
Language* language_manager_get_language_by_name(const char* name) {
    for (int i = 0; i < LANG_COUNT; i++) {
        if (strcmp(language_manager->languages[i].name, name) == 0) {
            return &language_manager->languages[i];
        }
    }

    return NULL;
}

// Get a language by file extension
Language* language_manager_get_language_by_extension(const char* extension) {
    for (int i = 0; i < LANG_COUNT; i++) {
        if (strcmp(language_manager->languages[i].file_extension, extension) == 0) {
            return &language_manager->languages[i];
        }
    }

    return NULL;
}

// Install a language
int language_manager_install_language(LanguageType type) {
    if (type < 0 || type >= LANG_COUNT) {
        return -1;
    }

    Language* language = &language_manager->languages[type];

    if (language->is_installed) {
        return 0; // Already installed
    }

    // In a real system, we would download and install the language here

    language->is_installed = 1;

    if (language->initialize) {
        language->initialize();
    }

    return 0;
}

// Uninstall a language
int language_manager_uninstall_language(LanguageType type) {
    if (type < 0 || type >= LANG_COUNT) {
        return -1;
    }

    Language* language = &language_manager->languages[type];

    if (!language->is_installed) {
        return 0; // Already uninstalled
    }

    // In a real system, we would uninstall the language here

    language->is_installed = 0;
    language->is_active = 0;

    return 0;
}

// Update a language to a specific version
int language_manager_update_language(LanguageType type, LanguageVersion version) {
    if (type < 0 || type >= LANG_COUNT) {
        return -1;
    }

    Language* language = &language_manager->languages[type];

    if (!language->is_installed) {
        return -1; // Not installed
    }

    if (language->update) {
        return language->update(version);
    }

    return -1; // Update not supported
}

// Set the default language
int language_manager_set_default_language(LanguageType type) {
    if (type < 0 || type >= LANG_COUNT) {
        return -1;
    }

    if (!language_manager->languages[type].is_installed) {
        return -1; // Not installed
    }

    language_manager->default_language = type;

    return 0;
}

// Print all available languages
void language_manager_print_languages() {
    terminal_write("Available Languages:\n");
    terminal_write("-------------------\n");

    for (int i = 0; i < LANG_COUNT; i++) {
        Language* language = &language_manager->languages[i];

        terminal_write(language->name);
        terminal_write(" (");

        char version_str[32];
        sprintf(version_str, "%d.%d.%d",
                language->current_version.major,
                language->current_version.minor,
                language->current_version.patch);

        terminal_write(version_str);
        terminal_write(") - ");

        if (language->is_installed) {
            if (language->is_active) {
                terminal_write_color("ACTIVE", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            } else {
                terminal_write_color("INSTALLED", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
            }
        } else {
            terminal_write_color("NOT INSTALLED", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        }

        if (i == language_manager->default_language) {
            terminal_write(" (DEFAULT)");
        }

        terminal_write("\n");
    }
}

// Create a language version
LanguageVersion language_version_create(int major, int minor, int patch, const char* build_string) {
    LanguageVersion version;
    version.major = major;
    version.minor = minor;
    version.patch = patch;
    version.build_string = (char*)build_string;
    return version;
}

// Compare two language versions
int language_version_compare(LanguageVersion a, LanguageVersion b) {
    if (a.major != b.major) {
        return a.major - b.major;
    }

    if (a.minor != b.minor) {
        return a.minor - b.minor;
    }

    return a.patch - b.patch;
}

// Convert a language version to a string
char* language_version_to_string(LanguageVersion version) {
    // In a real system, we would allocate memory for the string
    static char version_str[64];

    sprintf(version_str, "%d.%d.%d (%s)",
            version.major, version.minor, version.patch,
            version.build_string ? version.build_string : "");

    return version_str;
}

// Language-specific initialization functions
// These would be implemented in separate files in a real system

int c_language_init() {
    terminal_write("Initializing C language support\n");
    return 0;
}

int cpp_language_init() {
    terminal_write("Initializing C++ language support\n");
    return 0;
}

int python_language_init() {
    terminal_write("Initializing Python language support\n");
    return 0;
}

int javascript_language_init() {
    terminal_write("Initializing JavaScript (Node.js) language support\n");
    return 0;
}

int golang_language_init() {
    terminal_write("Initializing Go language support\n");
    return 0;
}

int dotnet_language_init() {
    terminal_write("Initializing C# (.NET) language support\n");
    return 0;
}

int java_language_init() {
    terminal_write("Initializing Java language support\n");
    return 0;
}

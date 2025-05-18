/**
 * LightOS Framework Manager
 * Implementation of the framework manager
 */

#include "framework_manager.h"
#include "../libc/string.h"
#include "../kernel/memory.h"
#include "../kernel/filesystem.h"
#include "../kernel/process.h"

// Initial capacity for the framework repository
#define INITIAL_FRAMEWORK_CAPACITY 32

// Global framework repository instance
FrameworkRepository* framework_repository = NULL;

// Initialize the framework manager
void framework_manager_init() {
    // Allocate memory for the framework repository
    framework_repository = (FrameworkRepository*)allocate_block();
    
    if (!framework_repository) {
        terminal_write("Failed to allocate memory for framework repository\n");
        return;
    }
    
    // Initialize the repository
    framework_repository->framework_count = 0;
    framework_repository->framework_capacity = INITIAL_FRAMEWORK_CAPACITY;
    
    // Allocate memory for the frameworks array
    framework_repository->frameworks = (Framework*)allocate_blocks(
        (INITIAL_FRAMEWORK_CAPACITY * sizeof(Framework)) / MEMORY_BLOCK_SIZE + 1);
    
    if (!framework_repository->frameworks) {
        terminal_write("Failed to allocate memory for frameworks array\n");
        free_block(framework_repository);
        framework_repository = NULL;
        return;
    }
    
    // Register built-in frameworks
    
    // Java frameworks
    spring_framework_init();
    spring_boot_framework_init();
    
    // JavaScript frameworks
    react_framework_init();
    angular_framework_init();
    vue_framework_init();
    node_express_framework_init();
    
    // Python frameworks
    django_framework_init();
    flask_framework_init();
    fastapi_framework_init();
    
    // C# frameworks
    aspnet_framework_init();
    dotnet_maui_framework_init();
    
    // Go frameworks
    gin_framework_init();
    echo_framework_init();
    
    terminal_write("Framework manager initialized\n");
}

// Get a framework by name
Framework* framework_manager_get_framework(const char* name) {
    for (int i = 0; i < framework_repository->framework_count; i++) {
        if (strcmp(framework_repository->frameworks[i].name, name) == 0) {
            return &framework_repository->frameworks[i];
        }
    }
    
    return NULL;
}

// Get frameworks for a specific language
Framework* framework_manager_get_frameworks_for_language(LanguageType language_type) {
    // Count frameworks for the language
    int count = 0;
    for (int i = 0; i < framework_repository->framework_count; i++) {
        if (framework_repository->frameworks[i].language_type == language_type) {
            count++;
        }
    }
    
    if (count == 0) {
        return NULL;
    }
    
    // Allocate memory for the result
    Framework* result = (Framework*)allocate_blocks(
        (count * sizeof(Framework)) / MEMORY_BLOCK_SIZE + 1);
    
    if (!result) {
        return NULL;
    }
    
    // Copy frameworks for the language
    int index = 0;
    for (int i = 0; i < framework_repository->framework_count; i++) {
        if (framework_repository->frameworks[i].language_type == language_type) {
            result[index++] = framework_repository->frameworks[i];
        }
    }
    
    return result;
}

// Install a framework
int framework_manager_install_framework(const char* name, FrameworkVersion version) {
    Framework* framework = framework_manager_get_framework(name);
    
    if (!framework) {
        terminal_write("Framework not found: ");
        terminal_write(name);
        terminal_write("\n");
        return -1;
    }
    
    if (framework->is_installed) {
        terminal_write("Framework already installed: ");
        terminal_write(name);
        terminal_write("\n");
        return 0;
    }
    
    // Check if the language is installed
    Language* language = language_manager_get_language(framework->language_type);
    
    if (!language || !language->is_installed) {
        terminal_write("Language not installed: ");
        terminal_write(language ? language->name : "Unknown");
        terminal_write("\n");
        terminal_write("Please install the language first.\n");
        return -1;
    }
    
    // Install the framework
    if (framework->install) {
        int result = framework->install(version);
        
        if (result == 0) {
            framework->is_installed = 1;
            framework->current_version = version;
            
            terminal_write("Framework installed: ");
            terminal_write(name);
            terminal_write(" (");
            terminal_write(framework_version_to_string(version));
            terminal_write(")\n");
        }
        
        return result;
    }
    
    return -1;
}

// Uninstall a framework
int framework_manager_uninstall_framework(const char* name) {
    Framework* framework = framework_manager_get_framework(name);
    
    if (!framework) {
        terminal_write("Framework not found: ");
        terminal_write(name);
        terminal_write("\n");
        return -1;
    }
    
    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(name);
        terminal_write("\n");
        return 0;
    }
    
    // Uninstall the framework
    if (framework->uninstall) {
        int result = framework->uninstall();
        
        if (result == 0) {
            framework->is_installed = 0;
            
            terminal_write("Framework uninstalled: ");
            terminal_write(name);
            terminal_write("\n");
        }
        
        return result;
    }
    
    return -1;
}

// Update a framework
int framework_manager_update_framework(const char* name, FrameworkVersion version) {
    Framework* framework = framework_manager_get_framework(name);
    
    if (!framework) {
        terminal_write("Framework not found: ");
        terminal_write(name);
        terminal_write("\n");
        return -1;
    }
    
    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(name);
        terminal_write("\n");
        return -1;
    }
    
    // Update the framework
    if (framework->update) {
        int result = framework->update(version);
        
        if (result == 0) {
            framework->current_version = version;
            
            terminal_write("Framework updated: ");
            terminal_write(name);
            terminal_write(" (");
            terminal_write(framework_version_to_string(version));
            terminal_write(")\n");
        }
        
        return result;
    }
    
    return -1;
}

// Create a project using a framework
int framework_manager_create_project(const char* framework_name, const char* project_name, const char* options) {
    Framework* framework = framework_manager_get_framework(framework_name);
    
    if (!framework) {
        terminal_write("Framework not found: ");
        terminal_write(framework_name);
        terminal_write("\n");
        return -1;
    }
    
    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework_name);
        terminal_write("\n");
        return -1;
    }
    
    // Create the project
    if (framework->create_project) {
        terminal_write("Creating project: ");
        terminal_write(project_name);
        terminal_write(" using ");
        terminal_write(framework_name);
        terminal_write("\n");
        
        return framework->create_project(project_name, options);
    }
    
    return -1;
}

// Run a project
int framework_manager_run_project(const char* framework_name, const char* project_path) {
    Framework* framework = framework_manager_get_framework(framework_name);
    
    if (!framework) {
        terminal_write("Framework not found: ");
        terminal_write(framework_name);
        terminal_write("\n");
        return -1;
    }
    
    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework_name);
        terminal_write("\n");
        return -1;
    }
    
    // Run the project
    if (framework->run_project) {
        terminal_write("Running project: ");
        terminal_write(project_path);
        terminal_write(" using ");
        terminal_write(framework_name);
        terminal_write("\n");
        
        return framework->run_project(project_path);
    }
    
    return -1;
}

// Build a project
int framework_manager_build_project(const char* framework_name, const char* project_path) {
    Framework* framework = framework_manager_get_framework(framework_name);
    
    if (!framework) {
        terminal_write("Framework not found: ");
        terminal_write(framework_name);
        terminal_write("\n");
        return -1;
    }
    
    if (!framework->is_installed) {
        terminal_write("Framework not installed: ");
        terminal_write(framework_name);
        terminal_write("\n");
        return -1;
    }
    
    // Build the project
    if (framework->build_project) {
        terminal_write("Building project: ");
        terminal_write(project_path);
        terminal_write(" using ");
        terminal_write(framework_name);
        terminal_write("\n");
        
        return framework->build_project(project_path);
    }
    
    return -1;
}

// Print all available frameworks
void framework_manager_print_frameworks() {
    terminal_write("Available Frameworks:\n");
    terminal_write("--------------------\n");
    
    for (int i = 0; i < framework_repository->framework_count; i++) {
        Framework* framework = &framework_repository->frameworks[i];
        Language* language = language_manager_get_language(framework->language_type);
        
        terminal_write(framework->name);
        terminal_write(" (");
        terminal_write(framework_version_to_string(framework->current_version));
        terminal_write(") - ");
        
        if (framework->is_installed) {
            terminal_write_color("INSTALLED", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
        } else {
            terminal_write_color("NOT INSTALLED", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        }
        
        terminal_write(" - For ");
        terminal_write(language ? language->name : "Unknown");
        terminal_write("\n");
        
        terminal_write("  ");
        terminal_write(framework->description);
        terminal_write("\n");
    }
}

// Print frameworks for a specific language
void framework_manager_print_frameworks_for_language(LanguageType language_type) {
    Language* language = language_manager_get_language(language_type);
    
    if (!language) {
        terminal_write("Invalid language type\n");
        return;
    }
    
    terminal_write("Frameworks for ");
    terminal_write(language->name);
    terminal_write(":\n");
    terminal_write("--------------------\n");
    
    int found = 0;
    
    for (int i = 0; i < framework_repository->framework_count; i++) {
        Framework* framework = &framework_repository->frameworks[i];
        
        if (framework->language_type == language_type) {
            found = 1;
            
            terminal_write(framework->name);
            terminal_write(" (");
            terminal_write(framework_version_to_string(framework->current_version));
            terminal_write(") - ");
            
            if (framework->is_installed) {
                terminal_write_color("INSTALLED", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            } else {
                terminal_write_color("NOT INSTALLED", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            }
            
            terminal_write("\n");
            
            terminal_write("  ");
            terminal_write(framework->description);
            terminal_write("\n");
        }
    }
    
    if (!found) {
        terminal_write("No frameworks available for ");
        terminal_write(language->name);
        terminal_write("\n");
    }
}

// Register a framework
int framework_manager_register_framework(Framework framework) {
    // Check if we need to resize the frameworks array
    if (framework_repository->framework_count >= framework_repository->framework_capacity) {
        // Double the capacity
        int new_capacity = framework_repository->framework_capacity * 2;
        
        // Allocate a new array
        Framework* new_frameworks = (Framework*)allocate_blocks(
            (new_capacity * sizeof(Framework)) / MEMORY_BLOCK_SIZE + 1);
        
        if (!new_frameworks) {
            terminal_write("Failed to resize frameworks array\n");
            return -1;
        }
        
        // Copy existing frameworks
        for (int i = 0; i < framework_repository->framework_count; i++) {
            new_frameworks[i] = framework_repository->frameworks[i];
        }
        
        // Free the old array
        free_blocks(framework_repository->frameworks, 
            (framework_repository->framework_capacity * sizeof(Framework)) / MEMORY_BLOCK_SIZE + 1);
        
        // Update the repository
        framework_repository->frameworks = new_frameworks;
        framework_repository->framework_capacity = new_capacity;
    }
    
    // Add the framework
    framework_repository->frameworks[framework_repository->framework_count++] = framework;
    
    return 0;
}

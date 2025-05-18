/**
 * LightOS Package
 * Package Manager implementation
 */

#include "package_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "../kernel/filesystem.h"
#include "../networking/network.h"
#include "../security/crypto/crypto.h"

// Maximum number of packages
#define MAX_PACKAGES 1000

// Maximum number of repositories
#define MAX_REPOSITORIES 100

// Package array
static package_t* packages[MAX_PACKAGES];
static unsigned int package_count = 0;

// Repository array
static repository_t* repositories[MAX_REPOSITORIES];
static unsigned int repository_count = 0;

// Configuration
static char install_root[256] = "/";
static char cache_directory[256] = "/var/cache/packages";
static char log_file[256] = "/var/log/package.log";
static unsigned int log_level = 1;
static unsigned int download_timeout = 60;
static unsigned int download_retries = 3;
static char proxy[256] = "";
static char gpg_home[256] = "/etc/package/gpg";
static char architecture[32] = "x86_64";
static char distribution[64] = "stable";
static char component[64] = "main";
static char mirror[256] = "http://packages.lightos.org";
static char user_agent[128] = "LightOS Package Manager/1.0";
static char compression[32] = "gzip";
static unsigned int verify_signatures = 1;
static unsigned int allow_untrusted = 0;
static unsigned int auto_update = 1;
static unsigned int auto_upgrade = 0;
static unsigned int auto_clean = 1;
static unsigned int auto_remove = 0;
static unsigned int force_overwrite = 0;
static unsigned int force_downgrade = 0;
static unsigned int force_reinstall = 0;
static unsigned int force_remove = 0;
static unsigned int force_depends = 0;
static unsigned int force_conflicts = 0;
static unsigned int force_breaks = 0;
static unsigned int force_architecture = 0;
static unsigned int force_version = 0;

// Initialize the package manager
void package_manager_init() {
    terminal_write("Initializing package manager...\n");
    
    // Clear the package array
    for (int i = 0; i < MAX_PACKAGES; i++) {
        packages[i] = NULL;
    }
    
    package_count = 0;
    
    // Clear the repository array
    for (int i = 0; i < MAX_REPOSITORIES; i++) {
        repositories[i] = NULL;
    }
    
    repository_count = 0;
    
    // Create the default repository
    package_add_repository("main", mirror, distribution, component, architecture);
    
    // Create the cache directory
    filesystem_create_directory(cache_directory);
    
    // Create the GPG home directory
    filesystem_create_directory(gpg_home);
    
    // Parse the configuration file
    package_parse_config("/etc/package/package.conf");
    
    // Update the repositories if auto-update is enabled
    if (auto_update) {
        package_update_all_repositories();
    }
    
    terminal_write("Package manager initialized\n");
}

// Install a package
int package_install(const char* name, const char* version) {
    if (!name) {
        terminal_write("Error: Package name cannot be NULL\n");
        return -1;
    }
    
    terminal_write("Installing package '");
    terminal_write(name);
    
    if (version) {
        terminal_write("' version '");
        terminal_write(version);
    }
    
    terminal_write("'...\n");
    
    // Check if the package is already installed
    package_t* pkg = package_get(name);
    
    if (pkg && pkg->installed) {
        if (!version || strcmp(pkg->version, version) == 0) {
            terminal_write("Package '");
            terminal_write(name);
            terminal_write("' is already installed\n");
            return 0;
        } else if (!force_reinstall) {
            terminal_write("Error: Package '");
            terminal_write(name);
            terminal_write("' is already installed with version '");
            terminal_write(pkg->version);
            terminal_write("'\n");
            terminal_write("Use --force-reinstall to reinstall\n");
            return -1;
        }
    }
    
    // Find the package in the available packages
    if (!pkg) {
        // Search for the package in the repositories
        for (unsigned int i = 0; i < repository_count; i++) {
            if (!repositories[i]->enabled) {
                continue;
            }
            
            // In a real system, we would search the repository's package list
            // For now, just simulate not finding the package
        }
        
        terminal_write("Error: Package '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the requested version is available
    if (version && strcmp(pkg->version, version) != 0) {
        terminal_write("Error: Package '");
        terminal_write(name);
        terminal_write("' version '");
        terminal_write(version);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Resolve dependencies
    package_dependency_t dependencies[100];
    unsigned int dependency_count = 0;
    
    if (package_resolve_dependencies(name, dependencies, &dependency_count) != 0) {
        terminal_write("Error: Failed to resolve dependencies for package '");
        terminal_write(name);
        terminal_write("'\n");
        return -1;
    }
    
    // Install dependencies
    for (unsigned int i = 0; i < dependency_count; i++) {
        if (dependencies[i].type == PACKAGE_DEPENDENCY_REQUIRED) {
            if (package_install(dependencies[i].name, dependencies[i].version) != 0) {
                terminal_write("Error: Failed to install dependency '");
                terminal_write(dependencies[i].name);
                terminal_write("'\n");
                return -1;
            }
        }
    }
    
    // Download the package
    char package_file[256];
    sprintf(package_file, "%s/%s_%s.pkg", cache_directory, name, pkg->version);
    
    if (package_download(name, pkg->version, package_file) != 0) {
        terminal_write("Error: Failed to download package '");
        terminal_write(name);
        terminal_write("'\n");
        return -1;
    }
    
    // Verify the package signature
    if (verify_signatures && !allow_untrusted) {
        if (package_verify_signature(package_file) != 0) {
            terminal_write("Error: Failed to verify signature for package '");
            terminal_write(name);
            terminal_write("'\n");
            return -1;
        }
    }
    
    // Extract the package
    if (package_extract(package_file, install_root) != 0) {
        terminal_write("Error: Failed to extract package '");
        terminal_write(name);
        terminal_write("'\n");
        return -1;
    }
    
    // Run the post-install script
    if (package_run_maintainer_script(name, "postinst") != 0) {
        terminal_write("Warning: Failed to run post-install script for package '");
        terminal_write(name);
        terminal_write("'\n");
    }
    
    // Mark the package as installed
    pkg->installed = 1;
    
    // Set the install date
    char date[32];
    // In a real system, we would get the current date and time
    strcpy(date, "2023-01-01 00:00:00");
    strcpy(pkg->install_date, date);
    
    terminal_write("Package '");
    terminal_write(name);
    terminal_write("' installed successfully\n");
    
    return 0;
}

// Remove a package
int package_remove(const char* name) {
    if (!name) {
        terminal_write("Error: Package name cannot be NULL\n");
        return -1;
    }
    
    terminal_write("Removing package '");
    terminal_write(name);
    terminal_write("'...\n");
    
    // Check if the package is installed
    package_t* pkg = package_get(name);
    
    if (!pkg || !pkg->installed) {
        terminal_write("Error: Package '");
        terminal_write(name);
        terminal_write("' is not installed\n");
        return -1;
    }
    
    // Check for reverse dependencies
    package_dependency_t reverse_dependencies[100];
    unsigned int reverse_dependency_count = 0;
    
    if (package_get_reverse_dependencies(name, reverse_dependencies, &reverse_dependency_count) != 0) {
        terminal_write("Error: Failed to get reverse dependencies for package '");
        terminal_write(name);
        terminal_write("'\n");
        return -1;
    }
    
    // Check if any reverse dependencies are required
    for (unsigned int i = 0; i < reverse_dependency_count; i++) {
        if (reverse_dependencies[i].type == PACKAGE_DEPENDENCY_REQUIRED && !force_depends) {
            terminal_write("Error: Package '");
            terminal_write(reverse_dependencies[i].name);
            terminal_write("' depends on '");
            terminal_write(name);
            terminal_write("'\n");
            terminal_write("Use --force-depends to remove anyway\n");
            return -1;
        }
    }
    
    // Run the pre-remove script
    if (package_run_maintainer_script(name, "prerm") != 0) {
        terminal_write("Warning: Failed to run pre-remove script for package '");
        terminal_write(name);
        terminal_write("'\n");
    }
    
    // Remove the package files
    for (unsigned int i = 0; i < pkg->file_count; i++) {
        char file_path[256];
        sprintf(file_path, "%s%s", install_root, pkg->files[i]);
        
        if (filesystem_remove_file(file_path) != 0) {
            terminal_write("Warning: Failed to remove file '");
            terminal_write(file_path);
            terminal_write("'\n");
        }
    }
    
    // Run the post-remove script
    if (package_run_maintainer_script(name, "postrm") != 0) {
        terminal_write("Warning: Failed to run post-remove script for package '");
        terminal_write(name);
        terminal_write("'\n");
    }
    
    // Mark the package as not installed
    pkg->installed = 0;
    
    terminal_write("Package '");
    terminal_write(name);
    terminal_write("' removed successfully\n");
    
    return 0;
}

// Upgrade a package
int package_upgrade(const char* name) {
    if (!name) {
        terminal_write("Error: Package name cannot be NULL\n");
        return -1;
    }
    
    terminal_write("Upgrading package '");
    terminal_write(name);
    terminal_write("'...\n");
    
    // Check if the package is installed
    package_t* pkg = package_get(name);
    
    if (!pkg || !pkg->installed) {
        terminal_write("Error: Package '");
        terminal_write(name);
        terminal_write("' is not installed\n");
        return -1;
    }
    
    // Check if an upgrade is available
    // In a real system, we would check the repositories for a newer version
    // For now, just simulate no upgrade available
    
    terminal_write("Package '");
    terminal_write(name);
    terminal_write("' is already at the latest version\n");
    
    return 0;
}

// Upgrade all packages
int package_upgrade_all() {
    terminal_write("Upgrading all packages...\n");
    
    // Get the list of installed packages
    package_t* installed_packages[MAX_PACKAGES];
    unsigned int installed_count = 0;
    
    if (package_list_installed(installed_packages, &installed_count) != 0) {
        terminal_write("Error: Failed to get list of installed packages\n");
        return -1;
    }
    
    // Upgrade each package
    for (unsigned int i = 0; i < installed_count; i++) {
        package_upgrade(installed_packages[i]->name);
    }
    
    terminal_write("All packages upgraded\n");
    
    return 0;
}

// Get a package by name
package_t* package_get(const char* name) {
    if (!name) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < package_count; i++) {
        if (strcmp(packages[i]->name, name) == 0) {
            return packages[i];
        }
    }
    
    return NULL;
}

// List all packages
int package_list(package_t** packages_out, unsigned int* count) {
    if (!packages_out || !count) {
        terminal_write("Error: Packages and count cannot be NULL\n");
        return -1;
    }
    
    *packages_out = packages;
    *count = package_count;
    
    return 0;
}

// List installed packages
int package_list_installed(package_t** packages_out, unsigned int* count) {
    if (!packages_out || !count) {
        terminal_write("Error: Packages and count cannot be NULL\n");
        return -1;
    }
    
    // Count the number of installed packages
    unsigned int installed_count = 0;
    
    for (unsigned int i = 0; i < package_count; i++) {
        if (packages[i]->installed) {
            installed_count++;
        }
    }
    
    // Allocate memory for the installed packages array
    package_t** installed_packages = (package_t**)allocate_blocks((installed_count * sizeof(package_t*) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!installed_packages) {
        terminal_write("Error: Failed to allocate memory for installed packages\n");
        return -1;
    }
    
    // Fill the installed packages array
    installed_count = 0;
    
    for (unsigned int i = 0; i < package_count; i++) {
        if (packages[i]->installed) {
            installed_packages[installed_count++] = packages[i];
        }
    }
    
    *packages_out = installed_packages;
    *count = installed_count;
    
    return 0;
}

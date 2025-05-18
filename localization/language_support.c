/**
 * LightOS Localization
 * Language Support implementation
 */

#include "language_support.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "localization_manager.h"

// Language installation status
static unsigned int installed_languages[LANGUAGE_MAX];
static unsigned int default_language = LANGUAGE_EN_US;

// Initialize language support
void language_support_init() {
    terminal_write("Initializing language support...\n");
    
    // Clear the installed languages array
    for (int i = 0; i < LANGUAGE_MAX; i++) {
        installed_languages[i] = 0;
    }
    
    // Install the default language (English - US)
    installed_languages[LANGUAGE_EN_US] = 1;
    default_language = LANGUAGE_EN_US;
    
    terminal_write("Language support initialized\n");
}

// Add a language
int language_support_add_language(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        terminal_write("Error: Invalid language\n");
        return -1;
    }
    
    // Check if the language is already installed
    if (installed_languages[language]) {
        terminal_write("Language already installed\n");
        return 0;
    }
    
    // Install the language pack
    if (language_support_install_language_pack(language) != 0) {
        terminal_write("Error: Failed to install language pack\n");
        return -1;
    }
    
    // Mark the language as installed
    installed_languages[language] = 1;
    
    terminal_write("Language added: ");
    terminal_write(localization_get_language_name(language));
    terminal_write("\n");
    
    return 0;
}

// Remove a language
int language_support_remove_language(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        terminal_write("Error: Invalid language\n");
        return -1;
    }
    
    // Check if the language is installed
    if (!installed_languages[language]) {
        terminal_write("Language not installed\n");
        return 0;
    }
    
    // Check if the language is the default language
    if (language == default_language) {
        terminal_write("Error: Cannot remove the default language\n");
        return -1;
    }
    
    // Uninstall the language pack
    if (language_support_uninstall_language_pack(language) != 0) {
        terminal_write("Error: Failed to uninstall language pack\n");
        return -1;
    }
    
    // Mark the language as not installed
    installed_languages[language] = 0;
    
    terminal_write("Language removed: ");
    terminal_write(localization_get_language_name(language));
    terminal_write("\n");
    
    return 0;
}

// Check if a language is installed
int language_support_is_language_installed(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        return 0;
    }
    
    return installed_languages[language];
}

// Get the list of installed languages
int language_support_get_installed_languages(unsigned int* languages, unsigned int* count) {
    if (!languages || !count) {
        return -1;
    }
    
    unsigned int installed_count = 0;
    
    for (unsigned int i = 0; i < LANGUAGE_MAX; i++) {
        if (installed_languages[i]) {
            if (installed_count < *count) {
                languages[installed_count] = i;
            }
            installed_count++;
        }
    }
    
    *count = installed_count;
    
    return 0;
}

// Set the default language
int language_support_set_default_language(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        terminal_write("Error: Invalid language\n");
        return -1;
    }
    
    // Check if the language is installed
    if (!installed_languages[language]) {
        terminal_write("Error: Language not installed\n");
        return -1;
    }
    
    // Set the default language
    default_language = language;
    
    // Update the system language
    localization_set_language(language);
    
    terminal_write("Default language set to: ");
    terminal_write(localization_get_language_name(language));
    terminal_write("\n");
    
    return 0;
}

// Get the default language
unsigned int language_support_get_default_language() {
    return default_language;
}

// Install a language pack
int language_support_install_language_pack(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        terminal_write("Error: Invalid language\n");
        return -1;
    }
    
    terminal_write("Installing language pack: ");
    terminal_write(localization_get_language_name(language));
    terminal_write("...\n");
    
    // In a real system, we would:
    // 1. Download the language pack from a server
    // 2. Verify the package integrity
    // 3. Extract the package
    // 4. Install the language files
    
    // For now, just simulate a successful installation
    terminal_write("Language pack installed successfully\n");
    
    return 0;
}

// Uninstall a language pack
int language_support_uninstall_language_pack(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        terminal_write("Error: Invalid language\n");
        return -1;
    }
    
    terminal_write("Uninstalling language pack: ");
    terminal_write(localization_get_language_name(language));
    terminal_write("...\n");
    
    // In a real system, we would:
    // 1. Remove the language files
    // 2. Update the language database
    
    // For now, just simulate a successful uninstallation
    terminal_write("Language pack uninstalled successfully\n");
    
    return 0;
}

// Update a language pack
int language_support_update_language_pack(unsigned int language) {
    if (language >= LANGUAGE_MAX) {
        terminal_write("Error: Invalid language\n");
        return -1;
    }
    
    // Check if the language is installed
    if (!installed_languages[language]) {
        terminal_write("Error: Language not installed\n");
        return -1;
    }
    
    terminal_write("Updating language pack: ");
    terminal_write(localization_get_language_name(language));
    terminal_write("...\n");
    
    // In a real system, we would:
    // 1. Check for updates
    // 2. Download the updated language pack
    // 3. Verify the package integrity
    // 4. Extract the package
    // 5. Update the language files
    
    // For now, just simulate a successful update
    terminal_write("Language pack updated successfully\n");
    
    return 0;
}

// Get the language pack version
int language_support_get_language_pack_version(unsigned int language, char* version, unsigned int version_size) {
    if (language >= LANGUAGE_MAX || !version || version_size == 0) {
        return -1;
    }
    
    // Check if the language is installed
    if (!installed_languages[language]) {
        return -1;
    }
    
    // In a real system, we would get the actual version
    // For now, just return a dummy version
    strncpy(version, "1.0.0", version_size);
    
    return 0;
}

// Get the language pack size
int language_support_get_language_pack_size(unsigned int language, unsigned int* size) {
    if (language >= LANGUAGE_MAX || !size) {
        return -1;
    }
    
    // Check if the language is installed
    if (!installed_languages[language]) {
        return -1;
    }
    
    // In a real system, we would get the actual size
    // For now, just return a dummy size
    *size = 1024 * 1024; // 1 MB
    
    return 0;
}

// Get the language pack author
int language_support_get_language_pack_author(unsigned int language, char* author, unsigned int author_size) {
    if (language >= LANGUAGE_MAX || !author || author_size == 0) {
        return -1;
    }
    
    // Check if the language is installed
    if (!installed_languages[language]) {
        return -1;
    }
    
    // In a real system, we would get the actual author
    // For now, just return a dummy author
    strncpy(author, "LightOS Team", author_size);
    
    return 0;
}

/**
 * LightOS Accessibility
 * Accessibility Manager implementation
 */

#include "accessibility_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "screen_reader.h"
#include "magnifier.h"
#include "high_contrast.h"
#include "keyboard_helper.h"

// Global accessibility settings
static accessibility_settings_t accessibility_settings;

// Initialize the accessibility manager
void accessibility_manager_init() {
    terminal_write("Initializing accessibility manager...\n");
    
    // Set default settings
    accessibility_settings.enabled_features = 0;
    
    // Screen reader settings
    accessibility_settings.screen_reader_speed = 50;
    accessibility_settings.screen_reader_volume = 75;
    accessibility_settings.screen_reader_pitch = 50;
    
    // Magnifier settings
    accessibility_settings.magnifier_zoom = 200;
    accessibility_settings.magnifier_follow_mouse = 1;
    accessibility_settings.magnifier_follow_keyboard = 1;
    accessibility_settings.magnifier_follow_text = 1;
    
    // High contrast settings
    accessibility_settings.high_contrast_theme = 0;
    accessibility_settings.high_contrast_text_size = 100;
    
    // Keyboard helper settings
    accessibility_settings.keyboard_sticky_keys = 0;
    accessibility_settings.keyboard_bounce_keys = 0;
    accessibility_settings.keyboard_toggle_keys = 0;
    accessibility_settings.keyboard_slow_keys = 0;
    accessibility_settings.keyboard_mouse_keys = 0;
    
    // Mouse helper settings
    accessibility_settings.mouse_pointer_size = 100;
    accessibility_settings.mouse_pointer_color = 0;
    accessibility_settings.mouse_pointer_speed = 50;
    accessibility_settings.mouse_click_assist = 0;
    
    // Text-to-speech settings
    accessibility_settings.tts_voice = 0;
    accessibility_settings.tts_speed = 50;
    accessibility_settings.tts_volume = 75;
    accessibility_settings.tts_pitch = 50;
    
    // Speech-to-text settings
    accessibility_settings.stt_language = 0;
    accessibility_settings.stt_continuous = 0;
    accessibility_settings.stt_punctuation = 1;
    
    // Color filter settings
    accessibility_settings.color_filter_type = 0;
    accessibility_settings.color_filter_intensity = 50;
    
    // Initialize accessibility features
    screen_reader_init();
    magnifier_init();
    high_contrast_init();
    keyboard_helper_init();
    
    // Load settings from file
    accessibility_load_settings();
    
    terminal_write("Accessibility manager initialized\n");
}

// Enable an accessibility feature
int accessibility_enable_feature(unsigned int feature) {
    // Check if the feature is valid
    if (feature != ACCESSIBILITY_SCREEN_READER &&
        feature != ACCESSIBILITY_MAGNIFIER &&
        feature != ACCESSIBILITY_HIGH_CONTRAST &&
        feature != ACCESSIBILITY_KEYBOARD_HELPER &&
        feature != ACCESSIBILITY_MOUSE_HELPER &&
        feature != ACCESSIBILITY_TEXT_TO_SPEECH &&
        feature != ACCESSIBILITY_SPEECH_TO_TEXT &&
        feature != ACCESSIBILITY_COLOR_FILTERS) {
        terminal_write("Error: Invalid accessibility feature\n");
        return -1;
    }
    
    // Enable the feature
    accessibility_settings.enabled_features |= feature;
    
    // Initialize the feature if needed
    if (feature == ACCESSIBILITY_SCREEN_READER) {
        screen_reader_init();
    } else if (feature == ACCESSIBILITY_MAGNIFIER) {
        magnifier_init();
    } else if (feature == ACCESSIBILITY_HIGH_CONTRAST) {
        high_contrast_init();
    } else if (feature == ACCESSIBILITY_KEYBOARD_HELPER) {
        keyboard_helper_init();
    } else if (feature == ACCESSIBILITY_MOUSE_HELPER) {
        mouse_helper_init();
    } else if (feature == ACCESSIBILITY_TEXT_TO_SPEECH) {
        text_to_speech_init();
    } else if (feature == ACCESSIBILITY_SPEECH_TO_TEXT) {
        speech_to_text_init();
    } else if (feature == ACCESSIBILITY_COLOR_FILTERS) {
        color_filter_init();
    }
    
    // Save settings
    accessibility_save_settings();
    
    return 0;
}

// Disable an accessibility feature
int accessibility_disable_feature(unsigned int feature) {
    // Check if the feature is valid
    if (feature != ACCESSIBILITY_SCREEN_READER &&
        feature != ACCESSIBILITY_MAGNIFIER &&
        feature != ACCESSIBILITY_HIGH_CONTRAST &&
        feature != ACCESSIBILITY_KEYBOARD_HELPER &&
        feature != ACCESSIBILITY_MOUSE_HELPER &&
        feature != ACCESSIBILITY_TEXT_TO_SPEECH &&
        feature != ACCESSIBILITY_SPEECH_TO_TEXT &&
        feature != ACCESSIBILITY_COLOR_FILTERS) {
        terminal_write("Error: Invalid accessibility feature\n");
        return -1;
    }
    
    // Disable the feature
    accessibility_settings.enabled_features &= ~feature;
    
    // Save settings
    accessibility_save_settings();
    
    return 0;
}

// Check if an accessibility feature is enabled
int accessibility_is_feature_enabled(unsigned int feature) {
    return (accessibility_settings.enabled_features & feature) != 0;
}

// Get the accessibility settings
accessibility_settings_t* accessibility_get_settings() {
    return &accessibility_settings;
}

// Set the accessibility settings
int accessibility_set_settings(accessibility_settings_t* settings) {
    if (!settings) {
        terminal_write("Error: Invalid settings\n");
        return -1;
    }
    
    // Copy the settings
    accessibility_settings = *settings;
    
    // Save settings
    accessibility_save_settings();
    
    return 0;
}

// Reset the accessibility settings to defaults
int accessibility_reset_settings() {
    // Reset to default settings
    accessibility_settings.enabled_features = 0;
    
    // Screen reader settings
    accessibility_settings.screen_reader_speed = 50;
    accessibility_settings.screen_reader_volume = 75;
    accessibility_settings.screen_reader_pitch = 50;
    
    // Magnifier settings
    accessibility_settings.magnifier_zoom = 200;
    accessibility_settings.magnifier_follow_mouse = 1;
    accessibility_settings.magnifier_follow_keyboard = 1;
    accessibility_settings.magnifier_follow_text = 1;
    
    // High contrast settings
    accessibility_settings.high_contrast_theme = 0;
    accessibility_settings.high_contrast_text_size = 100;
    
    // Keyboard helper settings
    accessibility_settings.keyboard_sticky_keys = 0;
    accessibility_settings.keyboard_bounce_keys = 0;
    accessibility_settings.keyboard_toggle_keys = 0;
    accessibility_settings.keyboard_slow_keys = 0;
    accessibility_settings.keyboard_mouse_keys = 0;
    
    // Mouse helper settings
    accessibility_settings.mouse_pointer_size = 100;
    accessibility_settings.mouse_pointer_color = 0;
    accessibility_settings.mouse_pointer_speed = 50;
    accessibility_settings.mouse_click_assist = 0;
    
    // Text-to-speech settings
    accessibility_settings.tts_voice = 0;
    accessibility_settings.tts_speed = 50;
    accessibility_settings.tts_volume = 75;
    accessibility_settings.tts_pitch = 50;
    
    // Speech-to-text settings
    accessibility_settings.stt_language = 0;
    accessibility_settings.stt_continuous = 0;
    accessibility_settings.stt_punctuation = 1;
    
    // Color filter settings
    accessibility_settings.color_filter_type = 0;
    accessibility_settings.color_filter_intensity = 50;
    
    // Save settings
    accessibility_save_settings();
    
    return 0;
}

// Save the accessibility settings to a file
int accessibility_save_settings() {
    // In a real system, we would save the settings to a file
    // For now, just simulate a successful save
    return 0;
}

// Load the accessibility settings from a file
int accessibility_load_settings() {
    // In a real system, we would load the settings from a file
    // For now, just simulate a successful load
    return 0;
}

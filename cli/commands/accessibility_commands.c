/**
 * LightOS CLI
 * Accessibility Commands implementation
 */

#include "accessibility_commands.h"
#include "../../kernel/kernel.h"
#include "../../accessibility/accessibility_manager.h"
#include "../../accessibility/screen_reader.h"
#include "../../accessibility/magnifier.h"
#include "../../accessibility/high_contrast.h"
#include "../../accessibility/keyboard_helper.h"
#include "../../libc/string.h"

// Register accessibility commands
void register_accessibility_commands() {
    cli_register_command("accessibility", accessibility_command, "Accessibility settings and features");
    cli_register_command("screen-reader", screen_reader_command, "Screen reader settings and control");
    cli_register_command("magnifier", magnifier_command, "Screen magnifier settings and control");
    cli_register_command("high-contrast", high_contrast_command, "High contrast mode settings and control");
    cli_register_command("keyboard-helper", keyboard_helper_command, "Keyboard accessibility settings and control");
}

// Accessibility command handler
int accessibility_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: accessibility <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  enable <feature>   - Enable an accessibility feature\n");
        terminal_write("  disable <feature>  - Disable an accessibility feature\n");
        terminal_write("  status             - Show the status of all accessibility features\n");
        terminal_write("  reset              - Reset all accessibility settings to defaults\n");
        terminal_write("  save               - Save accessibility settings\n");
        terminal_write("  load               - Load accessibility settings\n");
        terminal_write("Features:\n");
        terminal_write("  screen-reader      - Screen reader\n");
        terminal_write("  magnifier          - Screen magnifier\n");
        terminal_write("  high-contrast      - High contrast mode\n");
        terminal_write("  keyboard-helper    - Keyboard accessibility features\n");
        terminal_write("  mouse-helper       - Mouse accessibility features\n");
        terminal_write("  text-to-speech     - Text-to-speech\n");
        terminal_write("  speech-to-text     - Speech-to-text\n");
        terminal_write("  color-filters      - Color filters\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "enable") == 0) {
        if (argc < 3) {
            terminal_write("Usage: accessibility enable <feature>\n");
            return -1;
        }
        
        const char* feature = argv[2];
        unsigned int feature_flag = 0;
        
        if (strcmp(feature, "screen-reader") == 0) {
            feature_flag = ACCESSIBILITY_SCREEN_READER;
        } else if (strcmp(feature, "magnifier") == 0) {
            feature_flag = ACCESSIBILITY_MAGNIFIER;
        } else if (strcmp(feature, "high-contrast") == 0) {
            feature_flag = ACCESSIBILITY_HIGH_CONTRAST;
        } else if (strcmp(feature, "keyboard-helper") == 0) {
            feature_flag = ACCESSIBILITY_KEYBOARD_HELPER;
        } else if (strcmp(feature, "mouse-helper") == 0) {
            feature_flag = ACCESSIBILITY_MOUSE_HELPER;
        } else if (strcmp(feature, "text-to-speech") == 0) {
            feature_flag = ACCESSIBILITY_TEXT_TO_SPEECH;
        } else if (strcmp(feature, "speech-to-text") == 0) {
            feature_flag = ACCESSIBILITY_SPEECH_TO_TEXT;
        } else if (strcmp(feature, "color-filters") == 0) {
            feature_flag = ACCESSIBILITY_COLOR_FILTERS;
        } else {
            terminal_write("Error: Unknown feature '");
            terminal_write(feature);
            terminal_write("'\n");
            return -1;
        }
        
        if (accessibility_enable_feature(feature_flag) != 0) {
            terminal_write("Error: Failed to enable feature '");
            terminal_write(feature);
            terminal_write("'\n");
            return -1;
        }
        
        terminal_write("Enabled accessibility feature '");
        terminal_write(feature);
        terminal_write("'\n");
        
        return 0;
    }
    else if (strcmp(command, "disable") == 0) {
        if (argc < 3) {
            terminal_write("Usage: accessibility disable <feature>\n");
            return -1;
        }
        
        const char* feature = argv[2];
        unsigned int feature_flag = 0;
        
        if (strcmp(feature, "screen-reader") == 0) {
            feature_flag = ACCESSIBILITY_SCREEN_READER;
        } else if (strcmp(feature, "magnifier") == 0) {
            feature_flag = ACCESSIBILITY_MAGNIFIER;
        } else if (strcmp(feature, "high-contrast") == 0) {
            feature_flag = ACCESSIBILITY_HIGH_CONTRAST;
        } else if (strcmp(feature, "keyboard-helper") == 0) {
            feature_flag = ACCESSIBILITY_KEYBOARD_HELPER;
        } else if (strcmp(feature, "mouse-helper") == 0) {
            feature_flag = ACCESSIBILITY_MOUSE_HELPER;
        } else if (strcmp(feature, "text-to-speech") == 0) {
            feature_flag = ACCESSIBILITY_TEXT_TO_SPEECH;
        } else if (strcmp(feature, "speech-to-text") == 0) {
            feature_flag = ACCESSIBILITY_SPEECH_TO_TEXT;
        } else if (strcmp(feature, "color-filters") == 0) {
            feature_flag = ACCESSIBILITY_COLOR_FILTERS;
        } else {
            terminal_write("Error: Unknown feature '");
            terminal_write(feature);
            terminal_write("'\n");
            return -1;
        }
        
        if (accessibility_disable_feature(feature_flag) != 0) {
            terminal_write("Error: Failed to disable feature '");
            terminal_write(feature);
            terminal_write("'\n");
            return -1;
        }
        
        terminal_write("Disabled accessibility feature '");
        terminal_write(feature);
        terminal_write("'\n");
        
        return 0;
    }
    else if (strcmp(command, "status") == 0) {
        accessibility_settings_t* settings = accessibility_get_settings();
        
        terminal_write("Accessibility Features Status:\n");
        
        terminal_write("  Screen Reader: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_SCREEN_READER) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  Magnifier: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_MAGNIFIER) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  High Contrast: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_HIGH_CONTRAST) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  Keyboard Helper: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_KEYBOARD_HELPER) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  Mouse Helper: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_MOUSE_HELPER) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  Text-to-Speech: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_TEXT_TO_SPEECH) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  Speech-to-Text: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_SPEECH_TO_TEXT) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        terminal_write("  Color Filters: ");
        terminal_write(accessibility_is_feature_enabled(ACCESSIBILITY_COLOR_FILTERS) ? "Enabled" : "Disabled");
        terminal_write("\n");
        
        return 0;
    }
    else if (strcmp(command, "reset") == 0) {
        if (accessibility_reset_settings() != 0) {
            terminal_write("Error: Failed to reset accessibility settings\n");
            return -1;
        }
        
        terminal_write("Reset all accessibility settings to defaults\n");
        
        return 0;
    }
    else if (strcmp(command, "save") == 0) {
        if (accessibility_save_settings() != 0) {
            terminal_write("Error: Failed to save accessibility settings\n");
            return -1;
        }
        
        terminal_write("Saved accessibility settings\n");
        
        return 0;
    }
    else if (strcmp(command, "load") == 0) {
        if (accessibility_load_settings() != 0) {
            terminal_write("Error: Failed to load accessibility settings\n");
            return -1;
        }
        
        terminal_write("Loaded accessibility settings\n");
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Screen reader command handler
int screen_reader_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: screen-reader <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  enable            - Enable the screen reader\n");
        terminal_write("  disable           - Disable the screen reader\n");
        terminal_write("  toggle            - Toggle the screen reader\n");
        terminal_write("  speak <text>      - Speak the specified text\n");
        terminal_write("  stop              - Stop speaking\n");
        terminal_write("  speed <value>     - Set the speech speed (0-100)\n");
        terminal_write("  volume <value>    - Set the speech volume (0-100)\n");
        terminal_write("  pitch <value>     - Set the speech pitch (0-100)\n");
        terminal_write("  status            - Show the screen reader status\n");
        terminal_write("  read-screen       - Read the current screen\n");
        terminal_write("  read-selection    - Read the current selection\n");
        terminal_write("  read-line         - Read the current line\n");
        terminal_write("  read-word         - Read the current word\n");
        terminal_write("  read-character    - Read the current character\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "enable") == 0) {
        if (screen_reader_enable() != 0) {
            terminal_write("Error: Failed to enable screen reader\n");
            return -1;
        }
        
        return 0;
    }
    else if (strcmp(command, "disable") == 0) {
        if (screen_reader_disable() != 0) {
            terminal_write("Error: Failed to disable screen reader\n");
            return -1;
        }
        
        return 0;
    }
    else if (strcmp(command, "toggle") == 0) {
        if (screen_reader_toggle() != 0) {
            terminal_write("Error: Failed to toggle screen reader\n");
            return -1;
        }
        
        return 0;
    }
    else if (strcmp(command, "speak") == 0) {
        if (argc < 3) {
            terminal_write("Usage: screen-reader speak <text>\n");
            return -1;
        }
        
        const char* text = argv[2];
        
        if (screen_reader_speak(text) != 0) {
            terminal_write("Error: Failed to speak text\n");
            return -1;
        }
        
        return 0;
    }
    else if (strcmp(command, "stop") == 0) {
        if (screen_reader_stop() != 0) {
            terminal_write("Error: Failed to stop speaking\n");
            return -1;
        }
        
        return 0;
    }
    else if (strcmp(command, "speed") == 0) {
        if (argc < 3) {
            terminal_write("Usage: screen-reader speed <value>\n");
            return -1;
        }
        
        int speed = atoi(argv[2]);
        
        if (speed < 0 || speed > 100) {
            terminal_write("Error: Speed must be between 0 and 100\n");
            return -1;
        }
        
        if (screen_reader_set_speed(speed) != 0) {
            terminal_write("Error: Failed to set speech speed\n");
            return -1;
        }
        
        terminal_write("Set speech speed to ");
        terminal_write(argv[2]);
        terminal_write("\n");
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

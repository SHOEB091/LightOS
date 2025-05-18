/**
 * LightOS CLI
 * Accessibility Commands header
 */

#ifndef ACCESSIBILITY_COMMANDS_H
#define ACCESSIBILITY_COMMANDS_H

// Register accessibility commands
void register_accessibility_commands();

// Command handlers
int accessibility_command(int argc, char** argv);
int screen_reader_command(int argc, char** argv);
int magnifier_command(int argc, char** argv);
int high_contrast_command(int argc, char** argv);
int keyboard_helper_command(int argc, char** argv);

#endif /* ACCESSIBILITY_COMMANDS_H */

/**
 * LightOS CLI
 * System Commands header
 */

#ifndef SYSTEM_COMMANDS_H
#define SYSTEM_COMMANDS_H

// Register system commands
void register_system_commands();

// Command handlers
int system_command(int argc, char** argv);
int package_command(int argc, char** argv);
int update_command(int argc, char** argv);
int backup_command(int argc, char** argv);
int monitor_command(int argc, char** argv);

#endif /* SYSTEM_COMMANDS_H */

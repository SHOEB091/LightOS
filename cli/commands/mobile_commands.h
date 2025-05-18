/**
 * LightOS CLI
 * Mobile Commands header
 */

#ifndef MOBILE_COMMANDS_H
#define MOBILE_COMMANDS_H

// Register mobile commands
void register_mobile_commands();

// Command handlers
int mobile_command(int argc, char** argv);
int android_command(int argc, char** argv);
int ios_command(int argc, char** argv);
int mobile_sync_command(int argc, char** argv);

#endif /* MOBILE_COMMANDS_H */

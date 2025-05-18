/**
 * LightOS CLI
 * Security Commands header
 */

#ifndef SECURITY_COMMANDS_H
#define SECURITY_COMMANDS_H

// Register security commands
void register_security_commands();

// Command handlers
int security_command(int argc, char** argv);
int user_command(int argc, char** argv);
int group_command(int argc, char** argv);
int firewall_command(int argc, char** argv);
int crypto_command(int argc, char** argv);

#endif /* SECURITY_COMMANDS_H */

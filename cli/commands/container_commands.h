/**
 * LightOS CLI
 * Container Commands header
 */

#ifndef CONTAINER_COMMANDS_H
#define CONTAINER_COMMANDS_H

// Register container commands
void register_container_commands();

// Command handlers
int container_command(int argc, char** argv);
int docker_command(int argc, char** argv);
int lxc_command(int argc, char** argv);

#endif /* CONTAINER_COMMANDS_H */

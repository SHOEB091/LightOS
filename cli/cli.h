/**
 * LightOS CLI
 * Command Line Interface header
 */

#ifndef CLI_H
#define CLI_H

// CLI functions
void cli_init();
void cli_run();
void cli_read_command(char* buffer, int max_length);
void cli_add_to_history(const char* command);
int cli_parse_command(char* command, char* args[], int max_args);
void cli_execute_command(char* command);
int cli_strcmp(const char* s1, const char* s2);

// CLI commands
void cli_cmd_help();
void cli_cmd_clear();
void cli_cmd_echo(int argc, char* argv[]);
void cli_cmd_ls(int argc, char* argv[]);
void cli_cmd_cd(int argc, char* argv[]);
void cli_cmd_pwd();
void cli_cmd_server(int argc, char* argv[]);
void cli_cmd_gui();
void cli_cmd_exit();

// Language-related commands
void cli_cmd_lang(int argc, char* argv[]);
void cli_cmd_lang_list();
void cli_cmd_lang_install(int argc, char* argv[]);
void cli_cmd_lang_uninstall(int argc, char* argv[]);
void cli_cmd_lang_update(int argc, char* argv[]);
void cli_cmd_lang_set_default(int argc, char* argv[]);
void cli_cmd_lang_run(int argc, char* argv[]);
void cli_cmd_lang_repl(int argc, char* argv[]);

// Framework-related commands
void cli_cmd_framework(int argc, char* argv[]);
void cli_cmd_framework_list(int argc, char* argv[]);
void cli_cmd_framework_install(int argc, char* argv[]);
void cli_cmd_framework_uninstall(int argc, char* argv[]);
void cli_cmd_framework_update(int argc, char* argv[]);
void cli_cmd_framework_create(int argc, char* argv[]);
void cli_cmd_framework_run(int argc, char* argv[]);
void cli_cmd_framework_build(int argc, char* argv[]);

// Device-related commands
void cli_cmd_device(int argc, char* argv[]);
void cli_cmd_device_list(int argc, char* argv[]);
void cli_cmd_device_info(int argc, char* argv[]);
void cli_cmd_device_enable(int argc, char* argv[]);
void cli_cmd_device_disable(int argc, char* argv[]);
void cli_cmd_device_drivers(int argc, char* argv[]);

#endif /* CLI_H */

/**
 * LightOS Python Language Support
 * Header for Python language implementation
 */

#ifndef PYTHON_LANGUAGE_H
#define PYTHON_LANGUAGE_H

#include "../language_manager.h"

// Python interpreter options
typedef struct {
    int optimize;            // 0, 1, 2
    int verbose;             // 0 or 1
    int no_site;             // 0 or 1
    int no_user_site;        // 0 or 1
    int ignore_environment;  // 0 or 1
    int debug;               // 0 or 1
    char* python_path;       // Semicolon-separated list of paths
    char* module_search_path; // Semicolon-separated list of module search paths
} PythonOptions;

// Python language functions
int python_language_init();
int python_language_run(const char* file);
int python_language_run_repl();
int python_language_update(LanguageVersion version);

// Python interpreter functions
int python_interpreter_set_options(PythonOptions options);
PythonOptions python_interpreter_get_options();
int python_interpreter_run_string(const char* code);
int python_interpreter_run_file(const char* file);
int python_interpreter_start_repl();

// Python standard library functions
void python_stdlib_init();
void python_stdlib_register_modules();

#endif /* PYTHON_LANGUAGE_H */

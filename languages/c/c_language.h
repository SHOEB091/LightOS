/**
 * LightOS C Language Support
 * Header for C language implementation
 */

#ifndef C_LANGUAGE_H
#define C_LANGUAGE_H

#include "../language_manager.h"

// C compiler options
typedef struct {
    int optimization_level;  // 0, 1, 2, 3, s
    int debug_info;          // 0 or 1
    int warnings_as_errors;  // 0 or 1
    int standard;            // 89, 99, 11, 17
    char* include_paths;     // Semicolon-separated list of include paths
    char* library_paths;     // Semicolon-separated list of library paths
    char* libraries;         // Semicolon-separated list of libraries
} CCompilerOptions;

// C language functions
int c_language_init();
int c_language_compile(const char* source_file, const char* output_file);
int c_language_run(const char* file);
int c_language_update(LanguageVersion version);

// C compiler functions
int c_compiler_set_options(CCompilerOptions options);
CCompilerOptions c_compiler_get_options();
int c_compiler_compile(const char* source_file, const char* output_file);
int c_compiler_link(const char* object_files[], int count, const char* output_file);

// C standard library functions
void c_stdlib_init();
void c_stdlib_register_functions();

#endif /* C_LANGUAGE_H */

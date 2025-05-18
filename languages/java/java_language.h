/**
 * LightOS Java Language Support
 * Header for Java language implementation
 */

#ifndef JAVA_LANGUAGE_H
#define JAVA_LANGUAGE_H

#include "../language_manager.h"

// Java VM options
typedef struct {
    int min_heap_size;       // Minimum heap size in MB
    int max_heap_size;       // Maximum heap size in MB
    int thread_stack_size;   // Thread stack size in KB
    int class_verification;  // 0 or 1
    int debug_mode;          // 0 or 1
    int server_mode;         // 0 or 1
    char* classpath;         // Semicolon-separated list of classpath entries
    char* java_home;         // Java home directory
} JavaVMOptions;

// Java language functions
int java_language_init();
int java_language_compile(const char* source_file, const char* output_file);
int java_language_run(const char* file);
int java_language_run_repl();
int java_language_update(LanguageVersion version);

// Java VM functions
int java_vm_set_options(JavaVMOptions options);
JavaVMOptions java_vm_get_options();
int java_vm_start();
int java_vm_stop();
int java_vm_run_class(const char* class_name, int argc, char* argv[]);
int java_vm_run_jar(const char* jar_file, int argc, char* argv[]);

// Java compiler functions
int java_compiler_compile(const char* source_file, const char* output_dir);
int java_compiler_compile_with_options(const char* source_file, const char* output_dir, const char* options);

// Java standard library functions
void java_stdlib_init();
void java_stdlib_register_packages();

#endif /* JAVA_LANGUAGE_H */

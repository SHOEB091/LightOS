/**
 * LightOS Testing
 * Test framework implementation
 */

#include "test_framework.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// Maximum number of test suites
#define MAX_TEST_SUITES 32

// Maximum number of test cases per suite
#define MAX_TEST_CASES_PER_SUITE 64

// Test suites
static test_suite_t test_suites[MAX_TEST_SUITES];
static unsigned int test_suite_count = 0;

// Test statistics
static unsigned int total_tests = 0;
static unsigned int passed_tests = 0;
static unsigned int failed_tests = 0;
static unsigned int skipped_tests = 0;
static unsigned int error_tests = 0;

// Initialize the test framework
void test_framework_init() {
    terminal_write("Initializing test framework...\n");
    
    // Clear test suites
    memset(test_suites, 0, sizeof(test_suites));
    test_suite_count = 0;
    
    // Reset statistics
    total_tests = 0;
    passed_tests = 0;
    failed_tests = 0;
    skipped_tests = 0;
    error_tests = 0;
    
    terminal_write("Test framework initialized\n");
}

// Find a test suite by name
static test_suite_t* find_test_suite(const char* name) {
    for (unsigned int i = 0; i < test_suite_count; i++) {
        if (strcmp(test_suites[i].name, name) == 0) {
            return &test_suites[i];
        }
    }
    
    return NULL;
}

// Find a test case by name
static test_case_t* find_test_case(test_suite_t* suite, const char* name) {
    if (!suite) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < suite->test_case_count; i++) {
        if (strcmp(suite->test_cases[i].name, name) == 0) {
            return &suite->test_cases[i];
        }
    }
    
    return NULL;
}

// Add a test suite
int test_add_suite(const char* name, const char* description) {
    if (!name) {
        terminal_write("Error: Test suite name cannot be NULL\n");
        return -1;
    }
    
    // Check if the suite already exists
    if (find_test_suite(name)) {
        terminal_write("Error: Test suite '");
        terminal_write(name);
        terminal_write("' already exists\n");
        return -1;
    }
    
    // Check if we have room for another suite
    if (test_suite_count >= MAX_TEST_SUITES) {
        terminal_write("Error: Maximum number of test suites reached\n");
        return -1;
    }
    
    // Add the suite
    strcpy(test_suites[test_suite_count].name, name);
    
    if (description) {
        strcpy(test_suites[test_suite_count].description, description);
    } else {
        test_suites[test_suite_count].description[0] = '\0';
    }
    
    // Allocate memory for test cases
    test_suites[test_suite_count].test_cases = (test_case_t*)allocate_blocks((MAX_TEST_CASES_PER_SUITE * sizeof(test_case_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!test_suites[test_suite_count].test_cases) {
        terminal_write("Error: Failed to allocate memory for test cases\n");
        return -1;
    }
    
    // Clear test cases
    memset(test_suites[test_suite_count].test_cases, 0, MAX_TEST_CASES_PER_SUITE * sizeof(test_case_t));
    test_suites[test_suite_count].test_case_count = 0;
    test_suites[test_suite_count].enabled = 1;
    
    test_suite_count++;
    
    return 0;
}

// Add a test case
int test_add_case(const char* suite_name, const char* name, const char* description, test_func_t test_func) {
    if (!suite_name || !name || !test_func) {
        terminal_write("Error: Invalid test case parameters\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(suite_name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(suite_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the case already exists
    if (find_test_case(suite, name)) {
        terminal_write("Error: Test case '");
        terminal_write(name);
        terminal_write("' already exists in suite '");
        terminal_write(suite_name);
        terminal_write("'\n");
        return -1;
    }
    
    // Check if we have room for another case
    if (suite->test_case_count >= MAX_TEST_CASES_PER_SUITE) {
        terminal_write("Error: Maximum number of test cases reached for suite '");
        terminal_write(suite_name);
        terminal_write("'\n");
        return -1;
    }
    
    // Add the case
    strcpy(suite->test_cases[suite->test_case_count].name, name);
    
    if (description) {
        strcpy(suite->test_cases[suite->test_case_count].description, description);
    } else {
        suite->test_cases[suite->test_case_count].description[0] = '\0';
    }
    
    suite->test_cases[suite->test_case_count].test_func = test_func;
    suite->test_cases[suite->test_case_count].enabled = 1;
    
    suite->test_case_count++;
    
    return 0;
}

// Enable a test suite
int test_enable_suite(const char* name) {
    if (!name) {
        terminal_write("Error: Test suite name cannot be NULL\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    suite->enabled = 1;
    
    return 0;
}

// Disable a test suite
int test_disable_suite(const char* name) {
    if (!name) {
        terminal_write("Error: Test suite name cannot be NULL\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    suite->enabled = 0;
    
    return 0;
}

// Enable a test case
int test_enable_case(const char* suite_name, const char* name) {
    if (!suite_name || !name) {
        terminal_write("Error: Invalid test case parameters\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(suite_name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(suite_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Find the case
    test_case_t* test_case = find_test_case(suite, name);
    
    if (!test_case) {
        terminal_write("Error: Test case '");
        terminal_write(name);
        terminal_write("' not found in suite '");
        terminal_write(suite_name);
        terminal_write("'\n");
        return -1;
    }
    
    test_case->enabled = 1;
    
    return 0;
}

// Disable a test case
int test_disable_case(const char* suite_name, const char* name) {
    if (!suite_name || !name) {
        terminal_write("Error: Invalid test case parameters\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(suite_name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(suite_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Find the case
    test_case_t* test_case = find_test_case(suite, name);
    
    if (!test_case) {
        terminal_write("Error: Test case '");
        terminal_write(name);
        terminal_write("' not found in suite '");
        terminal_write(suite_name);
        terminal_write("'\n");
        return -1;
    }
    
    test_case->enabled = 0;
    
    return 0;
}

// Run a test suite
int test_run_suite(const char* name) {
    if (!name) {
        terminal_write("Error: Test suite name cannot be NULL\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!suite->enabled) {
        terminal_write("Test suite '");
        terminal_write(name);
        terminal_write("' is disabled\n");
        return 0;
    }
    
    terminal_write("Running test suite '");
    terminal_write(name);
    terminal_write("'...\n");
    
    // Run all enabled test cases
    for (unsigned int i = 0; i < suite->test_case_count; i++) {
        if (suite->test_cases[i].enabled) {
            terminal_write("  Running test case '");
            terminal_write(suite->test_cases[i].name);
            terminal_write("'... ");
            
            total_tests++;
            
            // Run the test
            test_result_t result = suite->test_cases[i].test_func();
            
            // Update statistics
            switch (result) {
                case TEST_RESULT_PASS:
                    terminal_write_color("PASS\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
                    passed_tests++;
                    break;
                case TEST_RESULT_FAIL:
                    terminal_write_color("FAIL\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
                    failed_tests++;
                    break;
                case TEST_RESULT_SKIP:
                    terminal_write_color("SKIP\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
                    skipped_tests++;
                    break;
                case TEST_RESULT_ERROR:
                    terminal_write_color("ERROR\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
                    error_tests++;
                    break;
            }
        }
    }
    
    return 0;
}

// Run a test case
int test_run_case(const char* suite_name, const char* name) {
    if (!suite_name || !name) {
        terminal_write("Error: Invalid test case parameters\n");
        return -1;
    }
    
    // Find the suite
    test_suite_t* suite = find_test_suite(suite_name);
    
    if (!suite) {
        terminal_write("Error: Test suite '");
        terminal_write(suite_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!suite->enabled) {
        terminal_write("Test suite '");
        terminal_write(suite_name);
        terminal_write("' is disabled\n");
        return 0;
    }
    
    // Find the case
    test_case_t* test_case = find_test_case(suite, name);
    
    if (!test_case) {
        terminal_write("Error: Test case '");
        terminal_write(name);
        terminal_write("' not found in suite '");
        terminal_write(suite_name);
        terminal_write("'\n");
        return -1;
    }
    
    if (!test_case->enabled) {
        terminal_write("Test case '");
        terminal_write(name);
        terminal_write("' is disabled\n");
        return 0;
    }
    
    terminal_write("Running test case '");
    terminal_write(suite_name);
    terminal_write(".");
    terminal_write(name);
    terminal_write("'... ");
    
    total_tests++;
    
    // Run the test
    test_result_t result = test_case->test_func();
    
    // Update statistics
    switch (result) {
        case TEST_RESULT_PASS:
            terminal_write_color("PASS\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            passed_tests++;
            break;
        case TEST_RESULT_FAIL:
            terminal_write_color("FAIL\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            failed_tests++;
            break;
        case TEST_RESULT_SKIP:
            terminal_write_color("SKIP\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
            skipped_tests++;
            break;
        case TEST_RESULT_ERROR:
            terminal_write_color("ERROR\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
            error_tests++;
            break;
    }
    
    return 0;
}

// Run all tests
int test_run_all() {
    terminal_write("Running all tests...\n");
    
    // Reset statistics
    total_tests = 0;
    passed_tests = 0;
    failed_tests = 0;
    skipped_tests = 0;
    error_tests = 0;
    
    // Run all enabled suites
    for (unsigned int i = 0; i < test_suite_count; i++) {
        if (test_suites[i].enabled) {
            test_run_suite(test_suites[i].name);
        }
    }
    
    // Print results
    test_print_results();
    
    return 0;
}

// Print test results
void test_print_results() {
    terminal_write("\nTest Results:\n");
    terminal_write("-------------\n");
    
    terminal_write("Total tests: ");
    
    // Convert total_tests to string
    char count_str[16];
    int temp = total_tests;
    int i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write(count_str);
    terminal_write("\n");
    
    // Print passed tests
    terminal_write("Passed: ");
    
    temp = passed_tests;
    i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write_color(count_str, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_write("\n");
    
    // Print failed tests
    terminal_write("Failed: ");
    
    temp = failed_tests;
    i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write_color(count_str, VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    terminal_write("\n");
    
    // Print skipped tests
    terminal_write("Skipped: ");
    
    temp = skipped_tests;
    i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write_color(count_str, VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    terminal_write("\n");
    
    // Print error tests
    terminal_write("Errors: ");
    
    temp = error_tests;
    i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write_color(count_str, VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    terminal_write("\n");
}

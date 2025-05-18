/**
 * LightOS Testing
 * Test framework header
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

// Test result types
typedef enum {
    TEST_RESULT_PASS,
    TEST_RESULT_FAIL,
    TEST_RESULT_SKIP,
    TEST_RESULT_ERROR
} test_result_t;

// Test function type
typedef test_result_t (*test_func_t)(void);

// Test case structure
typedef struct {
    char name[64];
    char description[256];
    test_func_t test_func;
    unsigned int enabled;
} test_case_t;

// Test suite structure
typedef struct {
    char name[64];
    char description[256];
    test_case_t* test_cases;
    unsigned int test_case_count;
    unsigned int enabled;
} test_suite_t;

// Test framework functions
void test_framework_init();
int test_add_suite(const char* name, const char* description);
int test_add_case(const char* suite_name, const char* name, const char* description, test_func_t test_func);
int test_enable_suite(const char* name);
int test_disable_suite(const char* name);
int test_enable_case(const char* suite_name, const char* name);
int test_disable_case(const char* suite_name, const char* name);
int test_run_suite(const char* name);
int test_run_case(const char* suite_name, const char* name);
int test_run_all();
void test_print_results();

// Assertion macros
#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            terminal_write("Assertion failed: " #condition "\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            terminal_write("Assertion failed: " #expected " == " #actual "\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#define TEST_ASSERT_NOT_EQUAL(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            terminal_write("Assertion failed: " #expected " != " #actual "\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#define TEST_ASSERT_NULL(pointer) \
    do { \
        if ((pointer) != NULL) { \
            terminal_write("Assertion failed: " #pointer " == NULL\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#define TEST_ASSERT_NOT_NULL(pointer) \
    do { \
        if ((pointer) == NULL) { \
            terminal_write("Assertion failed: " #pointer " != NULL\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#define TEST_ASSERT_STRING_EQUAL(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            terminal_write("Assertion failed: " #expected " == " #actual "\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#define TEST_ASSERT_STRING_NOT_EQUAL(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) == 0) { \
            terminal_write("Assertion failed: " #expected " != " #actual "\n"); \
            return TEST_RESULT_FAIL; \
        } \
    } while (0)

#endif /* TEST_FRAMEWORK_H */

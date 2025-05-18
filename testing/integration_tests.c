/**
 * LightOS Testing
 * Integration tests implementation
 */

#include "test_framework.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../kernel/filesystem.h"
#include "../kernel/filesystem_ext.h"
#include "../drivers/driver_manager.h"
#include "../drivers/keyboard.h"
#include "../drivers/mouse.h"
#include "../drivers/storage.h"
#include "../drivers/network_driver.h"
#include "../networking/network.h"
#include "../networking/tcp.h"
#include "../gui/gui.h"
#include "../gui/window_manager.h"
#include "../server/server.h"
#include "../package-manager/package_manager.h"
#include "../security/security_manager.h"
#include "../performance/performance_monitor.h"
#include "../libc/string.h"

// Test driver manager integration
test_result_t test_driver_manager_integration() {
    // Initialize driver manager
    driver_manager_init();
    
    // Check if we can get devices
    Device* devices = driver_manager_detect_devices();
    TEST_ASSERT_NOT_NULL(devices);
    
    // Check if we can get a keyboard device
    Device* keyboard = driver_manager_get_device_by_type(DEVICE_KEYBOARD, 0);
    TEST_ASSERT_NOT_NULL(keyboard);
    
    // Check if we can get a mouse device
    Device* mouse = driver_manager_get_device_by_type(DEVICE_MOUSE, 0);
    TEST_ASSERT_NOT_NULL(mouse);
    
    // Check if we can get a storage device
    Device* storage = driver_manager_get_device_by_type(DEVICE_STORAGE, 0);
    TEST_ASSERT_NOT_NULL(storage);
    
    // Check if we can get a network device
    Device* network = driver_manager_get_device_by_type(DEVICE_NETWORK, 0);
    TEST_ASSERT_NOT_NULL(network);
    
    return TEST_RESULT_PASS;
}

// Test storage driver integration
test_result_t test_storage_driver_integration() {
    // Initialize storage subsystem
    storage_init();
    
    // Check if we can get a storage device
    storage_device_t* device = storage_get_device("hda");
    TEST_ASSERT_NOT_NULL(device);
    
    // Allocate a buffer for testing
    unsigned char* buffer = (unsigned char*)allocate_block();
    TEST_ASSERT_NOT_NULL(buffer);
    
    // Try to read from the device
    int result = storage_read_sectors("hda", 0, 1, buffer);
    TEST_ASSERT_EQUAL(0, result);
    
    // Free the buffer
    free_block(buffer);
    
    return TEST_RESULT_PASS;
}

// Test network driver integration
test_result_t test_network_driver_integration() {
    // Initialize network drivers
    network_driver_init();
    
    // Check if we can get a network driver
    network_driver_t* driver = network_driver_get("eth0");
    TEST_ASSERT_NOT_NULL(driver);
    
    // Allocate a packet for testing
    network_packet_t* packet = network_packet_allocate(1500);
    TEST_ASSERT_NOT_NULL(packet);
    
    // Fill the packet with some data
    memset(packet->data, 0xAA, packet->length);
    
    // Try to send the packet
    int result = network_driver_send_packet("eth0", packet);
    TEST_ASSERT_EQUAL(0, result);
    
    // Free the packet
    network_packet_free(packet);
    
    return TEST_RESULT_PASS;
}

// Test file system integration
test_result_t test_filesystem_integration() {
    // Initialize file system manager
    fs_manager_init();
    
    // Check if we can get a file system
    filesystem_t* fs = fs_get_filesystem("ext4");
    TEST_ASSERT_NOT_NULL(fs);
    
    // Try to mount the file system
    int result = fs_mount("ext4", "hda", "/", 0);
    TEST_ASSERT_EQUAL(0, result);
    
    // Try to unmount the file system
    result = fs_unmount("/");
    TEST_ASSERT_EQUAL(0, result);
    
    return TEST_RESULT_PASS;
}

// Test networking integration
test_result_t test_networking_integration() {
    // Initialize networking
    network_init();
    
    // Initialize TCP
    tcp_init();
    
    // Register a network interface
    unsigned char mac_addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    int interface_idx = network_register_interface("eth0", mac_addr, NULL, NULL);
    TEST_ASSERT(interface_idx >= 0);
    
    // Configure the interface
    int result = network_configure_interface(interface_idx, 0x0A000001, 0xFFFFFF00, 0x0A000001);
    TEST_ASSERT_EQUAL(0, result);
    
    // Bring up the interface
    result = network_interface_up(interface_idx);
    TEST_ASSERT_EQUAL(0, result);
    
    // Create a TCP socket
    int socket_id = tcp_socket_create();
    TEST_ASSERT(socket_id > 0);
    
    // Close the socket
    result = tcp_socket_close(socket_id);
    TEST_ASSERT_EQUAL(0, result);
    
    // Bring down the interface
    result = network_interface_down(interface_idx);
    TEST_ASSERT_EQUAL(0, result);
    
    return TEST_RESULT_PASS;
}

// Test GUI integration
test_result_t test_gui_integration() {
    // Initialize GUI
    gui_init();
    
    // Initialize window manager
    window_manager_init();
    
    // Create a window
    int window_id = window_create("Test Window", 100, 100, 400, 300, WINDOW_FLAG_VISIBLE | WINDOW_FLAG_BORDER | WINDOW_FLAG_TITLEBAR);
    TEST_ASSERT(window_id > 0);
    
    // Get the window
    window_t* window = window_get(window_id);
    TEST_ASSERT_NOT_NULL(window);
    
    // Draw something in the window
    int result = window_fill_rect(window_id, 10, 10, 100, 100, 0xFF0000);
    TEST_ASSERT_EQUAL(0, result);
    
    // Destroy the window
    result = window_destroy(window_id);
    TEST_ASSERT_EQUAL(0, result);
    
    return TEST_RESULT_PASS;
}

// Test server integration
test_result_t test_server_integration() {
    // Initialize server
    server_init();
    
    // Configure the server
    server_config_t config;
    config.port = 8080;
    config.max_connections = 10;
    config.document_root = "/var/www";
    config.server_name = "LightOS Test Server";
    
    server_configure(&config);
    
    // Start the server
    int result = server_start();
    TEST_ASSERT_EQUAL(0, result);
    
    // Check if the server is running
    TEST_ASSERT(server_is_running());
    
    // Stop the server
    server_stop();
    
    // Check if the server is stopped
    TEST_ASSERT(!server_is_running());
    
    return TEST_RESULT_PASS;
}

// Test package manager integration
test_result_t test_package_manager_integration() {
    // Initialize package manager
    package_manager_init();
    
    // Install a package
    int result = package_install("test-package", "1.0.0");
    TEST_ASSERT_EQUAL(0, result);
    
    // Get package info
    package_t* package = package_get_info("test-package");
    TEST_ASSERT_NOT_NULL(package);
    TEST_ASSERT(package->installed);
    
    // Update the package
    result = package_update("test-package");
    TEST_ASSERT_EQUAL(0, result);
    
    // Uninstall the package
    result = package_uninstall("test-package");
    TEST_ASSERT_EQUAL(0, result);
    
    return TEST_RESULT_PASS;
}

// Test security manager integration
test_result_t test_security_integration() {
    // Initialize security manager
    security_manager_init();
    
    // Add a user
    int result = security_add_user("testuser", "password", 2000, 2000, "/home/testuser", "/bin/bash", 0);
    TEST_ASSERT_EQUAL(0, result);
    
    // Get the user
    user_t* user = security_get_user("testuser");
    TEST_ASSERT_NOT_NULL(user);
    TEST_ASSERT_EQUAL(2000, user->uid);
    
    // Add a group
    result = security_add_group("testgroup", 2000);
    TEST_ASSERT_EQUAL(0, result);
    
    // Get the group
    group_t* group = security_get_group("testgroup");
    TEST_ASSERT_NOT_NULL(group);
    TEST_ASSERT_EQUAL(2000, group->gid);
    
    // Add the user to the group
    result = security_add_user_to_group("testuser", "testgroup");
    TEST_ASSERT_EQUAL(0, result);
    
    // Authenticate the user
    result = security_authenticate("testuser", "password");
    TEST_ASSERT_EQUAL(0, result);
    
    // Remove the user from the group
    result = security_remove_user_from_group("testuser", "testgroup");
    TEST_ASSERT_EQUAL(0, result);
    
    // Remove the group
    result = security_remove_group("testgroup");
    TEST_ASSERT_EQUAL(0, result);
    
    // Remove the user
    result = security_remove_user("testuser");
    TEST_ASSERT_EQUAL(0, result);
    
    return TEST_RESULT_PASS;
}

// Test performance monitor integration
test_result_t test_performance_integration() {
    // Initialize performance monitor
    performance_monitor_init();
    
    // Start the monitor
    performance_monitor_start();
    
    // Check if the monitor is running
    TEST_ASSERT(performance_monitor_is_running());
    
    // Update the monitor
    performance_monitor_update();
    
    // Get a counter
    performance_counter_t* counter = performance_monitor_get_counter(PERF_COUNTER_CPU_USAGE);
    TEST_ASSERT_NOT_NULL(counter);
    
    // Add an event
    int result = performance_monitor_add_event(PERF_EVENT_PROCESS_CREATED, 1, 1, 0, "Test process created");
    TEST_ASSERT_EQUAL(0, result);
    
    // Stop the monitor
    performance_monitor_stop();
    
    // Check if the monitor is stopped
    TEST_ASSERT(!performance_monitor_is_running());
    
    return TEST_RESULT_PASS;
}

// Initialize integration tests
void integration_tests_init() {
    // Initialize the test framework
    test_framework_init();
    
    // Add test suites
    test_add_suite("integration", "Integration tests for LightOS components");
    
    // Add test cases
    test_add_case("integration", "driver_manager", "Test driver manager integration", test_driver_manager_integration);
    test_add_case("integration", "storage_driver", "Test storage driver integration", test_storage_driver_integration);
    test_add_case("integration", "network_driver", "Test network driver integration", test_network_driver_integration);
    test_add_case("integration", "filesystem", "Test file system integration", test_filesystem_integration);
    test_add_case("integration", "networking", "Test networking integration", test_networking_integration);
    test_add_case("integration", "gui", "Test GUI integration", test_gui_integration);
    test_add_case("integration", "server", "Test server integration", test_server_integration);
    test_add_case("integration", "package_manager", "Test package manager integration", test_package_manager_integration);
    test_add_case("integration", "security", "Test security integration", test_security_integration);
    test_add_case("integration", "performance", "Test performance monitor integration", test_performance_integration);
}

// Run integration tests
void integration_tests_run() {
    // Run all integration tests
    test_run_suite("integration");
}

/**
 * LightOS Drivers
 * Mouse driver implementation
 */

#include "mouse.h"
#include "../kernel/kernel.h"

// Mouse state
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_buttons = 0;
static int mouse_wheel_delta = 0;

// Mouse bounds
static int mouse_min_x = 0;
static int mouse_min_y = 0;
static int mouse_max_x = 639; // Default to VGA width - 1
static int mouse_max_y = 479; // Default to VGA height - 1

// Mouse packet state
static unsigned char mouse_packet[4];
static int mouse_packet_index = 0;
static int mouse_packet_size = 3; // Default to 3-byte packet

// Mouse callbacks
#define MAX_MOUSE_CALLBACKS 10
static mouse_callback_t mouse_callbacks[MAX_MOUSE_CALLBACKS];
static int mouse_callback_count = 0;

// Initialize the mouse
void mouse_init() {
    // Reset mouse state
    mouse_x = 0;
    mouse_y = 0;
    mouse_buttons = 0;
    mouse_wheel_delta = 0;
    mouse_packet_index = 0;
    
    // Clear callbacks
    for (int i = 0; i < MAX_MOUSE_CALLBACKS; i++) {
        mouse_callbacks[i] = NULL;
    }
    mouse_callback_count = 0;
    
    // In a real system, we would:
    // 1. Send initialization commands to the mouse
    // 2. Enable mouse interrupts
    // 3. Configure the mouse for the desired mode (e.g., wheel support)
    
    terminal_write("Mouse initialized\n");
}

// Handle mouse data
void mouse_handler(unsigned char data) {
    // Add the byte to the packet
    mouse_packet[mouse_packet_index++] = data;
    
    // Check if we have a complete packet
    if (mouse_packet_index >= mouse_packet_size) {
        // Process the packet
        int dx = mouse_packet[1];
        int dy = mouse_packet[2];
        
        // Handle sign bits
        if (mouse_packet[0] & 0x10) dx |= 0xFFFFFF00; // Sign extend
        if (mouse_packet[0] & 0x20) dy |= 0xFFFFFF00; // Sign extend
        
        // Invert Y axis (mouse Y is inverted relative to screen Y)
        dy = -dy;
        
        // Update mouse position
        mouse_x += dx;
        mouse_y += dy;
        
        // Apply bounds
        if (mouse_x < mouse_min_x) mouse_x = mouse_min_x;
        if (mouse_y < mouse_min_y) mouse_y = mouse_min_y;
        if (mouse_x > mouse_max_x) mouse_x = mouse_max_x;
        if (mouse_y > mouse_max_y) mouse_y = mouse_max_y;
        
        // Update button state
        mouse_buttons = mouse_packet[0] & 0x07; // Bottom 3 bits are button states
        
        // Handle wheel if we have a 4-byte packet
        if (mouse_packet_size == 4) {
            mouse_wheel_delta = (char)mouse_packet[3]; // Signed value
        } else {
            mouse_wheel_delta = 0;
        }
        
        // Create mouse event
        mouse_event_t event;
        event.x = mouse_x;
        event.y = mouse_y;
        event.dx = dx;
        event.dy = dy;
        event.buttons = mouse_buttons;
        event.wheel_delta = mouse_wheel_delta;
        
        // Call registered callbacks
        for (int i = 0; i < mouse_callback_count; i++) {
            if (mouse_callbacks[i]) {
                mouse_callbacks[i](&event);
            }
        }
        
        // Reset packet index
        mouse_packet_index = 0;
    }
}

// Register a mouse callback
void mouse_register_callback(mouse_callback_t callback) {
    if (mouse_callback_count < MAX_MOUSE_CALLBACKS) {
        mouse_callbacks[mouse_callback_count++] = callback;
    }
}

// Unregister a mouse callback
void mouse_unregister_callback(mouse_callback_t callback) {
    for (int i = 0; i < mouse_callback_count; i++) {
        if (mouse_callbacks[i] == callback) {
            // Remove the callback by shifting all subsequent callbacks
            for (int j = i; j < mouse_callback_count - 1; j++) {
                mouse_callbacks[j] = mouse_callbacks[j + 1];
            }
            mouse_callbacks[--mouse_callback_count] = NULL;
            break;
        }
    }
}

// Get the current mouse X position
int mouse_get_x() {
    return mouse_x;
}

// Get the current mouse Y position
int mouse_get_y() {
    return mouse_y;
}

// Get the current mouse button state
int mouse_get_buttons() {
    return mouse_buttons;
}

// Set the mouse position
void mouse_set_position(int x, int y) {
    // Apply bounds
    if (x < mouse_min_x) x = mouse_min_x;
    if (y < mouse_min_y) y = mouse_min_y;
    if (x > mouse_max_x) x = mouse_max_x;
    if (y > mouse_max_y) y = mouse_max_y;
    
    mouse_x = x;
    mouse_y = y;
}

// Set the mouse bounds
void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y) {
    mouse_min_x = min_x;
    mouse_min_y = min_y;
    mouse_max_x = max_x;
    mouse_max_y = max_y;
    
    // Ensure the mouse is within the new bounds
    if (mouse_x < min_x) mouse_x = min_x;
    if (mouse_y < min_y) mouse_y = min_y;
    if (mouse_x > max_x) mouse_x = max_x;
    if (mouse_y > max_y) mouse_y = max_y;
}

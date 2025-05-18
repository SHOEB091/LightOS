/**
 * LightOS Drivers
 * Keyboard driver implementation
 */

#include "keyboard.h"
#include "../kernel/kernel.h"

// Keyboard buffer
#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_start = 0;
static int buffer_end = 0;
static int buffer_count = 0;

// Keyboard state
static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;
static int caps_lock = 0;
static int num_lock = 1;
static int scroll_lock = 0;

// US keyboard layout
static const char keymap_lower[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const char keymap_upper[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Initialize the keyboard
void keyboard_init() {
    // Register keyboard interrupt handler
    // This would be implemented in a real system
    
    // Clear the keyboard buffer
    buffer_start = 0;
    buffer_end = 0;
    buffer_count = 0;
    
    // Reset keyboard state
    shift_pressed = 0;
    ctrl_pressed = 0;
    alt_pressed = 0;
    caps_lock = 0;
    num_lock = 1;
    scroll_lock = 0;
}

// Handle a keyboard interrupt
void keyboard_handler(unsigned char scancode) {
    // Check if this is a key release (bit 7 set)
    if (scancode & 0x80) {
        // Key release
        scancode &= 0x7F; // Clear the release bit
        
        // Update modifier key state
        switch (scancode) {
            case 0x2A: // Left shift
            case 0x36: // Right shift
                shift_pressed = 0;
                break;
            case 0x1D: // Control
                ctrl_pressed = 0;
                break;
            case 0x38: // Alt
                alt_pressed = 0;
                break;
        }
    } else {
        // Key press
        
        // Update modifier key state
        switch (scancode) {
            case 0x2A: // Left shift
            case 0x36: // Right shift
                shift_pressed = 1;
                break;
            case 0x1D: // Control
                ctrl_pressed = 1;
                break;
            case 0x38: // Alt
                alt_pressed = 1;
                break;
            case 0x3A: // Caps Lock
                caps_lock = !caps_lock;
                break;
            case 0x45: // Num Lock
                num_lock = !num_lock;
                break;
            case 0x46: // Scroll Lock
                scroll_lock = !scroll_lock;
                break;
            default:
                // Regular key
                if (scancode < 128) {
                    // Determine the character based on shift state and caps lock
                    int uppercase = (shift_pressed && !caps_lock) || (!shift_pressed && caps_lock);
                    char c = uppercase ? keymap_upper[scancode] : keymap_lower[scancode];
                    
                    // Add the character to the buffer if it's printable
                    if (c) {
                        keyboard_buffer_put(c);
                    }
                }
                break;
        }
    }
}

// Add a character to the keyboard buffer
void keyboard_buffer_put(char c) {
    if (buffer_count < KEYBOARD_BUFFER_SIZE) {
        keyboard_buffer[buffer_end] = c;
        buffer_end = (buffer_end + 1) % KEYBOARD_BUFFER_SIZE;
        buffer_count++;
    }
}

// Get a character from the keyboard buffer
char keyboard_buffer_get() {
    if (buffer_count > 0) {
        char c = keyboard_buffer[buffer_start];
        buffer_start = (buffer_start + 1) % KEYBOARD_BUFFER_SIZE;
        buffer_count--;
        return c;
    }
    
    return 0; // No character available
}

// Check if there are characters in the keyboard buffer
int keyboard_buffer_available() {
    return buffer_count;
}

// Read a character from the keyboard (blocking)
char keyboard_read() {
    // Wait for a character to be available
    while (!keyboard_buffer_available()) {
        // In a real system, we would yield the CPU here
    }
    
    return keyboard_buffer_get();
}

// Read a line from the keyboard
void keyboard_read_line(char* buffer, int max_length) {
    int i = 0;
    char c;
    
    while (i < max_length - 1) {
        c = keyboard_read();
        
        if (c == '\n') {
            // End of line
            buffer[i] = '\0';
            return;
        } else if (c == '\b') {
            // Backspace
            if (i > 0) {
                i--;
                // In a real system, we would also handle the display
            }
        } else {
            // Regular character
            buffer[i++] = c;
            // In a real system, we would also echo the character
        }
    }
    
    // Buffer full
    buffer[i] = '\0';
}

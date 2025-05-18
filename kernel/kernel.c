/**
 * LightOS Kernel
 * Main kernel file for our Linux-based operating system
 *
 * This is a customized implementation based on the Linux kernel,
 * optimized for performance and resource efficiency.
 */

#include "kernel.h"
#include "../init/init.h"

// Video memory address (standard VGA text mode)
#define VIDEO_MEMORY 0xB8000
// VGA text mode dimensions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Text colors in VGA mode
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Current cursor position
static int cursor_x = 0;
static int cursor_y = 0;
// Current text color
static unsigned char current_color = 0;

// Create a VGA color byte from foreground and background colors
static inline unsigned char vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Create a VGA character entry (character + color)
static inline unsigned short vga_entry(unsigned char c, unsigned char color) {
    return (unsigned short) c | (unsigned short) color << 8;
}

// Initialize the terminal
void terminal_initialize() {
    // Set default color (light grey on black)
    current_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    // Clear the screen
    unsigned short* video_memory = (unsigned short*) VIDEO_MEMORY;
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
            video_memory[index] = vga_entry(' ', current_color);
        }
    }

    // Reset cursor position
    cursor_x = 0;
    cursor_y = 0;
}

// Set the text color
void terminal_set_color(enum vga_color fg, enum vga_color bg) {
    current_color = vga_entry_color(fg, bg);
}

// Put a character at a specific position
void terminal_put_char_at(char c, unsigned char color, int x, int y) {
    const int index = y * VGA_WIDTH + x;
    unsigned short* video_memory = (unsigned short*) VIDEO_MEMORY;
    video_memory[index] = vga_entry(c, color);
}

// Scroll the terminal up one line
void terminal_scroll() {
    unsigned short* video_memory = (unsigned short*) VIDEO_MEMORY;

    // Move each line up one position
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int current_index = y * VGA_WIDTH + x;
            const int next_index = (y + 1) * VGA_WIDTH + x;
            video_memory[current_index] = video_memory[next_index];
        }
    }

    // Clear the last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        const int index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        video_memory[index] = vga_entry(' ', current_color);
    }
}

// Put a character at the current cursor position
void terminal_put_char(char c) {
    // Handle special characters
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        // Tab is 4 spaces
        for (int i = 0; i < 4; i++) {
            terminal_put_char(' ');
        }
    } else {
        // Regular character
        terminal_put_char_at(c, current_color, cursor_x, cursor_y);
        cursor_x++;
    }

    // Handle line wrapping
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // Handle scrolling
    if (cursor_y >= VGA_HEIGHT) {
        terminal_scroll();
        cursor_y = VGA_HEIGHT - 1;
    }
}

// Write a string to the terminal
void terminal_write(const char* data) {
    for (int i = 0; data[i] != '\0'; i++) {
        terminal_put_char(data[i]);
    }
}

// Write a string with a specific color
void terminal_write_color(const char* data, enum vga_color fg, enum vga_color bg) {
    unsigned char old_color = current_color;
    terminal_set_color(fg, bg);
    terminal_write(data);
    current_color = old_color;
}

// Clear the terminal
void terminal_clear() {
    terminal_initialize();
}

// Main kernel function
void kernel_main() {
    // Initialize terminal
    terminal_initialize();

    // Display welcome message
    terminal_write_color("LightOS Kernel\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_write_color("----------------\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_write("\n");
    terminal_write("Welcome to LightOS - A fast, resource-efficient Linux-based OS\n");
    terminal_write("\n");
    terminal_write("System initializing...\n");

    // Initialize the system
    init_system();

    // We should never reach here, as init_system() starts the CLI
    // which has its own main loop

    // Enter kernel main loop as a fallback
    while (1) {
        // This is where we would handle interrupts, schedule processes, etc.
    }
}

/**
 * LightOS Kernel Header
 * Main kernel header file for our custom operating system
 */

#ifndef KERNEL_H
#define KERNEL_H

// VGA color enumeration
enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE,
};

// Terminal functions
void terminal_initialize();
void terminal_set_color(enum vga_color fg, enum vga_color bg);
void terminal_put_char_at(char c, unsigned char color, int x, int y);
void terminal_put_char(char c);
void terminal_write(const char* data);
void terminal_write_color(const char* data, enum vga_color fg, enum vga_color bg);
void terminal_clear();

// Kernel main function
void kernel_main();

#endif /* KERNEL_H */

/**
 * LightOS Drivers
 * Keyboard driver header
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

// Keyboard functions
void keyboard_init();
void keyboard_handler(unsigned char scancode);
void keyboard_buffer_put(char c);
char keyboard_buffer_get();
int keyboard_buffer_available();
char keyboard_read();
void keyboard_read_line(char* buffer, int max_length);

#endif /* KEYBOARD_H */

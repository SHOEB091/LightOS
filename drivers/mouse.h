/**
 * LightOS Drivers
 * Mouse driver header
 */

#ifndef MOUSE_H
#define MOUSE_H

// Mouse button states
#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_RIGHT_BUTTON  0x02
#define MOUSE_MIDDLE_BUTTON 0x04

// Mouse event structure
typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int buttons;
    int wheel_delta;
} mouse_event_t;

// Mouse callback function type
typedef void (*mouse_callback_t)(mouse_event_t* event);

// Mouse functions
void mouse_init();
void mouse_handler(unsigned char data);
void mouse_register_callback(mouse_callback_t callback);
void mouse_unregister_callback(mouse_callback_t callback);
int mouse_get_x();
int mouse_get_y();
int mouse_get_buttons();
void mouse_set_position(int x, int y);
void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y);

#endif /* MOUSE_H */

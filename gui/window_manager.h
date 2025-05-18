/**
 * LightOS GUI
 * Window Manager header
 */

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

// Window flags
#define WINDOW_FLAG_VISIBLE      0x01
#define WINDOW_FLAG_RESIZABLE    0x02
#define WINDOW_FLAG_MOVABLE      0x04
#define WINDOW_FLAG_MINIMIZABLE  0x08
#define WINDOW_FLAG_MAXIMIZABLE  0x10
#define WINDOW_FLAG_CLOSABLE     0x20
#define WINDOW_FLAG_BORDER       0x40
#define WINDOW_FLAG_TITLEBAR     0x80

// Window state
#define WINDOW_STATE_NORMAL      0
#define WINDOW_STATE_MINIMIZED   1
#define WINDOW_STATE_MAXIMIZED   2

// Window event types
#define WINDOW_EVENT_PAINT       0
#define WINDOW_EVENT_RESIZE      1
#define WINDOW_EVENT_MOVE        2
#define WINDOW_EVENT_CLOSE       3
#define WINDOW_EVENT_MINIMIZE    4
#define WINDOW_EVENT_MAXIMIZE    5
#define WINDOW_EVENT_RESTORE     6
#define WINDOW_EVENT_FOCUS       7
#define WINDOW_EVENT_BLUR        8
#define WINDOW_EVENT_MOUSE_MOVE  9
#define WINDOW_EVENT_MOUSE_DOWN  10
#define WINDOW_EVENT_MOUSE_UP    11
#define WINDOW_EVENT_KEY_DOWN    12
#define WINDOW_EVENT_KEY_UP      13

// Window event structure
typedef struct {
    int type;
    int window_id;
    union {
        struct {
            int x;
            int y;
            int width;
            int height;
        } resize;
        struct {
            int x;
            int y;
        } move;
        struct {
            int x;
            int y;
            int buttons;
        } mouse;
        struct {
            int key_code;
            int modifiers;
        } key;
    } data;
} window_event_t;

// Window structure
typedef struct {
    int id;
    char title[64];
    int x;
    int y;
    int width;
    int height;
    unsigned int flags;
    int state;
    unsigned int background_color;
    void* buffer;
    int buffer_width;
    int buffer_height;
    int (*event_handler)(window_event_t* event);
    void* user_data;
} window_t;

// Window manager functions
void window_manager_init();
int window_create(const char* title, int x, int y, int width, int height, unsigned int flags);
int window_destroy(int window_id);
int window_show(int window_id);
int window_hide(int window_id);
int window_move(int window_id, int x, int y);
int window_resize(int window_id, int width, int height);
int window_set_title(int window_id, const char* title);
int window_set_background_color(int window_id, unsigned int color);
int window_set_event_handler(int window_id, int (*event_handler)(window_event_t* event));
int window_set_user_data(int window_id, void* user_data);
int window_invalidate(int window_id);
int window_invalidate_rect(int window_id, int x, int y, int width, int height);
int window_draw_pixel(int window_id, int x, int y, unsigned int color);
int window_draw_line(int window_id, int x1, int y1, int x2, int y2, unsigned int color);
int window_draw_rect(int window_id, int x, int y, int width, int height, unsigned int color);
int window_fill_rect(int window_id, int x, int y, int width, int height, unsigned int color);
int window_draw_text(int window_id, int x, int y, const char* text, unsigned int color);
int window_draw_image(int window_id, int x, int y, int width, int height, const void* image_data);
window_t* window_get(int window_id);
int window_get_count();
void window_manager_process_events();
void window_manager_update();

#endif /* WINDOW_MANAGER_H */

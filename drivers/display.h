/**
 * LightOS Drivers
 * Display driver header
 */

#ifndef DISPLAY_H
#define DISPLAY_H

// Display mode structure
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned int refresh_rate;
} display_mode_t;

// Display device structure
typedef struct {
    char name[32];
    display_mode_t current_mode;
    display_mode_t* supported_modes;
    unsigned int supported_mode_count;
    void* framebuffer;
    unsigned int framebuffer_size;
    unsigned int pitch;
    
    // Function pointers for device operations
    int (*init)(struct display_device* dev);
    int (*close)(struct display_device* dev);
    int (*set_mode)(struct display_device* dev, display_mode_t* mode);
    int (*get_mode)(struct display_device* dev, display_mode_t* mode);
    int (*clear)(struct display_device* dev, unsigned int color);
    int (*update)(struct display_device* dev, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    int (*set_brightness)(struct display_device* dev, unsigned int brightness);
    int (*get_brightness)(struct display_device* dev);
    int (*set_contrast)(struct display_device* dev, unsigned int contrast);
    int (*get_contrast)(struct display_device* dev);
    
    // Private data for the driver
    void* private_data;
} display_device_t;

// Display driver functions
void display_init();
int display_register_device(display_device_t* device);
int display_unregister_device(const char* name);
display_device_t* display_get_device(const char* name);
int display_set_mode(const char* device_name, display_mode_t* mode);
int display_get_mode(const char* device_name, display_mode_t* mode);
int display_clear(const char* device_name, unsigned int color);
int display_update(const char* device_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
int display_set_brightness(const char* device_name, unsigned int brightness);
int display_get_brightness(const char* device_name);
int display_set_contrast(const char* device_name, unsigned int contrast);
int display_get_contrast(const char* device_name);
void display_list_devices();
void display_list_modes(const char* device_name);

// Drawing functions
int display_draw_pixel(const char* device_name, unsigned int x, unsigned int y, unsigned int color);
int display_draw_line(const char* device_name, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
int display_draw_rect(const char* device_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);
int display_fill_rect(const char* device_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);
int display_draw_circle(const char* device_name, unsigned int x, unsigned int y, unsigned int radius, unsigned int color);
int display_fill_circle(const char* device_name, unsigned int x, unsigned int y, unsigned int radius, unsigned int color);
int display_draw_text(const char* device_name, unsigned int x, unsigned int y, const char* text, unsigned int color);
int display_draw_image(const char* device_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* image_data);

// Specific display device detection
int detect_display_devices();

#endif /* DISPLAY_H */

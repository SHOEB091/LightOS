# LightOS GUI

## Overview

LightOS provides a comprehensive graphical user interface (GUI) system with a window manager. The GUI system is designed to be lightweight, fast, and customizable.

## GUI Architecture

The LightOS GUI system follows a layered architecture:

1. **Display Driver Layer**: Interfaces with the hardware to draw pixels on the screen.
2. **Graphics Primitives Layer**: Provides basic drawing functions (lines, rectangles, text, etc.).
3. **Window Manager Layer**: Manages windows, dialogs, and other UI elements.
4. **Widget Layer**: Provides reusable UI components (buttons, text fields, etc.).
5. **Application Layer**: User applications that use the GUI system.

## Display Modes

LightOS supports two display modes:

1. **Text Mode**: A character-based display mode with limited graphics capabilities.
2. **Graphics Mode**: A pixel-based display mode with full graphics capabilities.

### API Reference

```c
void gui_init();
void gui_switch_mode();
int gui_get_mode();
```

Initializes the GUI system, switches between text and graphics modes, or gets the current mode.

## Graphics Primitives

### Key Features

- **Pixel Drawing**: Draws individual pixels.
- **Line Drawing**: Draws lines between two points.
- **Rectangle Drawing**: Draws filled or outlined rectangles.
- **Text Drawing**: Draws text with various fonts and styles.
- **Image Drawing**: Draws images from various formats.

### API Reference

```c
void gui_clear_screen(unsigned int color);
void gui_set_pixel(int x, int y, unsigned int color);
void gui_draw_line(int x1, int y1, int x2, int y2, unsigned int color);
void gui_draw_rect(int x, int y, int width, int height, unsigned int color);
void gui_fill_rect(int x, int y, int width, int height, unsigned int color);
void gui_draw_char(int x, int y, char c, unsigned int color);
void gui_draw_string(int x, int y, const char* str, unsigned int color);
```

Clears the screen, sets a pixel, draws a line, draws a rectangle, fills a rectangle, draws a character, or draws a string.

## Window Manager

### Key Features

- **Window Creation**: Creates windows with various properties.
- **Window Destruction**: Destroys windows.
- **Window Manipulation**: Moves, resizes, minimizes, maximizes, and restores windows.
- **Window Decoration**: Adds borders, title bars, and buttons to windows.
- **Window Events**: Handles window events (mouse, keyboard, etc.).
- **Window Rendering**: Renders window contents.

### API Reference

#### Window Management

```c
int window_create(const char* title, int x, int y, int width, int height, unsigned int flags);
int window_destroy(int window_id);
int window_show(int window_id);
int window_hide(int window_id);
int window_move(int window_id, int x, int y);
int window_resize(int window_id, int width, int height);
int window_set_title(int window_id, const char* title);
int window_set_background_color(int window_id, unsigned int color);
```

Creates, destroys, shows, hides, moves, resizes, or sets properties of a window.

#### Event Handling

```c
int window_set_event_handler(int window_id, int (*event_handler)(window_event_t* event));
```

Sets an event handler for a window.

#### Drawing

```c
int window_invalidate(int window_id);
int window_invalidate_rect(int window_id, int x, int y, int width, int height);
int window_draw_pixel(int window_id, int x, int y, unsigned int color);
int window_draw_line(int window_id, int x1, int y1, int x2, int y2, unsigned int color);
int window_draw_rect(int window_id, int x, int y, int width, int height, unsigned int color);
int window_fill_rect(int window_id, int x, int y, int width, int height, unsigned int color);
int window_draw_text(int window_id, int x, int y, const char* text, unsigned int color);
int window_draw_image(int window_id, int x, int y, int width, int height, const void* image_data);
```

Invalidates a window or rectangle, or draws various primitives in a window.

#### Window Information

```c
window_t* window_get(int window_id);
int window_get_count();
```

Gets a window by ID or gets the number of windows.

#### Window Manager Control

```c
void window_manager_init();
void window_manager_process_events();
void window_manager_update();
```

Initializes the window manager, processes events, or updates the display.

## Widgets

LightOS provides a set of reusable UI components (widgets) that can be used to build user interfaces.

### Button

A clickable button that triggers an action when clicked.

#### API Reference

```c
int button_create(int window_id, int x, int y, int width, int height, const char* text);
int button_destroy(int button_id);
int button_set_text(int button_id, const char* text);
int button_set_callback(int button_id, int (*callback)(int button_id));
```

Creates, destroys, or sets properties of a button.

### Text Field

A field for entering text.

#### API Reference

```c
int text_field_create(int window_id, int x, int y, int width, int height);
int text_field_destroy(int text_field_id);
int text_field_set_text(int text_field_id, const char* text);
const char* text_field_get_text(int text_field_id);
int text_field_set_callback(int text_field_id, int (*callback)(int text_field_id));
```

Creates, destroys, or sets properties of a text field.

### Label

A non-interactive text display.

#### API Reference

```c
int label_create(int window_id, int x, int y, int width, int height, const char* text);
int label_destroy(int label_id);
int label_set_text(int label_id, const char* text);
```

Creates, destroys, or sets properties of a label.

### Checkbox

A toggleable checkbox.

#### API Reference

```c
int checkbox_create(int window_id, int x, int y, int width, int height, const char* text, int checked);
int checkbox_destroy(int checkbox_id);
int checkbox_set_text(int checkbox_id, const char* text);
int checkbox_set_checked(int checkbox_id, int checked);
int checkbox_is_checked(int checkbox_id);
int checkbox_set_callback(int checkbox_id, int (*callback)(int checkbox_id, int checked));
```

Creates, destroys, or sets properties of a checkbox.

### Radio Button

A radio button that can be selected from a group.

#### API Reference

```c
int radio_button_create(int window_id, int x, int y, int width, int height, const char* text, int group_id, int selected);
int radio_button_destroy(int radio_button_id);
int radio_button_set_text(int radio_button_id, const char* text);
int radio_button_set_selected(int radio_button_id, int selected);
int radio_button_is_selected(int radio_button_id);
int radio_button_set_callback(int radio_button_id, int (*callback)(int radio_button_id, int selected));
```

Creates, destroys, or sets properties of a radio button.

### Dropdown

A dropdown menu for selecting from a list of options.

#### API Reference

```c
int dropdown_create(int window_id, int x, int y, int width, int height);
int dropdown_destroy(int dropdown_id);
int dropdown_add_item(int dropdown_id, const char* text);
int dropdown_remove_item(int dropdown_id, int item_index);
int dropdown_set_selected_item(int dropdown_id, int item_index);
int dropdown_get_selected_item(int dropdown_id);
const char* dropdown_get_selected_text(int dropdown_id);
int dropdown_set_callback(int dropdown_id, int (*callback)(int dropdown_id, int item_index));
```

Creates, destroys, or sets properties of a dropdown.

### List Box

A scrollable list of items.

#### API Reference

```c
int list_box_create(int window_id, int x, int y, int width, int height);
int list_box_destroy(int list_box_id);
int list_box_add_item(int list_box_id, const char* text);
int list_box_remove_item(int list_box_id, int item_index);
int list_box_set_selected_item(int list_box_id, int item_index);
int list_box_get_selected_item(int list_box_id);
const char* list_box_get_selected_text(int list_box_id);
int list_box_set_callback(int list_box_id, int (*callback)(int list_box_id, int item_index));
```

Creates, destroys, or sets properties of a list box.

### Progress Bar

A bar that shows progress.

#### API Reference

```c
int progress_bar_create(int window_id, int x, int y, int width, int height);
int progress_bar_destroy(int progress_bar_id);
int progress_bar_set_progress(int progress_bar_id, int progress);
int progress_bar_get_progress(int progress_bar_id);
```

Creates, destroys, or sets properties of a progress bar.

### Slider

A slider for selecting a value from a range.

#### API Reference

```c
int slider_create(int window_id, int x, int y, int width, int height, int min_value, int max_value, int value);
int slider_destroy(int slider_id);
int slider_set_value(int slider_id, int value);
int slider_get_value(int slider_id);
int slider_set_callback(int slider_id, int (*callback)(int slider_id, int value));
```

Creates, destroys, or sets properties of a slider.

## Dialogs

LightOS provides a set of standard dialogs for common tasks.

### Message Dialog

A dialog that displays a message.

#### API Reference

```c
int message_dialog_show(const char* title, const char* message, int buttons);
```

Shows a message dialog.

### Input Dialog

A dialog that prompts for input.

#### API Reference

```c
int input_dialog_show(const char* title, const char* message, char* input, int max_length);
```

Shows an input dialog.

### File Dialog

A dialog for selecting a file.

#### API Reference

```c
int file_dialog_show(const char* title, const char* initial_dir, char* selected_file, int max_length, int save);
```

Shows a file dialog.

### Color Dialog

A dialog for selecting a color.

#### API Reference

```c
int color_dialog_show(const char* title, unsigned int* color);
```

Shows a color dialog.

## Themes

LightOS supports themes for customizing the appearance of the GUI.

### API Reference

```c
void theme_init();
int theme_load(const char* theme_file);
int theme_save(const char* theme_file);
int theme_set_color(const char* element, unsigned int color);
unsigned int theme_get_color(const char* element);
```

Initializes the theme system, loads or saves a theme, or sets or gets a theme color.

## Writing Custom Widgets

### Widget Structure

```c
typedef struct {
    int id;
    int window_id;
    int x;
    int y;
    int width;
    int height;
    unsigned int flags;
    void* data;
    int (*event_handler)(struct widget* widget, window_event_t* event);
    void (*draw)(struct widget* widget);
    void (*destroy)(struct widget* widget);
} widget_t;
```

### Widget Implementation Steps

1. **Create a Widget Structure**: Define a widget structure with the necessary information.
2. **Implement Widget Functions**: Implement the required widget functions (event_handler, draw, destroy).
3. **Register the Widget**: Register the widget with the window manager.
4. **Handle Widget Events**: Handle widget events such as mouse clicks and keyboard input.

### Example Widget

```c
// Example widget for a simple button
widget_t* button_create(int window_id, int x, int y, int width, int height, const char* text) {
    // Allocate memory for the widget
    widget_t* widget = (widget_t*)allocate_block();
    
    if (!widget) {
        terminal_write("Failed to allocate memory for button widget\n");
        return NULL;
    }
    
    // Initialize the widget
    widget->id = next_widget_id++;
    widget->window_id = window_id;
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
    widget->flags = WIDGET_FLAG_VISIBLE | WIDGET_FLAG_ENABLED;
    
    // Allocate memory for the button data
    button_data_t* data = (button_data_t*)allocate_block();
    
    if (!data) {
        terminal_write("Failed to allocate memory for button data\n");
        free_block(widget);
        return NULL;
    }
    
    // Initialize the button data
    strcpy(data->text, text);
    data->callback = NULL;
    
    widget->data = data;
    widget->event_handler = button_event_handler;
    widget->draw = button_draw;
    widget->destroy = button_destroy;
    
    // Register the widget with the window manager
    window_manager_register_widget(widget);
    
    return widget;
}
```

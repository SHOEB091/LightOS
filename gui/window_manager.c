/**
 * LightOS GUI
 * Window Manager implementation
 */

#include "window_manager.h"
#include "gui.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "../drivers/mouse.h"
#include "../drivers/keyboard.h"

// Maximum number of windows
#define MAX_WINDOWS 64

// Window array
static window_t windows[MAX_WINDOWS];
static int window_count = 0;

// Next window ID
static int next_window_id = 1;

// Active window ID
static int active_window_id = 0;

// Mouse state
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_buttons = 0;

// Window being dragged
static int drag_window_id = 0;
static int drag_offset_x = 0;
static int drag_offset_y = 0;

// Window being resized
static int resize_window_id = 0;
static int resize_edge = 0; // 1 = left, 2 = right, 3 = top, 4 = bottom, 5 = top-left, 6 = top-right, 7 = bottom-left, 8 = bottom-right

// Default window colors
#define WINDOW_COLOR_BACKGROUND  0xFFFFFF
#define WINDOW_COLOR_BORDER      0x000000
#define WINDOW_COLOR_TITLEBAR    0x0000AA
#define WINDOW_COLOR_TITLETEXT   0xFFFFFF
#define WINDOW_COLOR_BUTTON      0xCCCCCC
#define WINDOW_COLOR_BUTTON_TEXT 0x000000

// Initialize the window manager
void window_manager_init() {
    terminal_write("Initializing window manager...\n");
    
    // Clear the window array
    for (int i = 0; i < MAX_WINDOWS; i++) {
        windows[i].id = 0;
    }
    
    window_count = 0;
    next_window_id = 1;
    active_window_id = 0;
    
    // Register mouse callback
    mouse_register_callback(window_manager_mouse_callback);
    
    terminal_write("Window manager initialized\n");
}

// Mouse callback function
static void window_manager_mouse_callback(mouse_event_t* event) {
    mouse_x = event->x;
    mouse_y = event->y;
    mouse_buttons = event->buttons;
    
    // Handle window dragging
    if (drag_window_id != 0) {
        if (mouse_buttons & MOUSE_LEFT_BUTTON) {
            // Continue dragging
            window_move(drag_window_id, mouse_x - drag_offset_x, mouse_y - drag_offset_y);
        } else {
            // Stop dragging
            drag_window_id = 0;
        }
    }
    
    // Handle window resizing
    if (resize_window_id != 0) {
        if (mouse_buttons & MOUSE_LEFT_BUTTON) {
            // Continue resizing
            window_t* window = window_get(resize_window_id);
            
            if (window) {
                int new_x = window->x;
                int new_y = window->y;
                int new_width = window->width;
                int new_height = window->height;
                
                switch (resize_edge) {
                    case 1: // Left
                        new_x = mouse_x;
                        new_width = window->x + window->width - mouse_x;
                        break;
                    case 2: // Right
                        new_width = mouse_x - window->x;
                        break;
                    case 3: // Top
                        new_y = mouse_y;
                        new_height = window->y + window->height - mouse_y;
                        break;
                    case 4: // Bottom
                        new_height = mouse_y - window->y;
                        break;
                    case 5: // Top-left
                        new_x = mouse_x;
                        new_y = mouse_y;
                        new_width = window->x + window->width - mouse_x;
                        new_height = window->y + window->height - mouse_y;
                        break;
                    case 6: // Top-right
                        new_y = mouse_y;
                        new_width = mouse_x - window->x;
                        new_height = window->y + window->height - mouse_y;
                        break;
                    case 7: // Bottom-left
                        new_x = mouse_x;
                        new_width = window->x + window->width - mouse_x;
                        new_height = mouse_y - window->y;
                        break;
                    case 8: // Bottom-right
                        new_width = mouse_x - window->x;
                        new_height = mouse_y - window->y;
                        break;
                }
                
                // Enforce minimum size
                if (new_width < 50) new_width = 50;
                if (new_height < 50) new_height = 50;
                
                // Apply the new position and size
                if (new_x != window->x || new_y != window->y) {
                    window_move(resize_window_id, new_x, new_y);
                }
                
                if (new_width != window->width || new_height != window->height) {
                    window_resize(resize_window_id, new_width, new_height);
                }
            }
        } else {
            // Stop resizing
            resize_window_id = 0;
        }
    }
    
    // Handle mouse events for windows
    if (drag_window_id == 0 && resize_window_id == 0) {
        // Find the window under the mouse
        int window_id = window_manager_find_window_at(mouse_x, mouse_y);
        
        if (window_id != 0) {
            window_t* window = window_get(window_id);
            
            if (window) {
                // Check if the mouse is on the title bar
                if ((window->flags & WINDOW_FLAG_TITLEBAR) && 
                    mouse_y >= window->y && mouse_y < window->y + 20 &&
                    mouse_x >= window->x && mouse_x < window->x + window->width) {
                    
                    // Check if the mouse is on the close button
                    if ((window->flags & WINDOW_FLAG_CLOSABLE) &&
                        mouse_x >= window->x + window->width - 20 && mouse_x < window->x + window->width) {
                        
                        if (mouse_buttons & MOUSE_LEFT_BUTTON) {
                            // Close the window
                            window_destroy(window_id);
                            return;
                        }
                    }
                    
                    // Check if the mouse is on the maximize button
                    if ((window->flags & WINDOW_FLAG_MAXIMIZABLE) &&
                        mouse_x >= window->x + window->width - 40 && mouse_x < window->x + window->width - 20) {
                        
                        if (mouse_buttons & MOUSE_LEFT_BUTTON) {
                            // Toggle maximize
                            if (window->state == WINDOW_STATE_MAXIMIZED) {
                                // Restore
                                window_event_t event;
                                event.type = WINDOW_EVENT_RESTORE;
                                event.window_id = window_id;
                                
                                if (window->event_handler) {
                                    window->event_handler(&event);
                                }
                                
                                window->state = WINDOW_STATE_NORMAL;
                            } else {
                                // Maximize
                                window_event_t event;
                                event.type = WINDOW_EVENT_MAXIMIZE;
                                event.window_id = window_id;
                                
                                if (window->event_handler) {
                                    window->event_handler(&event);
                                }
                                
                                window->state = WINDOW_STATE_MAXIMIZED;
                            }
                            
                            return;
                        }
                    }
                    
                    // Check if the mouse is on the minimize button
                    if ((window->flags & WINDOW_FLAG_MINIMIZABLE) &&
                        mouse_x >= window->x + window->width - 60 && mouse_x < window->x + window->width - 40) {
                        
                        if (mouse_buttons & MOUSE_LEFT_BUTTON) {
                            // Minimize
                            window_event_t event;
                            event.type = WINDOW_EVENT_MINIMIZE;
                            event.window_id = window_id;
                            
                            if (window->event_handler) {
                                window->event_handler(&event);
                            }
                            
                            window->state = WINDOW_STATE_MINIMIZED;
                            return;
                        }
                    }
                    
                    // Start dragging if the window is movable
                    if ((window->flags & WINDOW_FLAG_MOVABLE) && (mouse_buttons & MOUSE_LEFT_BUTTON)) {
                        drag_window_id = window_id;
                        drag_offset_x = mouse_x - window->x;
                        drag_offset_y = mouse_y - window->y;
                        
                        // Bring the window to the front
                        window_manager_bring_to_front(window_id);
                        return;
                    }
                }
                
                // Check if the mouse is on the window border for resizing
                if ((window->flags & WINDOW_FLAG_RESIZABLE) && (mouse_buttons & MOUSE_LEFT_BUTTON)) {
                    int border_size = 5;
                    
                    // Left border
                    if (mouse_x >= window->x && mouse_x < window->x + border_size) {
                        // Top-left corner
                        if (mouse_y >= window->y && mouse_y < window->y + border_size) {
                            resize_window_id = window_id;
                            resize_edge = 5; // Top-left
                            return;
                        }
                        
                        // Bottom-left corner
                        if (mouse_y >= window->y + window->height - border_size && mouse_y < window->y + window->height) {
                            resize_window_id = window_id;
                            resize_edge = 7; // Bottom-left
                            return;
                        }
                        
                        // Left edge
                        if (mouse_y >= window->y + border_size && mouse_y < window->y + window->height - border_size) {
                            resize_window_id = window_id;
                            resize_edge = 1; // Left
                            return;
                        }
                    }
                    
                    // Right border
                    if (mouse_x >= window->x + window->width - border_size && mouse_x < window->x + window->width) {
                        // Top-right corner
                        if (mouse_y >= window->y && mouse_y < window->y + border_size) {
                            resize_window_id = window_id;
                            resize_edge = 6; // Top-right
                            return;
                        }
                        
                        // Bottom-right corner
                        if (mouse_y >= window->y + window->height - border_size && mouse_y < window->y + window->height) {
                            resize_window_id = window_id;
                            resize_edge = 8; // Bottom-right
                            return;
                        }
                        
                        // Right edge
                        if (mouse_y >= window->y + border_size && mouse_y < window->y + window->height - border_size) {
                            resize_window_id = window_id;
                            resize_edge = 2; // Right
                            return;
                        }
                    }
                    
                    // Top border
                    if (mouse_y >= window->y && mouse_y < window->y + border_size &&
                        mouse_x >= window->x + border_size && mouse_x < window->x + window->width - border_size) {
                        resize_window_id = window_id;
                        resize_edge = 3; // Top
                        return;
                    }
                    
                    // Bottom border
                    if (mouse_y >= window->y + window->height - border_size && mouse_y < window->y + window->height &&
                        mouse_x >= window->x + border_size && mouse_x < window->x + window->width - border_size) {
                        resize_window_id = window_id;
                        resize_edge = 4; // Bottom
                        return;
                    }
                }
                
                // Send mouse event to the window
                window_event_t event;
                
                if (mouse_buttons & MOUSE_LEFT_BUTTON) {
                    event.type = WINDOW_EVENT_MOUSE_DOWN;
                } else {
                    event.type = WINDOW_EVENT_MOUSE_MOVE;
                }
                
                event.window_id = window_id;
                event.data.mouse.x = mouse_x - window->x;
                event.data.mouse.y = mouse_y - window->y;
                event.data.mouse.buttons = mouse_buttons;
                
                if (window->event_handler) {
                    window->event_handler(&event);
                }
                
                // Bring the window to the front if clicked
                if (mouse_buttons & MOUSE_LEFT_BUTTON) {
                    window_manager_bring_to_front(window_id);
                }
            }
        }
    }
}

// Find the window at a given position
static int window_manager_find_window_at(int x, int y) {
    // Search from front to back
    for (int i = 0; i < window_count; i++) {
        window_t* window = &windows[i];
        
        if (window->id != 0 && (window->flags & WINDOW_FLAG_VISIBLE) && window->state != WINDOW_STATE_MINIMIZED) {
            if (x >= window->x && x < window->x + window->width &&
                y >= window->y && y < window->y + window->height) {
                return window->id;
            }
        }
    }
    
    return 0;
}

// Bring a window to the front
static void window_manager_bring_to_front(int window_id) {
    // Find the window
    int index = -1;
    
    for (int i = 0; i < window_count; i++) {
        if (windows[i].id == window_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return;
    }
    
    // Already at the front?
    if (index == 0) {
        return;
    }
    
    // Move the window to the front
    window_t temp = windows[index];
    
    for (int i = index; i > 0; i--) {
        windows[i] = windows[i - 1];
    }
    
    windows[0] = temp;
    
    // Set as active window
    active_window_id = window_id;
    
    // Send focus event
    window_event_t event;
    event.type = WINDOW_EVENT_FOCUS;
    event.window_id = window_id;
    
    if (windows[0].event_handler) {
        windows[0].event_handler(&event);
    }
    
    // Redraw all windows
    window_manager_update();
}

// Create a window
int window_create(const char* title, int x, int y, int width, int height, unsigned int flags) {
    // Check if we have room for another window
    if (window_count >= MAX_WINDOWS) {
        terminal_write("Error: Maximum number of windows reached\n");
        return 0;
    }
    
    // Find a free slot
    int index = -1;
    
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i].id == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: No free window slots\n");
        return 0;
    }
    
    // Allocate a buffer for the window
    void* buffer = allocate_blocks((width * height * 4 + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!buffer) {
        terminal_write("Error: Failed to allocate window buffer\n");
        return 0;
    }
    
    // Initialize the window
    windows[index].id = next_window_id++;
    strncpy(windows[index].title, title, 63);
    windows[index].title[63] = '\0';
    windows[index].x = x;
    windows[index].y = y;
    windows[index].width = width;
    windows[index].height = height;
    windows[index].flags = flags;
    windows[index].state = WINDOW_STATE_NORMAL;
    windows[index].background_color = WINDOW_COLOR_BACKGROUND;
    windows[index].buffer = buffer;
    windows[index].buffer_width = width;
    windows[index].buffer_height = height;
    windows[index].event_handler = NULL;
    windows[index].user_data = NULL;
    
    // Clear the window buffer
    memset(buffer, 0, width * height * 4);
    
    window_count++;
    
    // Bring the window to the front
    window_manager_bring_to_front(windows[index].id);
    
    return windows[index].id;
}

// Destroy a window
int window_destroy(int window_id) {
    // Find the window
    int index = -1;
    
    for (int i = 0; i < window_count; i++) {
        if (windows[i].id == window_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: Invalid window ID\n");
        return -1;
    }
    
    // Send close event
    window_event_t event;
    event.type = WINDOW_EVENT_CLOSE;
    event.window_id = window_id;
    
    if (windows[index].event_handler) {
        windows[index].event_handler(&event);
    }
    
    // Free the window buffer
    if (windows[index].buffer) {
        free_blocks(windows[index].buffer, (windows[index].buffer_width * windows[index].buffer_height * 4 + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    // Remove the window
    windows[index].id = 0;
    window_count--;
    
    // If this was the active window, find a new active window
    if (active_window_id == window_id) {
        active_window_id = 0;
        
        if (window_count > 0) {
            active_window_id = windows[0].id;
        }
    }
    
    // Redraw all windows
    window_manager_update();
    
    return 0;
}

/**
 * LightOS GUI
 * GUI header
 */

#ifndef GUI_H
#define GUI_H

// GUI modes
#define GUI_MODE_TEXT     0
#define GUI_MODE_GRAPHICS 1

// GUI functions
void gui_init();
void gui_switch_mode();
int gui_get_mode();
void gui_clear_screen(unsigned int color);
void gui_set_pixel(int x, int y, unsigned int color);
void gui_draw_line(int x1, int y1, int x2, int y2, unsigned int color);
void gui_draw_rect(int x, int y, int width, int height, unsigned int color);
void gui_fill_rect(int x, int y, int width, int height, unsigned int color);
void gui_draw_char(int x, int y, char c, unsigned int color);
void gui_draw_string(int x, int y, const char* str, unsigned int color);
void gui_draw_desktop();

#endif /* GUI_H */

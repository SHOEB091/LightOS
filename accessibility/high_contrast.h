/**
 * LightOS Accessibility
 * High Contrast header
 */

#ifndef HIGH_CONTRAST_H
#define HIGH_CONTRAST_H

// High contrast themes
#define HIGH_CONTRAST_THEME_BLACK_ON_WHITE 0
#define HIGH_CONTRAST_THEME_WHITE_ON_BLACK 1
#define HIGH_CONTRAST_THEME_YELLOW_ON_BLACK 2
#define HIGH_CONTRAST_THEME_WHITE_ON_BLUE 3
#define HIGH_CONTRAST_THEME_GREEN_ON_BLACK 4
#define HIGH_CONTRAST_THEME_CUSTOM 5

// High contrast functions
int high_contrast_init();
int high_contrast_set_theme(unsigned int theme);
int high_contrast_get_theme();
int high_contrast_set_text_size(unsigned int size);
int high_contrast_get_text_size();
int high_contrast_is_enabled();
int high_contrast_enable();
int high_contrast_disable();
int high_contrast_toggle();
int high_contrast_set_foreground_color(unsigned int color);
int high_contrast_get_foreground_color();
int high_contrast_set_background_color(unsigned int color);
int high_contrast_get_background_color();
int high_contrast_set_link_color(unsigned int color);
int high_contrast_get_link_color();
int high_contrast_set_visited_link_color(unsigned int color);
int high_contrast_get_visited_link_color();
int high_contrast_set_active_element_color(unsigned int color);
int high_contrast_get_active_element_color();
int high_contrast_set_selection_color(unsigned int color);
int high_contrast_get_selection_color();
int high_contrast_set_selection_text_color(unsigned int color);
int high_contrast_get_selection_text_color();
int high_contrast_set_button_color(unsigned int color);
int high_contrast_get_button_color();
int high_contrast_set_button_text_color(unsigned int color);
int high_contrast_get_button_text_color();
int high_contrast_set_disabled_element_color(unsigned int color);
int high_contrast_get_disabled_element_color();
int high_contrast_set_disabled_text_color(unsigned int color);
int high_contrast_get_disabled_text_color();
int high_contrast_set_error_color(unsigned int color);
int high_contrast_get_error_color();
int high_contrast_set_warning_color(unsigned int color);
int high_contrast_get_warning_color();
int high_contrast_set_success_color(unsigned int color);
int high_contrast_get_success_color();
int high_contrast_set_info_color(unsigned int color);
int high_contrast_get_info_color();
int high_contrast_set_border_color(unsigned int color);
int high_contrast_get_border_color();
int high_contrast_set_border_width(unsigned int width);
int high_contrast_get_border_width();
int high_contrast_set_focus_outline_color(unsigned int color);
int high_contrast_get_focus_outline_color();
int high_contrast_set_focus_outline_width(unsigned int width);
int high_contrast_get_focus_outline_width();
int high_contrast_set_font_family(const char* font_family);
int high_contrast_get_font_family(char* buffer, unsigned int buffer_size);
int high_contrast_set_font_weight(unsigned int weight);
int high_contrast_get_font_weight();
int high_contrast_set_line_spacing(unsigned int spacing);
int high_contrast_get_line_spacing();
int high_contrast_set_letter_spacing(unsigned int spacing);
int high_contrast_get_letter_spacing();
int high_contrast_set_word_spacing(unsigned int spacing);
int high_contrast_get_word_spacing();
int high_contrast_set_cursor_thickness(unsigned int thickness);
int high_contrast_get_cursor_thickness();
int high_contrast_set_cursor_blink_rate(unsigned int rate);
int high_contrast_get_cursor_blink_rate();
int high_contrast_set_cursor_color(unsigned int color);
int high_contrast_get_cursor_color();
int high_contrast_reset();
int high_contrast_save_settings();
int high_contrast_load_settings();

#endif /* HIGH_CONTRAST_H */

/**
 * LightOS Accessibility
 * Magnifier header
 */

#ifndef MAGNIFIER_H
#define MAGNIFIER_H

// Magnifier functions
int magnifier_init();
int magnifier_zoom_in();
int magnifier_zoom_out();
int magnifier_set_zoom(unsigned int zoom);
int magnifier_get_zoom();
int magnifier_follow_mouse(unsigned int enable);
int magnifier_follow_keyboard(unsigned int enable);
int magnifier_follow_text(unsigned int enable);
int magnifier_is_following_mouse();
int magnifier_is_following_keyboard();
int magnifier_is_following_text();
int magnifier_is_enabled();
int magnifier_enable();
int magnifier_disable();
int magnifier_toggle();
int magnifier_set_position(int x, int y);
int magnifier_get_position(int* x, int* y);
int magnifier_set_size(int width, int height);
int magnifier_get_size(int* width, int* height);
int magnifier_set_color_inversion(unsigned int enable);
int magnifier_is_color_inversion_enabled();
int magnifier_set_lens_mode(unsigned int enable);
int magnifier_is_lens_mode_enabled();
int magnifier_set_fullscreen_mode(unsigned int enable);
int magnifier_is_fullscreen_mode_enabled();
int magnifier_set_docked_mode(unsigned int enable);
int magnifier_is_docked_mode_enabled();
int magnifier_set_dock_position(unsigned int position);
int magnifier_get_dock_position();
int magnifier_set_lens_size(int width, int height);
int magnifier_get_lens_size(int* width, int* height);
int magnifier_set_lens_shape(unsigned int shape);
int magnifier_get_lens_shape();
int magnifier_set_smoothing(unsigned int enable);
int magnifier_is_smoothing_enabled();
int magnifier_set_tracking_speed(unsigned int speed);
int magnifier_get_tracking_speed();
int magnifier_set_focus_tracking(unsigned int enable);
int magnifier_is_focus_tracking_enabled();
int magnifier_set_caret_tracking(unsigned int enable);
int magnifier_is_caret_tracking_enabled();
int magnifier_set_mouse_tracking(unsigned int enable);
int magnifier_is_mouse_tracking_enabled();
int magnifier_set_edge_padding(unsigned int padding);
int magnifier_get_edge_padding();
int magnifier_set_contrast(unsigned int contrast);
int magnifier_get_contrast();
int magnifier_set_brightness(unsigned int brightness);
int magnifier_get_brightness();
int magnifier_set_color_effect(unsigned int effect);
int magnifier_get_color_effect();
int magnifier_set_color_effect_intensity(unsigned int intensity);
int magnifier_get_color_effect_intensity();
int magnifier_set_presentation_mode(unsigned int enable);
int magnifier_is_presentation_mode_enabled();
int magnifier_set_magnification_mode(unsigned int mode);
int magnifier_get_magnification_mode();
int magnifier_set_zoom_increment(unsigned int increment);
int magnifier_get_zoom_increment();
int magnifier_set_max_zoom(unsigned int max_zoom);
int magnifier_get_max_zoom();
int magnifier_set_min_zoom(unsigned int min_zoom);
int magnifier_get_min_zoom();
int magnifier_reset();
int magnifier_save_settings();
int magnifier_load_settings();

#endif /* MAGNIFIER_H */

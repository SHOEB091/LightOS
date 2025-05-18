/**
 * LightOS Accessibility
 * Keyboard Helper header
 */

#ifndef KEYBOARD_HELPER_H
#define KEYBOARD_HELPER_H

// Keyboard helper functions
int keyboard_helper_init();
int keyboard_helper_set_sticky_keys(unsigned int enable);
int keyboard_helper_is_sticky_keys_enabled();
int keyboard_helper_set_bounce_keys(unsigned int enable);
int keyboard_helper_is_bounce_keys_enabled();
int keyboard_helper_set_toggle_keys(unsigned int enable);
int keyboard_helper_is_toggle_keys_enabled();
int keyboard_helper_set_slow_keys(unsigned int enable);
int keyboard_helper_is_slow_keys_enabled();
int keyboard_helper_set_mouse_keys(unsigned int enable);
int keyboard_helper_is_mouse_keys_enabled();
int keyboard_helper_is_enabled();
int keyboard_helper_enable();
int keyboard_helper_disable();
int keyboard_helper_toggle();
int keyboard_helper_set_sticky_keys_timeout(unsigned int timeout);
int keyboard_helper_get_sticky_keys_timeout();
int keyboard_helper_set_bounce_keys_delay(unsigned int delay);
int keyboard_helper_get_bounce_keys_delay();
int keyboard_helper_set_slow_keys_delay(unsigned int delay);
int keyboard_helper_get_slow_keys_delay();
int keyboard_helper_set_mouse_keys_speed(unsigned int speed);
int keyboard_helper_get_mouse_keys_speed();
int keyboard_helper_set_mouse_keys_acceleration(unsigned int acceleration);
int keyboard_helper_get_mouse_keys_acceleration();
int keyboard_helper_set_mouse_keys_max_speed(unsigned int max_speed);
int keyboard_helper_get_mouse_keys_max_speed();
int keyboard_helper_set_repeat_keys(unsigned int enable);
int keyboard_helper_is_repeat_keys_enabled();
int keyboard_helper_set_repeat_keys_delay(unsigned int delay);
int keyboard_helper_get_repeat_keys_delay();
int keyboard_helper_set_repeat_keys_rate(unsigned int rate);
int keyboard_helper_get_repeat_keys_rate();
int keyboard_helper_set_cursor_blink_rate(unsigned int rate);
int keyboard_helper_get_cursor_blink_rate();
int keyboard_helper_set_cursor_thickness(unsigned int thickness);
int keyboard_helper_get_cursor_thickness();
int keyboard_helper_set_cursor_color(unsigned int color);
int keyboard_helper_get_cursor_color();
int keyboard_helper_set_key_sound(unsigned int enable);
int keyboard_helper_is_key_sound_enabled();
int keyboard_helper_set_key_sound_volume(unsigned int volume);
int keyboard_helper_get_key_sound_volume();
int keyboard_helper_set_key_sound_pitch(unsigned int pitch);
int keyboard_helper_get_key_sound_pitch();
int keyboard_helper_set_key_sound_duration(unsigned int duration);
int keyboard_helper_get_key_sound_duration();
int keyboard_helper_set_key_highlight(unsigned int enable);
int keyboard_helper_is_key_highlight_enabled();
int keyboard_helper_set_key_highlight_color(unsigned int color);
int keyboard_helper_get_key_highlight_color();
int keyboard_helper_set_key_highlight_duration(unsigned int duration);
int keyboard_helper_get_key_highlight_duration();
int keyboard_helper_set_on_screen_keyboard(unsigned int enable);
int keyboard_helper_is_on_screen_keyboard_enabled();
int keyboard_helper_set_on_screen_keyboard_size(unsigned int size);
int keyboard_helper_get_on_screen_keyboard_size();
int keyboard_helper_set_on_screen_keyboard_transparency(unsigned int transparency);
int keyboard_helper_get_on_screen_keyboard_transparency();
int keyboard_helper_set_on_screen_keyboard_position(unsigned int position);
int keyboard_helper_get_on_screen_keyboard_position();
int keyboard_helper_set_word_prediction(unsigned int enable);
int keyboard_helper_is_word_prediction_enabled();
int keyboard_helper_set_word_prediction_count(unsigned int count);
int keyboard_helper_get_word_prediction_count();
int keyboard_helper_reset();
int keyboard_helper_save_settings();
int keyboard_helper_load_settings();

#endif /* KEYBOARD_HELPER_H */

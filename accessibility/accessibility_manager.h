/**
 * LightOS Accessibility
 * Accessibility Manager header
 */

#ifndef ACCESSIBILITY_MANAGER_H
#define ACCESSIBILITY_MANAGER_H

// Accessibility feature flags
#define ACCESSIBILITY_SCREEN_READER    0x01
#define ACCESSIBILITY_MAGNIFIER        0x02
#define ACCESSIBILITY_HIGH_CONTRAST    0x04
#define ACCESSIBILITY_KEYBOARD_HELPER  0x08
#define ACCESSIBILITY_MOUSE_HELPER     0x10
#define ACCESSIBILITY_TEXT_TO_SPEECH   0x20
#define ACCESSIBILITY_SPEECH_TO_TEXT   0x40
#define ACCESSIBILITY_COLOR_FILTERS    0x80

// Accessibility settings structure
typedef struct {
    unsigned int enabled_features;
    
    // Screen reader settings
    unsigned int screen_reader_speed;
    unsigned int screen_reader_volume;
    unsigned int screen_reader_pitch;
    
    // Magnifier settings
    unsigned int magnifier_zoom;
    unsigned int magnifier_follow_mouse;
    unsigned int magnifier_follow_keyboard;
    unsigned int magnifier_follow_text;
    
    // High contrast settings
    unsigned int high_contrast_theme;
    unsigned int high_contrast_text_size;
    
    // Keyboard helper settings
    unsigned int keyboard_sticky_keys;
    unsigned int keyboard_bounce_keys;
    unsigned int keyboard_toggle_keys;
    unsigned int keyboard_slow_keys;
    unsigned int keyboard_mouse_keys;
    
    // Mouse helper settings
    unsigned int mouse_pointer_size;
    unsigned int mouse_pointer_color;
    unsigned int mouse_pointer_speed;
    unsigned int mouse_click_assist;
    
    // Text-to-speech settings
    unsigned int tts_voice;
    unsigned int tts_speed;
    unsigned int tts_volume;
    unsigned int tts_pitch;
    
    // Speech-to-text settings
    unsigned int stt_language;
    unsigned int stt_continuous;
    unsigned int stt_punctuation;
    
    // Color filter settings
    unsigned int color_filter_type;
    unsigned int color_filter_intensity;
} accessibility_settings_t;

// Accessibility manager functions
void accessibility_manager_init();
int accessibility_enable_feature(unsigned int feature);
int accessibility_disable_feature(unsigned int feature);
int accessibility_is_feature_enabled(unsigned int feature);
accessibility_settings_t* accessibility_get_settings();
int accessibility_set_settings(accessibility_settings_t* settings);
int accessibility_reset_settings();
int accessibility_save_settings();
int accessibility_load_settings();

// Screen reader functions
int screen_reader_init();
int screen_reader_speak(const char* text);
int screen_reader_stop();
int screen_reader_set_speed(unsigned int speed);
int screen_reader_set_volume(unsigned int volume);
int screen_reader_set_pitch(unsigned int pitch);

// Magnifier functions
int magnifier_init();
int magnifier_zoom_in();
int magnifier_zoom_out();
int magnifier_set_zoom(unsigned int zoom);
int magnifier_follow_mouse(unsigned int enable);
int magnifier_follow_keyboard(unsigned int enable);
int magnifier_follow_text(unsigned int enable);

// High contrast functions
int high_contrast_init();
int high_contrast_set_theme(unsigned int theme);
int high_contrast_set_text_size(unsigned int size);

// Keyboard helper functions
int keyboard_helper_init();
int keyboard_helper_set_sticky_keys(unsigned int enable);
int keyboard_helper_set_bounce_keys(unsigned int enable);
int keyboard_helper_set_toggle_keys(unsigned int enable);
int keyboard_helper_set_slow_keys(unsigned int enable);
int keyboard_helper_set_mouse_keys(unsigned int enable);

// Mouse helper functions
int mouse_helper_init();
int mouse_helper_set_pointer_size(unsigned int size);
int mouse_helper_set_pointer_color(unsigned int color);
int mouse_helper_set_pointer_speed(unsigned int speed);
int mouse_helper_set_click_assist(unsigned int enable);

// Text-to-speech functions
int text_to_speech_init();
int text_to_speech_speak(const char* text);
int text_to_speech_stop();
int text_to_speech_set_voice(unsigned int voice);
int text_to_speech_set_speed(unsigned int speed);
int text_to_speech_set_volume(unsigned int volume);
int text_to_speech_set_pitch(unsigned int pitch);

// Speech-to-text functions
int speech_to_text_init();
int speech_to_text_start();
int speech_to_text_stop();
int speech_to_text_get_text(char* buffer, unsigned int buffer_size);
int speech_to_text_set_language(unsigned int language);
int speech_to_text_set_continuous(unsigned int enable);
int speech_to_text_set_punctuation(unsigned int enable);

// Color filter functions
int color_filter_init();
int color_filter_set_type(unsigned int type);
int color_filter_set_intensity(unsigned int intensity);

#endif /* ACCESSIBILITY_MANAGER_H */

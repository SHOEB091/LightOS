/**
 * LightOS Drivers
 * Audio driver header
 */

#ifndef AUDIO_H
#define AUDIO_H

// Audio format structure
typedef struct {
    unsigned int sample_rate;
    unsigned int channels;
    unsigned int bits_per_sample;
} audio_format_t;

// Audio buffer structure
typedef struct {
    void* data;
    unsigned int size;
    unsigned int position;
    unsigned int loop;
} audio_buffer_t;

// Audio device structure
typedef struct {
    char name[32];
    audio_format_t format;
    unsigned int volume;
    unsigned int muted;
    
    // Function pointers for device operations
    int (*init)(struct audio_device* dev);
    int (*close)(struct audio_device* dev);
    int (*play)(struct audio_device* dev, audio_buffer_t* buffer);
    int (*stop)(struct audio_device* dev);
    int (*pause)(struct audio_device* dev);
    int (*resume)(struct audio_device* dev);
    int (*set_volume)(struct audio_device* dev, unsigned int volume);
    int (*get_volume)(struct audio_device* dev);
    int (*set_mute)(struct audio_device* dev, unsigned int muted);
    int (*get_mute)(struct audio_device* dev);
    int (*set_format)(struct audio_device* dev, audio_format_t* format);
    int (*get_format)(struct audio_device* dev, audio_format_t* format);
    
    // Private data for the driver
    void* private_data;
} audio_device_t;

// Audio driver functions
void audio_init();
int audio_register_device(audio_device_t* device);
int audio_unregister_device(const char* name);
audio_device_t* audio_get_device(const char* name);
int audio_play(const char* device_name, audio_buffer_t* buffer);
int audio_stop(const char* device_name);
int audio_pause(const char* device_name);
int audio_resume(const char* device_name);
int audio_set_volume(const char* device_name, unsigned int volume);
int audio_get_volume(const char* device_name);
int audio_set_mute(const char* device_name, unsigned int muted);
int audio_get_mute(const char* device_name);
int audio_set_format(const char* device_name, audio_format_t* format);
int audio_get_format(const char* device_name, audio_format_t* format);
void audio_list_devices();

// Audio buffer functions
audio_buffer_t* audio_buffer_create(unsigned int size);
void audio_buffer_destroy(audio_buffer_t* buffer);
int audio_buffer_load_wav(audio_buffer_t* buffer, const char* filename);
int audio_buffer_load_mp3(audio_buffer_t* buffer, const char* filename);
int audio_buffer_load_ogg(audio_buffer_t* buffer, const char* filename);
int audio_buffer_set_loop(audio_buffer_t* buffer, unsigned int loop);

// Specific audio device detection
int detect_audio_devices();

#endif /* AUDIO_H */

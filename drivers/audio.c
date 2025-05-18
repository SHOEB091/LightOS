/**
 * LightOS Drivers
 * Audio driver implementation
 */

#include "audio.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"

// Maximum number of audio devices
#define MAX_AUDIO_DEVICES 8

// Audio device array
static audio_device_t* audio_devices[MAX_AUDIO_DEVICES];
static int audio_device_count = 0;

// Initialize audio subsystem
void audio_init() {
    terminal_write("Initializing audio subsystem...\n");
    
    // Clear the device array
    for (int i = 0; i < MAX_AUDIO_DEVICES; i++) {
        audio_devices[i] = NULL;
    }
    
    audio_device_count = 0;
    
    // Detect audio devices
    detect_audio_devices();
    
    terminal_write("Audio subsystem initialized\n");
    terminal_write_color("Found ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert device_count to string
    char count_str[16];
    int temp = audio_device_count;
    int i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write(count_str);
    terminal_write(" audio devices\n");
}

// Register an audio device
int audio_register_device(audio_device_t* device) {
    if (!device) {
        return -1;
    }
    
    // Check if we have room for another device
    if (audio_device_count >= MAX_AUDIO_DEVICES) {
        terminal_write("Error: Maximum number of audio devices reached\n");
        return -1;
    }
    
    // Check if a device with the same name already exists
    for (int i = 0; i < audio_device_count; i++) {
        if (strcmp(audio_devices[i]->name, device->name) == 0) {
            terminal_write("Error: Audio device with name '");
            terminal_write(device->name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Allocate memory for the device
    audio_device_t* new_device = (audio_device_t*)allocate_block();
    
    if (!new_device) {
        terminal_write("Error: Failed to allocate memory for audio device\n");
        return -1;
    }
    
    // Copy the device data
    memcpy(new_device, device, sizeof(audio_device_t));
    
    // Add the device to the array
    audio_devices[audio_device_count++] = new_device;
    
    terminal_write("Registered audio device: ");
    terminal_write(device->name);
    terminal_write("\n");
    
    // Initialize the device if it has an init function
    if (new_device->init) {
        if (new_device->init(new_device) != 0) {
            terminal_write("Warning: Failed to initialize audio device '");
            terminal_write(new_device->name);
            terminal_write("'\n");
        }
    }
    
    return 0;
}

// Unregister an audio device
int audio_unregister_device(const char* name) {
    if (!name) {
        return -1;
    }
    
    // Find the device
    for (int i = 0; i < audio_device_count; i++) {
        if (strcmp(audio_devices[i]->name, name) == 0) {
            // Close the device if it has a close function
            if (audio_devices[i]->close) {
                audio_devices[i]->close(audio_devices[i]);
            }
            
            // Free the device memory
            free_block(audio_devices[i]);
            
            // Remove the device from the array by shifting all subsequent devices
            for (int j = i; j < audio_device_count - 1; j++) {
                audio_devices[j] = audio_devices[j + 1];
            }
            
            audio_devices[--audio_device_count] = NULL;
            
            terminal_write("Unregistered audio device: ");
            terminal_write(name);
            terminal_write("\n");
            
            return 0;
        }
    }
    
    terminal_write("Error: Audio device '");
    terminal_write(name);
    terminal_write("' not found\n");
    
    return -1;
}

// Get an audio device by name
audio_device_t* audio_get_device(const char* name) {
    if (!name) {
        return NULL;
    }
    
    // Find the device
    for (int i = 0; i < audio_device_count; i++) {
        if (strcmp(audio_devices[i]->name, name) == 0) {
            return audio_devices[i];
        }
    }
    
    return NULL;
}

// Play audio on a device
int audio_play(const char* device_name, audio_buffer_t* buffer) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->play) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support playing\n");
        return -1;
    }
    
    return device->play(device, buffer);
}

// Stop audio on a device
int audio_stop(const char* device_name) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->stop) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support stopping\n");
        return -1;
    }
    
    return device->stop(device);
}

// Pause audio on a device
int audio_pause(const char* device_name) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->pause) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support pausing\n");
        return -1;
    }
    
    return device->pause(device);
}

// Resume audio on a device
int audio_resume(const char* device_name) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->resume) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support resuming\n");
        return -1;
    }
    
    return device->resume(device);
}

// Set the volume of a device
int audio_set_volume(const char* device_name, unsigned int volume) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->set_volume) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support setting volume\n");
        return -1;
    }
    
    return device->set_volume(device, volume);
}

// Get the volume of a device
int audio_get_volume(const char* device_name) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->get_volume) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support getting volume\n");
        return -1;
    }
    
    return device->get_volume(device);
}

// Set the mute state of a device
int audio_set_mute(const char* device_name, unsigned int muted) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->set_mute) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support setting mute\n");
        return -1;
    }
    
    return device->set_mute(device, muted);
}

// Get the mute state of a device
int audio_get_mute(const char* device_name) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->get_mute) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support getting mute\n");
        return -1;
    }
    
    return device->get_mute(device);
}

// Set the format of a device
int audio_set_format(const char* device_name, audio_format_t* format) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->set_format) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support setting format\n");
        return -1;
    }
    
    return device->set_format(device, format);
}

// Get the format of a device
int audio_get_format(const char* device_name, audio_format_t* format) {
    audio_device_t* device = audio_get_device(device_name);
    
    if (!device) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' not found\n");
        return -1;
    }
    
    if (!device->get_format) {
        terminal_write("Error: Audio device '");
        terminal_write(device_name);
        terminal_write("' does not support getting format\n");
        return -1;
    }
    
    return device->get_format(device, format);
}

// List all audio devices
void audio_list_devices() {
    terminal_write("Audio Devices:\n");
    terminal_write("--------------\n");
    
    if (audio_device_count == 0) {
        terminal_write("No audio devices found\n");
        return;
    }
    
    for (int i = 0; i < audio_device_count; i++) {
        terminal_write(audio_devices[i]->name);
        terminal_write(": ");
        
        // Print format
        terminal_write("Sample Rate: ");
        
        // Convert sample rate to string
        char rate_str[16];
        int temp = audio_devices[i]->format.sample_rate;
        int idx = 0;
        
        if (temp == 0) {
            rate_str[idx++] = '0';
        } else {
            while (temp > 0) {
                rate_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        rate_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = rate_str[j];
            rate_str[j] = rate_str[idx - j - 1];
            rate_str[idx - j - 1] = tmp;
        }
        
        terminal_write(rate_str);
        terminal_write(" Hz, ");
        
        // Print channels
        terminal_write("Channels: ");
        
        // Convert channels to string
        char channels_str[16];
        temp = audio_devices[i]->format.channels;
        idx = 0;
        
        if (temp == 0) {
            channels_str[idx++] = '0';
        } else {
            while (temp > 0) {
                channels_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        channels_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = channels_str[j];
            channels_str[j] = channels_str[idx - j - 1];
            channels_str[idx - j - 1] = tmp;
        }
        
        terminal_write(channels_str);
        terminal_write(", ");
        
        // Print bits per sample
        terminal_write("Bits: ");
        
        // Convert bits per sample to string
        char bits_str[16];
        temp = audio_devices[i]->format.bits_per_sample;
        idx = 0;
        
        if (temp == 0) {
            bits_str[idx++] = '0';
        } else {
            while (temp > 0) {
                bits_str[idx++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        bits_str[idx] = '\0';
        
        // Reverse the string
        for (int j = 0; j < idx / 2; j++) {
            char tmp = bits_str[j];
            bits_str[j] = bits_str[idx - j - 1];
            bits_str[idx - j - 1] = tmp;
        }
        
        terminal_write(bits_str);
        
        terminal_write("\n");
    }
}

// Create an audio buffer
audio_buffer_t* audio_buffer_create(unsigned int size) {
    // Allocate memory for the buffer structure
    audio_buffer_t* buffer = (audio_buffer_t*)allocate_block();
    
    if (!buffer) {
        terminal_write("Error: Failed to allocate memory for audio buffer\n");
        return NULL;
    }
    
    // Allocate memory for the buffer data
    buffer->data = allocate_blocks((size + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!buffer->data) {
        terminal_write("Error: Failed to allocate memory for audio buffer data\n");
        free_block(buffer);
        return NULL;
    }
    
    // Initialize the buffer
    buffer->size = size;
    buffer->position = 0;
    buffer->loop = 0;
    
    return buffer;
}

// Destroy an audio buffer
void audio_buffer_destroy(audio_buffer_t* buffer) {
    if (!buffer) {
        return;
    }
    
    // Free the buffer data
    if (buffer->data) {
        free_blocks(buffer->data, (buffer->size + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    }
    
    // Free the buffer structure
    free_block(buffer);
}

// Load a WAV file into an audio buffer
int audio_buffer_load_wav(audio_buffer_t* buffer, const char* filename) {
    // In a real system, this would load a WAV file into the buffer
    // For now, just simulate a successful load
    return 0;
}

// Load an MP3 file into an audio buffer
int audio_buffer_load_mp3(audio_buffer_t* buffer, const char* filename) {
    // In a real system, this would load an MP3 file into the buffer
    // For now, just simulate a successful load
    return 0;
}

// Load an OGG file into an audio buffer
int audio_buffer_load_ogg(audio_buffer_t* buffer, const char* filename) {
    // In a real system, this would load an OGG file into the buffer
    // For now, just simulate a successful load
    return 0;
}

// Set the loop flag of an audio buffer
int audio_buffer_set_loop(audio_buffer_t* buffer, unsigned int loop) {
    if (!buffer) {
        return -1;
    }
    
    buffer->loop = loop;
    
    return 0;
}

// Detect audio devices
int detect_audio_devices() {
    // In a real system, this would scan for audio devices
    // For now, just simulate finding a sound card
    
    audio_device_t device;
    
    // Sound card
    strcpy(device.name, "soundcard0");
    device.format.sample_rate = 44100;
    device.format.channels = 2;
    device.format.bits_per_sample = 16;
    device.volume = 100;
    device.muted = 0;
    
    // Function pointers would be set to actual functions in a real system
    device.init = NULL;
    device.close = NULL;
    device.play = NULL;
    device.stop = NULL;
    device.pause = NULL;
    device.resume = NULL;
    device.set_volume = NULL;
    device.get_volume = NULL;
    device.set_mute = NULL;
    device.get_mute = NULL;
    device.set_format = NULL;
    device.get_format = NULL;
    
    device.private_data = NULL;
    
    audio_register_device(&device);
    
    return 1; // Found 1 device
}

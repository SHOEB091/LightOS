/**
 * LightOS IoT
 * Sensor Support implementation
 */

#include "sensor_support.h"
#include "../../kernel/kernel.h"
#include "../../kernel/memory.h"
#include "../../libc/string.h"
#include "../../libc/math.h"

// Sensor type names
static const char* sensor_type_names[] = {
    "Temperature",
    "Humidity",
    "Pressure",
    "Light",
    "Motion",
    "Door",
    "Window",
    "Water",
    "Smoke",
    "CO",
    "CO2",
    "Gas",
    "Dust",
    "Noise",
    "Vibration",
    "Voltage",
    "Current",
    "Power",
    "Energy",
    "Distance",
    "Angle",
    "Rotation",
    "Acceleration",
    "Gyroscope",
    "Magnetometer",
    "GPS",
    "Custom"
};

// Sensor unit names
static const char* sensor_unit_names[] = {
    "°C",
    "°F",
    "K",
    "%",
    "Pa",
    "hPa",
    "bar",
    "psi",
    "lux",
    "boolean",
    "ppm",
    "ppb",
    "dB",
    "V",
    "A",
    "W",
    "kW",
    "Wh",
    "kWh",
    "m",
    "cm",
    "mm",
    "in",
    "ft",
    "yd",
    "°",
    "rad",
    "rpm",
    "Hz",
    "m/s²",
    "g",
    "T",
    "G",
    "lat,lon",
    "custom"
};

// Initialize sensor support
int sensor_init() {
    terminal_write("Initializing sensor support...\n");
    
    // In a real system, we would:
    // 1. Initialize the sensor library
    // 2. Set up global sensor settings
    
    terminal_write("Sensor support initialized\n");
    
    return 0;
}

// Read a sensor value as a string
int sensor_read(iot_device_t* device, char* value, unsigned int value_size) {
    if (!device || !value) {
        terminal_write("Error: Device and value cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // In a real system, we would:
    // 1. Read the sensor value from the hardware
    // 2. Format the value as a string
    
    // For now, just simulate a sensor reading
    double sensor_value = 0.0;
    
    switch (sensor->type) {
        case SENSOR_TYPE_TEMPERATURE:
            sensor_value = 22.5; // 22.5°C
            break;
        
        case SENSOR_TYPE_HUMIDITY:
            sensor_value = 45.0; // 45%
            break;
        
        case SENSOR_TYPE_PRESSURE:
            sensor_value = 1013.25; // 1013.25 hPa
            break;
        
        case SENSOR_TYPE_LIGHT:
            sensor_value = 500.0; // 500 lux
            break;
        
        case SENSOR_TYPE_MOTION:
            sensor_value = 0.0; // No motion
            break;
        
        default:
            sensor_value = 0.0;
            break;
    }
    
    // Format the value as a string
    sensor_format_value(sensor_value, sensor->unit, sensor->precision, value, value_size);
    
    // Check if the value is outside the threshold range
    if (sensor->notifications_enabled && 
        (sensor_value < sensor->min_threshold || sensor_value > sensor->max_threshold)) {
        // Trigger a notification
        if (sensor->notification_callback) {
            sensor->notification_callback(device, sensor_value);
        }
    }
    
    return 0;
}

// Read a sensor value as a double
int sensor_read_double(iot_device_t* device, double* value) {
    if (!device || !value) {
        terminal_write("Error: Device and value cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // In a real system, we would:
    // 1. Read the sensor value from the hardware
    
    // For now, just simulate a sensor reading
    switch (sensor->type) {
        case SENSOR_TYPE_TEMPERATURE:
            *value = 22.5; // 22.5°C
            break;
        
        case SENSOR_TYPE_HUMIDITY:
            *value = 45.0; // 45%
            break;
        
        case SENSOR_TYPE_PRESSURE:
            *value = 1013.25; // 1013.25 hPa
            break;
        
        case SENSOR_TYPE_LIGHT:
            *value = 500.0; // 500 lux
            break;
        
        case SENSOR_TYPE_MOTION:
            *value = 0.0; // No motion
            break;
        
        default:
            *value = 0.0;
            break;
    }
    
    // Check if the value is outside the threshold range
    if (sensor->notifications_enabled && 
        (*value < sensor->min_threshold || *value > sensor->max_threshold)) {
        // Trigger a notification
        if (sensor->notification_callback) {
            sensor->notification_callback(device, *value);
        }
    }
    
    return 0;
}

// Set the threshold for a sensor
int sensor_set_threshold(iot_device_t* device, double min_threshold, double max_threshold) {
    if (!device) {
        terminal_write("Error: Device cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // Set the threshold values
    sensor->min_threshold = min_threshold;
    sensor->max_threshold = max_threshold;
    
    return 0;
}

// Get the threshold for a sensor
int sensor_get_threshold(iot_device_t* device, double* min_threshold, double* max_threshold) {
    if (!device || !min_threshold || !max_threshold) {
        terminal_write("Error: Device, min_threshold, and max_threshold cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // Get the threshold values
    *min_threshold = sensor->min_threshold;
    *max_threshold = sensor->max_threshold;
    
    return 0;
}

// Set the reading interval for a sensor
int sensor_set_interval(iot_device_t* device, unsigned int interval) {
    if (!device) {
        terminal_write("Error: Device cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // Set the interval
    sensor->interval = interval;
    
    return 0;
}

// Get the reading interval for a sensor
int sensor_get_interval(iot_device_t* device) {
    if (!device) {
        terminal_write("Error: Device cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // Return the interval
    return sensor->interval;
}

// Enable or disable notifications for a sensor
int sensor_enable_notifications(iot_device_t* device, unsigned int enable) {
    if (!device) {
        terminal_write("Error: Device cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // Set the notifications flag
    sensor->notifications_enabled = enable ? 1 : 0;
    
    return 0;
}

// Check if notifications are enabled for a sensor
int sensor_is_notifications_enabled(iot_device_t* device) {
    if (!device) {
        terminal_write("Error: Device cannot be NULL\n");
        return -1;
    }
    
    // Check if the device is a sensor
    if (device->type != IOT_DEVICE_SENSOR) {
        terminal_write("Error: Device is not a sensor\n");
        return -1;
    }
    
    // Check if the device has a private data structure
    sensor_t* sensor = (sensor_t*)device->private_data;
    
    if (!sensor) {
        terminal_write("Error: Sensor data not found\n");
        return -1;
    }
    
    // Return the notifications flag
    return sensor->notifications_enabled;
}

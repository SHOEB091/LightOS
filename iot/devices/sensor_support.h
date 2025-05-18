/**
 * LightOS IoT
 * Sensor Support header
 */

#ifndef SENSOR_SUPPORT_H
#define SENSOR_SUPPORT_H

#include "../iot_manager.h"

// Sensor types
#define SENSOR_TYPE_TEMPERATURE 0
#define SENSOR_TYPE_HUMIDITY 1
#define SENSOR_TYPE_PRESSURE 2
#define SENSOR_TYPE_LIGHT 3
#define SENSOR_TYPE_MOTION 4
#define SENSOR_TYPE_DOOR 5
#define SENSOR_TYPE_WINDOW 6
#define SENSOR_TYPE_WATER 7
#define SENSOR_TYPE_SMOKE 8
#define SENSOR_TYPE_CO 9
#define SENSOR_TYPE_CO2 10
#define SENSOR_TYPE_GAS 11
#define SENSOR_TYPE_DUST 12
#define SENSOR_TYPE_NOISE 13
#define SENSOR_TYPE_VIBRATION 14
#define SENSOR_TYPE_VOLTAGE 15
#define SENSOR_TYPE_CURRENT 16
#define SENSOR_TYPE_POWER 17
#define SENSOR_TYPE_ENERGY 18
#define SENSOR_TYPE_DISTANCE 19
#define SENSOR_TYPE_ANGLE 20
#define SENSOR_TYPE_ROTATION 21
#define SENSOR_TYPE_ACCELERATION 22
#define SENSOR_TYPE_GYROSCOPE 23
#define SENSOR_TYPE_MAGNETOMETER 24
#define SENSOR_TYPE_GPS 25
#define SENSOR_TYPE_CUSTOM 26

// Sensor units
#define SENSOR_UNIT_CELSIUS 0
#define SENSOR_UNIT_FAHRENHEIT 1
#define SENSOR_UNIT_KELVIN 2
#define SENSOR_UNIT_PERCENT 3
#define SENSOR_UNIT_PASCAL 4
#define SENSOR_UNIT_HECTOPASCAL 5
#define SENSOR_UNIT_BAR 6
#define SENSOR_UNIT_PSI 7
#define SENSOR_UNIT_LUX 8
#define SENSOR_UNIT_BOOLEAN 9
#define SENSOR_UNIT_PPM 10
#define SENSOR_UNIT_PPB 11
#define SENSOR_UNIT_DECIBEL 12
#define SENSOR_UNIT_VOLT 13
#define SENSOR_UNIT_AMPERE 14
#define SENSOR_UNIT_WATT 15
#define SENSOR_UNIT_KILOWATT 16
#define SENSOR_UNIT_WATT_HOUR 17
#define SENSOR_UNIT_KILOWATT_HOUR 18
#define SENSOR_UNIT_METER 19
#define SENSOR_UNIT_CENTIMETER 20
#define SENSOR_UNIT_MILLIMETER 21
#define SENSOR_UNIT_INCH 22
#define SENSOR_UNIT_FOOT 23
#define SENSOR_UNIT_YARD 24
#define SENSOR_UNIT_DEGREE 25
#define SENSOR_UNIT_RADIAN 26
#define SENSOR_UNIT_RPM 27
#define SENSOR_UNIT_HERTZ 28
#define SENSOR_UNIT_METER_PER_SECOND_SQUARED 29
#define SENSOR_UNIT_G_FORCE 30
#define SENSOR_UNIT_TESLA 31
#define SENSOR_UNIT_GAUSS 32
#define SENSOR_UNIT_LATITUDE_LONGITUDE 33
#define SENSOR_UNIT_CUSTOM 34

// Sensor structure
typedef struct {
    unsigned int type;
    unsigned int unit;
    double min_value;
    double max_value;
    double min_threshold;
    double max_threshold;
    unsigned int precision;
    unsigned int interval;
    unsigned int notifications_enabled;
    void (*notification_callback)(iot_device_t* device, double value);
} sensor_t;

// Sensor functions
int sensor_init();
int sensor_read(iot_device_t* device, char* value, unsigned int value_size);
int sensor_read_double(iot_device_t* device, double* value);
int sensor_set_threshold(iot_device_t* device, double min_threshold, double max_threshold);
int sensor_get_threshold(iot_device_t* device, double* min_threshold, double* max_threshold);
int sensor_set_interval(iot_device_t* device, unsigned int interval);
int sensor_get_interval(iot_device_t* device);
int sensor_enable_notifications(iot_device_t* device, unsigned int enable);
int sensor_is_notifications_enabled(iot_device_t* device);
int sensor_set_notification_callback(iot_device_t* device, void (*callback)(iot_device_t* device, double value));
int sensor_get_type(iot_device_t* device, unsigned int* type);
int sensor_set_type(iot_device_t* device, unsigned int type);
int sensor_get_unit(iot_device_t* device, unsigned int* unit);
int sensor_set_unit(iot_device_t* device, unsigned int unit);
int sensor_get_min_value(iot_device_t* device, double* min_value);
int sensor_set_min_value(iot_device_t* device, double min_value);
int sensor_get_max_value(iot_device_t* device, double* max_value);
int sensor_set_max_value(iot_device_t* device, double max_value);
int sensor_get_precision(iot_device_t* device, unsigned int* precision);
int sensor_set_precision(iot_device_t* device, unsigned int precision);
int sensor_convert_value(double value, unsigned int from_unit, unsigned int to_unit, double* converted_value);
int sensor_format_value(double value, unsigned int unit, unsigned int precision, char* formatted_value, unsigned int formatted_value_size);
int sensor_parse_value(const char* formatted_value, unsigned int unit, double* value);
int sensor_get_unit_name(unsigned int unit, char* name, unsigned int name_size);
int sensor_get_type_name(unsigned int type, char* name, unsigned int name_size);
int sensor_calibrate(iot_device_t* device, double reference_value);
int sensor_reset_calibration(iot_device_t* device);
int sensor_get_calibration_status(iot_device_t* device, unsigned int* calibrated);
int sensor_get_calibration_date(iot_device_t* device, char* date, unsigned int date_size);
int sensor_get_calibration_reference(iot_device_t* device, double* reference_value);
int sensor_get_calibration_offset(iot_device_t* device, double* offset);
int sensor_get_calibration_gain(iot_device_t* device, double* gain);
int sensor_set_calibration_offset(iot_device_t* device, double offset);
int sensor_set_calibration_gain(iot_device_t* device, double gain);

#endif /* SENSOR_SUPPORT_H */

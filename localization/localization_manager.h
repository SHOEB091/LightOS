/**
 * LightOS Localization
 * Localization Manager header
 */

#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

// Maximum string length for localization
#define LOCALIZATION_MAX_STRING_LENGTH 1024

// Language codes
#define LANGUAGE_EN_US 0 // English (United States)
#define LANGUAGE_EN_GB 1 // English (United Kingdom)
#define LANGUAGE_FR_FR 2 // French (France)
#define LANGUAGE_DE_DE 3 // German (Germany)
#define LANGUAGE_ES_ES 4 // Spanish (Spain)
#define LANGUAGE_IT_IT 5 // Italian (Italy)
#define LANGUAGE_PT_BR 6 // Portuguese (Brazil)
#define LANGUAGE_RU_RU 7 // Russian (Russia)
#define LANGUAGE_ZH_CN 8 // Chinese (Simplified)
#define LANGUAGE_ZH_TW 9 // Chinese (Traditional)
#define LANGUAGE_JA_JP 10 // Japanese (Japan)
#define LANGUAGE_KO_KR 11 // Korean (Korea)
#define LANGUAGE_AR_SA 12 // Arabic (Saudi Arabia)
#define LANGUAGE_HI_IN 13 // Hindi (India)
#define LANGUAGE_MAX 14 // Maximum number of languages

// Region codes
#define REGION_US 0 // United States
#define REGION_GB 1 // United Kingdom
#define REGION_FR 2 // France
#define REGION_DE 3 // Germany
#define REGION_ES 4 // Spain
#define REGION_IT 5 // Italy
#define REGION_BR 6 // Brazil
#define REGION_RU 7 // Russia
#define REGION_CN 8 // China
#define REGION_TW 9 // Taiwan
#define REGION_JP 10 // Japan
#define REGION_KR 11 // Korea
#define REGION_SA 12 // Saudi Arabia
#define REGION_IN 13 // India
#define REGION_MAX 14 // Maximum number of regions

// Date formats
#define DATE_FORMAT_MDY 0 // MM/DD/YYYY
#define DATE_FORMAT_DMY 1 // DD/MM/YYYY
#define DATE_FORMAT_YMD 2 // YYYY/MM/DD

// Time formats
#define TIME_FORMAT_12H 0 // 12-hour format
#define TIME_FORMAT_24H 1 // 24-hour format

// Number formats
#define NUMBER_FORMAT_PERIOD 0 // 1,234.56
#define NUMBER_FORMAT_COMMA 1 // 1.234,56

// Currency formats
#define CURRENCY_FORMAT_PREFIX 0 // $1,234.56
#define CURRENCY_FORMAT_SUFFIX 1 // 1,234.56€

// Measurement systems
#define MEASUREMENT_IMPERIAL 0 // Imperial system
#define MEASUREMENT_METRIC 1 // Metric system

// Paper sizes
#define PAPER_SIZE_LETTER 0 // Letter (8.5" x 11")
#define PAPER_SIZE_A4 1 // A4 (210mm x 297mm)

// Localization settings structure
typedef struct {
    unsigned int language;
    unsigned int region;
    unsigned int date_format;
    unsigned int time_format;
    unsigned int number_format;
    unsigned int currency_format;
    unsigned int measurement_system;
    unsigned int paper_size;
    char currency_symbol[8];
    char thousands_separator[8];
    char decimal_separator[8];
    char date_separator[8];
    char time_separator[8];
    char list_separator[8];
    char first_day_of_week;
    char keyboard_layout[32];
    char input_method[32];
} localization_settings_t;

// Localization manager functions
void localization_manager_init();
int localization_set_language(unsigned int language);
unsigned int localization_get_language();
int localization_set_region(unsigned int region);
unsigned int localization_get_region();
int localization_set_date_format(unsigned int format);
unsigned int localization_get_date_format();
int localization_set_time_format(unsigned int format);
unsigned int localization_get_time_format();
int localization_set_number_format(unsigned int format);
unsigned int localization_get_number_format();
int localization_set_currency_format(unsigned int format);
unsigned int localization_get_currency_format();
int localization_set_measurement_system(unsigned int system);
unsigned int localization_get_measurement_system();
int localization_set_paper_size(unsigned int size);
unsigned int localization_get_paper_size();
int localization_set_currency_symbol(const char* symbol);
const char* localization_get_currency_symbol();
int localization_set_thousands_separator(const char* separator);
const char* localization_get_thousands_separator();
int localization_set_decimal_separator(const char* separator);
const char* localization_get_decimal_separator();
int localization_set_date_separator(const char* separator);
const char* localization_get_date_separator();
int localization_set_time_separator(const char* separator);
const char* localization_get_time_separator();
int localization_set_list_separator(const char* separator);
const char* localization_get_list_separator();
int localization_set_first_day_of_week(char day);
char localization_get_first_day_of_week();
int localization_set_keyboard_layout(const char* layout);
const char* localization_get_keyboard_layout();
int localization_set_input_method(const char* method);
const char* localization_get_input_method();
localization_settings_t* localization_get_settings();
int localization_set_settings(localization_settings_t* settings);
int localization_reset_settings();
int localization_save_settings();
int localization_load_settings();
int localization_get_string(const char* key, char* buffer, unsigned int buffer_size);
int localization_format_date(unsigned int year, unsigned int month, unsigned int day, char* buffer, unsigned int buffer_size);
int localization_format_time(unsigned int hour, unsigned int minute, unsigned int second, char* buffer, unsigned int buffer_size);
int localization_format_number(double number, char* buffer, unsigned int buffer_size);
int localization_format_currency(double amount, char* buffer, unsigned int buffer_size);
int localization_format_percentage(double percentage, char* buffer, unsigned int buffer_size);
int localization_format_measurement(double value, unsigned int unit, char* buffer, unsigned int buffer_size);
int localization_parse_date(const char* date, unsigned int* year, unsigned int* month, unsigned int* day);
int localization_parse_time(const char* time, unsigned int* hour, unsigned int* minute, unsigned int* second);
int localization_parse_number(const char* number, double* value);
int localization_parse_currency(const char* currency, double* amount);
int localization_parse_percentage(const char* percentage, double* value);
int localization_parse_measurement(const char* measurement, double* value, unsigned int* unit);
const char* localization_get_language_name(unsigned int language);
const char* localization_get_region_name(unsigned int region);
const char* localization_get_date_format_name(unsigned int format);
const char* localization_get_time_format_name(unsigned int format);
const char* localization_get_number_format_name(unsigned int format);
const char* localization_get_currency_format_name(unsigned int format);
const char* localization_get_measurement_system_name(unsigned int system);
const char* localization_get_paper_size_name(unsigned int size);

#endif /* LOCALIZATION_MANAGER_H */

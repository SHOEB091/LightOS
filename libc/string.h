/**
 * LightOS C Library
 * String manipulation functions header
 */

#ifndef STRING_H
#define STRING_H

// Define size_t
typedef unsigned int size_t;

// String functions
size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
int strcmp(const char* s1, const char* s2);

// Memory functions
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);
void* memmove(void* dest, const void* src, size_t num);
int memcmp(const void* ptr1, const void* ptr2, size_t num);

#endif /* STRING_H */

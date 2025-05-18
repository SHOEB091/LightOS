/**
 * LightOS C Library
 * Basic string manipulation functions
 */

#include "string.h"

// Calculate the length of a string
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// Copy a string
char* strcpy(char* dest, const char* src) {
    int i = 0;
    while ((dest[i] = src[i]) != '\0')
        i++;
    return dest;
}

// Copy n characters from a string
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    
    return dest;
}

// Concatenate two strings
char* strcat(char* dest, const char* src) {
    char* ptr = dest + strlen(dest);
    
    while (*src != '\0') {
        *ptr++ = *src++;
    }
    
    *ptr = '\0';
    return dest;
}

// Compare two strings
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Set a block of memory
void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*) ptr;
    
    for (size_t i = 0; i < num; i++) {
        p[i] = (unsigned char) value;
    }
    
    return ptr;
}

// Copy a block of memory
void* memcpy(void* dest, const void* src, size_t num) {
    unsigned char* d = (unsigned char*) dest;
    const unsigned char* s = (const unsigned char*) src;
    
    for (size_t i = 0; i < num; i++) {
        d[i] = s[i];
    }
    
    return dest;
}

// Move a block of memory (handles overlapping memory)
void* memmove(void* dest, const void* src, size_t num) {
    unsigned char* d = (unsigned char*) dest;
    const unsigned char* s = (const unsigned char*) src;
    
    if (d < s) {
        // Copy from beginning to end
        for (size_t i = 0; i < num; i++) {
            d[i] = s[i];
        }
    } else if (d > s) {
        // Copy from end to beginning
        for (size_t i = num; i > 0; i--) {
            d[i-1] = s[i-1];
        }
    }
    
    return dest;
}

// Compare two blocks of memory
int memcmp(const void* ptr1, const void* ptr2, size_t num) {
    const unsigned char* p1 = (const unsigned char*) ptr1;
    const unsigned char* p2 = (const unsigned char*) ptr2;
    
    for (size_t i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    
    return 0;
}

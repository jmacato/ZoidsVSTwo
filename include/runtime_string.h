#ifndef ZOIDS_RUNTIME_STRING_H
#define ZOIDS_RUNTIME_STRING_H
#include "types.h"
typedef unsigned long size_t;
#ifdef __cplusplus
extern "C" {
#endif
void *memcpy(void *destination, const void *source, size_t count);
void *memset(void *destination, int value, size_t count);
void *memmove(void *destination, const void *source, size_t count);
void *memchr(const void *memory, int value, size_t count);
int memcmp(const void *left, const void *right, size_t count);
size_t strlen(const char *text);
char *strcpy(char *destination, const char *source);
char *strncpy(char *destination, const char *source, size_t count);
char *strcat(char *destination, const char *source);
int strcmp(const char *left, const char *right);
int strncmp(const char *left, const char *right, size_t count);
char *strchr(const char *text, int value);
void __fill_mem(void *destination, int value, unsigned long count);
void __copy_longs_aligned(void *destination, const void *source, size_t count);
void __copy_longs_rev_aligned(void *destination, const void *source, size_t count);
void __copy_longs_unaligned(void *destination, const void *source, size_t count);
void __copy_longs_rev_unaligned(void *destination, const void *source, size_t count);
#ifdef __cplusplus
}
#endif
#endif

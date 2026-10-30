#ifndef ZOIDS_OS_MEMORY_H
#define ZOIDS_OS_MEMORY_H
#include "types.h"
extern u32 OSGetPhysicalMemSize(void);
extern void __OSInitMemoryProtection(void);
#endif

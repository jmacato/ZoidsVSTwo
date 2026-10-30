#ifndef ZOIDS_OS_ARENA_H
#define ZOIDS_OS_ARENA_H
#include "types.h"
extern void *OSGetArenaHi(void);
extern void *OSGetArenaLo(void);
extern void OSSetArenaHi(void *address);
extern void OSSetArenaLo(void *address);
extern void *OSAllocFromArenaLo(u32 size, u32 alignment);
#endif

/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/os/OSArena.c. */
#include "os_arena.h"

#define ROUND(n, a) (((u32)(n) + (a) - 1) & ~((a) - 1))
#define TRUNC(n, a) (((u32)(n)) & ~((a) - 1))

static void *__OSArenaHi = 0;
static void *__OSArenaLo = (void *)-1;

void *OSGetArenaHi(void)
{
    return __OSArenaHi;
}

void *OSGetArenaLo(void)
{
    return __OSArenaLo;
}

void OSSetArenaHi(void *newHi)
{
    __OSArenaHi = newHi;
}

void OSSetArenaLo(void *newLo)
{
    __OSArenaLo = newLo;
}

void *OSAllocFromArenaLo(u32 size, u32 align)
{
    void *ptr;
    u8 *arenaLo;

    ptr = OSGetArenaLo();
    arenaLo = ptr = (void *)ROUND(ptr, align);
    arenaLo += size;
    arenaLo = (u8 *)ROUND(arenaLo, align);
    OSSetArenaLo(arenaLo);
    return ptr;
}

#include "initialize.h"

extern void *hsd_heap_arena_lo;
extern void *hsd_heap_arena_hi;

void HSD_GetCurrentArena(void **lo, void **hi)
{
    if (lo != 0)
        *lo = hsd_heap_arena_lo;
    if (hi != 0)
        *hi = hsd_heap_arena_hi;
}

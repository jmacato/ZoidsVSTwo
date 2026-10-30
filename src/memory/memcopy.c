#include "types.h"

extern void *memcpy(void *dst, const void *src, u32 n);
extern void DCFlushRange(void *address, u32 size);

void memcopy(void *dst, void *src, u32 size)
{
    memcpy(dst, src, size);
    DCFlushRange(dst, size);
}

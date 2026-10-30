#include "types.h"

extern void DCFlushRange(void *addr, u32 nBytes);

void MemCpy(u8 *source, u8 *destination, s32 size)
{
    while (size) {
        *destination++ = *source++;
        size--;
    }
    DCFlushRange(destination, size);
}

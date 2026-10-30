#include "types.h"
void __OSSystemCallVectorStart();
void __OSSystemCallVectorEnd();
extern void *memcpy(void *dest, const void *src, u32 n);
extern void DCFlushRangeNoSync(void *addr, u32 nBytes);
extern void ICInvalidateRange(void *addr, u32 nBytes);

#pragma peephole off

void __OSInitSystemCall(void)
{
    void *addr = (void *)0x80000C00;

    memcpy(addr, __OSSystemCallVectorStart, (u32)&__OSSystemCallVectorEnd - (u32)&__OSSystemCallVectorStart);
    DCFlushRangeNoSync(addr, 0x100);
    __sync();
    ICInvalidateRange(addr, 0x100);
}

#include "types.h"

extern void *MemCpy(void *dst, const void *src, u32 size);
extern void DCFlushRange(void *addr, u32 size);
extern void GXInvalidateTexAll(void);
extern void OSReport(const char *, ...);

s32 SetTex2(u8 *tex, u8 *data, s32 size)
{
    s32 imgsize = size;
    tex += 0x40;
    MemCpy(tex, data, imgsize);
    DCFlushRange(tex, imgsize);
    DCFlushRange(data, imgsize);
    GXInvalidateTexAll();
    OSReport("imgsize=%d tsize=%d \n", imgsize, size);
    return 1;
}

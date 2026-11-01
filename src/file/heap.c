#include "types.h"
#include "os_alloc.h"

extern void *HSD_Alloc(u32 size);
extern s32 HSD_GetHeap(void);
extern void OSReport(const char *, ...);
extern void putErr(const char *message);
extern s32 usrHeap;
extern s32 dbgAlloc;

#pragma dont_inline on
void debugFileInsert(void *memory)
{
    /* The shipped debug hook only checks its pointer and returns. */
    if (memory == 0)
        return;
}

void *mallocF(u32 size);



void *malloc2(u32 size)
{
    void *memory = mallocF(size);
    if (memory == 0)
        putErr(
            "\x83\x81\x83\x82\x83\x8a\x82\xaa\x91\xab\x82\xe8\x82\xdc\x82\xb9\x82\xf1");
    return memory;
}

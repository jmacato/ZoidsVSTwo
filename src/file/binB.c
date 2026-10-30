#include "types.h"

void *binB(void *bin, s32 index)
{
    u32 *entries = (u32 *)bin;
    u32 offset;
    u8 *base = (u8 *)bin;
    u32 count = *entries++;

    if (count <= index) {
        return 0;
    }
    offset = entries[index];
    if (offset == 0) {
        return 0;
    }
    return base + offset;
}

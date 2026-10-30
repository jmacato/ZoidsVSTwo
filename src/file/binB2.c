#include "types.h"

typedef struct BinEntry {
    u32 offset;
    u32 size;
} BinEntry;

u8 *binB2(u8 *bin, u32 index, u32 *size)
{
    u8 *base = bin;
    u32 count;
    BinEntry *entries;
    u32 offset;
    if (size != 0)
        *size = 0;
    count = *(u32 *)base;
    entries = (BinEntry *)(base + 8);
    if (count <= index)
        return 0;
    offset = entries[index].offset;
    if (offset == 0)
        return 0;
    if (size != 0)
        *size = entries[index].size;
    return base + offset;
}

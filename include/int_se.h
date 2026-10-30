#ifndef ZOIDS_INT_SE_H
#define ZOIDS_INT_SE_H

#include "types.h"

typedef struct IntSeEntry {
    f32 distance;
    s16 se;
    f32 x;
    f32 z;
    s32 handle;
    s32 timer;
} IntSeEntry;

typedef char IntSeEntrySizeCheck[sizeof(IntSeEntry) == 0x18 ? 1 : -1];

#endif

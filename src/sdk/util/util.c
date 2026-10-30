/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/util.c. */
#include "util.h"
extern void __assert(const char *, u32, const char *);
#define HSD_ASSERT(line, condition)                                                    \
    ((condition) ? (void)0 : __assert("util.c", line, #condition))
Mtx HSD_identityMtx = {{1.0f, 0, 0, 0}, {0, 1.0f, 0, 0}, {0, 0, 1.0f, 0}};

void HSD_MulColor(GXColor *first, GXColor *second, GXColor *dest)
{
    dest->r = (first->r * second->r) / 255U;
    dest->g = (first->g * second->g) / 255U;
    dest->b = (first->b * second->b) / 255U;
    dest->a = (first->a * second->a) / 255U;
}

u32 HSD_GetNbBits(u32 c)
{
    int ret = 0;
    int i;

    for (i = 0; i < 32; i++) {
        if (c & (1 << i)) {
            ret++;
        }
    }
    return ret;
}

s32 HSD_Index2PosNrmMtx(u32 index)
{
    switch (index) {
    case 0:
        return 0;
    case 1:
        return 3;
    case 2:
        return 6;
    case 3:
        return 9;
    case 4:
        return 12;
    case 5:
        return 15;
    case 6:
        return 18;
    case 7:
        return 21;
    case 8:
        return 24;
    case 9:
        return 27;
    default:
        HSD_ASSERT(132, 0);
        return 0;
    }
}

#include "fobj.h"

/* Encoding follows https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/fobj.c. */
f32 parseFloat(u8 **position, u8 format)
{
    union {
        f32 value;
        u32 bits;
    } number;
    f32 numerator;
    s32 denominator;

    if (format == HSD_A_FRAC_FLOAT) {
        number.bits = ((*position)++)[0];
        number.bits |= ((*position)++)[0] << 8;
        number.bits |= ((*position)++)[0] << 16;
        number.bits |= ((*position)++)[0] << 24;
        return number.value;
    }
    denominator = 1 << (format & 0x1F);
    switch (format & 0xE0) {
    case HSD_A_FRAC_S8:
        numerator = (s8)(*position)[0];
        *position += 1;
        break;
    case HSD_A_FRAC_U8:
        numerator = (*position)[0];
        *position += 1;
        break;
    case HSD_A_FRAC_S16:
        numerator = ((s8)(*position)[1] << 8) | (*position)[0];
        *position += 2;
        break;
    case HSD_A_FRAC_U16:
        numerator = ((*position)[1] << 8) | (*position)[0];
        *position += 2;
        break;
    default:
        return 0.0f;
    }
    return numerator / denominator;
}

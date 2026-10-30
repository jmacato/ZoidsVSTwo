#include "types.h"

inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

f32 angLimitAbs(f32 angle, f32 limit)
{
    if (absf(angle) <= limit) {
        return angle;
    }
    if (angle > limit) {
        return limit;
    }
    return -limit;
}

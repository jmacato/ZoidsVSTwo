#include "types.h"

inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

s32 sDivLineVn(f32 *start, f32 *end, s32 axis, f32 value, f32 *out)
{
    f32 start_distance;
    f32 end_distance;
    f32 t;
    s32 i;
    s32 pad = 0;

    if (value == start[axis]) {
        for (i = 0; i < 3; i++) {
            out[i] = start[i];
        }
        return 1;
    }
    if (value == end[axis]) {
        for (i = 0; i < 3; i++) {
            out[i] = end[i];
        }
        return 1;
    }
    if (start[axis] < value && end[axis] < value) {
        return 0;
    }
    if (start[axis] > value && end[axis] > value) {
        return 0;
    }
    start_distance = absf(value - start[axis]);
    end_distance = absf(value - end[axis]);
    if (start_distance > end_distance) {
        t = start_distance / absf(end[axis] - start[axis]);
        for (i = 0; i < 3; i++) {
            if (i == axis) {
                out[i] = value;
            } else {
                out[i] = t * (end[i] - start[i]) + start[i];
            }
        }
    } else {
        t = end_distance / absf(end[axis] - start[axis]);
        for (i = 0; i < 3; i++) {
            if (i == axis) {
                out[i] = value;
            } else {
                out[i] = t * (start[i] - end[i]) + end[i];
            }
        }
    }
    return 1;
}

#include "types.h"

extern f64 __frsqrte(f64);
extern void sPtRotX(Vec3 *point, f32 cosine, f32 sine);
extern void sPtRotY(Vec3 *point, f32 cosine, f32 sine);

static inline f32 sqrtf(f32 x)
{
    volatile f32 y;
    static const f64 _half = 0.5;
    static const f64 _three = 3.0;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        y = (f32)(x * guess);
        return y;
    }
    return x;
}

/* distance from point to the line start-end, 10000.0f when point projects outside the segment */
f32 sPassage(Vec3 *start, Vec3 *end, Vec3 *point)
{
    Vec3 to_point;
    Vec3 to_end;
    s32 i;
    f32 x;
    f32 point_x;
    f32 z;
    f32 len;
    f32 point_y;
    f32 point_z;

    for (i = 0; i < 3; i++) {
        *(f32 *)((u8 *)&to_end + 4 * i) = *(f32 *)((u8 *)end + 4 * i) - *(f32 *)((u8 *)start + 4 * i);
        *(f32 *)((u8 *)&to_point + 4 * i) = *(f32 *)((u8 *)point + 4 * i) - *(f32 *)((u8 *)start + 4 * i);
    }
    x = to_end.x;
    z = to_end.z;
    len = sqrtf(x * x + z * z);
    if (len != 0.0f) {
        x = x / len;
        z = z / len;
        sPtRotY(&to_end, z, -x);
        sPtRotY(&to_point, z, -x);
    }
    x = to_end.y;
    z = to_end.z;
    len = sqrtf(x * x + z * z);
    if (len != 0.0f) {
        x = x / len;
        z = z / len;
        sPtRotX(&to_end, z, -x);
        sPtRotX(&to_point, z, -x);
    }
    point_z = to_point.z;
    point_x = to_point.x;
    point_y = to_point.y;
    if (point_z < 0.0f)
        return 10000.0f;
    if (point_z > to_end.z)
        return 10000.0f;
    return sqrtf(point_x * point_x + point_y * point_y);
}

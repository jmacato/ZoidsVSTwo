#ifndef ZOIDS_CONSTRAINT_MATH_H
#define ZOIDS_CONSTRAINT_MATH_H

#include "types.h"

extern void PSVECCrossProduct(Vec3 *first, Vec3 *second, Vec3 *result);
extern f32 PSVECDotProduct(Vec3 *first, Vec3 *second);
extern void PSVECScale(Vec3 *source, Vec3 *result, f32 scale);
extern void PSVECSubtract(Vec3 *first, Vec3 *second, Vec3 *result);
extern f64 __frsqrte(f64 value);

static inline f32 constraint_root(f32 value)
{
    if (value > 0.0f) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;

        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        result = value * estimate;
        return result;
    }
    return value;
}

static inline f32 float_absolute(f32 value)
{
    union {
        f32 value;
        u32 bits;
    } result;
    result.value = value;
    result.bits &= 0x7FFFFFFF;
    return result.value;
}

#endif

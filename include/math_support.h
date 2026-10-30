#ifndef ZOIDS_MATH_SUPPORT_H
#define ZOIDS_MATH_SUPPORT_H

#include "types.h"

extern f64 __frsqrte(f64 value);

static inline f32 square_root(f32 value)
{
    if (value > 0.0f) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;

        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        /* This store preserves the original single-precision rounding step. */
        result = value * estimate;
        return result;
    }
    return value;
}

#endif

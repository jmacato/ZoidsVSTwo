#include "math_support.h"

f32 sLenPoint3dR(f32 *point)
{
    s32 axis;
    f32 total = 0.0f;

    for (axis = 0; axis < 3; axis++) {
        f32 value = point[axis];
        total += value * value;
    }
    return square_root(total);
}

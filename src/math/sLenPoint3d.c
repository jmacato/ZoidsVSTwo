#include "math_support.h"

f32 sLenPoint3d(f32 *first, f32 *second)
{
    s32 axis;
    f32 total = 0.0f;

    for (axis = 0; axis < 3; axis++) {
        f32 difference = first[axis] - second[axis];
        total += difference * difference;
    }
    return square_root(total);
}

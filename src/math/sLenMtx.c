#include "math_support.h"

f32 sLenMtx(f32 first[3][4], f32 second[3][4])
{
    s32 axis;
    f32 total = 0.0f;

    for (axis = 0; axis < 3; axis++) {
        f32 difference = first[axis][3] - second[axis][3];
        total += difference * difference;
    }
    return square_root(total);
}

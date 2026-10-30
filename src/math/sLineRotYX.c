#include "types.h"

extern void sRotVecYX(Vec3 *direction, Vec3 *point);

void sLineRotYX(f32 *origin, f32 *end, f32 *point)
{
    s32 axis;
    f32 direction[3];

    for (axis = 0; axis < 3; axis++) {
        point[axis] -= origin[axis];
        direction[axis] = end[axis] - origin[axis];
    }
    sRotVecYX((Vec3 *)direction, (Vec3 *)point);
}

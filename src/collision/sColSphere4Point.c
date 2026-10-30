#include "types.h"

s32 sColSphere4Point(Vec3 *point)
{
    f32 sum = point->z * point->z + (point->x * point->x + point->y * point->y);

    if (sum > 0.25) {
        return 0;
    }
    return 1;
}

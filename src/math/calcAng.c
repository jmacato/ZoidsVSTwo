#include "types.h"

extern f32 atan2f(f32 y, f32 x);

f32 calcAng(Vec3 *first, Vec3 *second)
{
    f32 x = first->x - second->x;
    f32 z = first->z - second->z;

    if (x == 0.0f && z == 0.0f) {
        return 100.0f;
    }
    return atan2f(x, z);
}

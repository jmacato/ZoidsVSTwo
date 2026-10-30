#include "types.h"

s32 sColCube4Point(Vec3 *point)
{
    if (point->x < -0.5) {
        return 0;
    }
    if (point->x > 0.5) {
        return 0;
    }
    if (point->y < -0.5) {
        return 0;
    }
    if (point->y > 0.5) {
        return 0;
    }
    if (point->z < -0.5) {
        return 0;
    }
    if (point->z > 0.5) {
        return 0;
    }
    return 1;
}

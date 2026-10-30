#include "types.h"

void sPtRotY(Vec3 *point, f32 cosine, f32 sine)
{
    f32 z = point->z;
    f32 x = point->x;

    point->z = z * cosine - x * sine;
    point->x = z * sine + x * cosine;
}

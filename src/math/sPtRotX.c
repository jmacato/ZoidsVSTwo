#include "types.h"

void sPtRotX(Vec3 *point, f32 cosine, f32 sine)
{
    f32 z = point->z;
    f32 y = point->y;

    point->z = z * cosine - y * sine;
    point->y = z * sine + y * cosine;
}

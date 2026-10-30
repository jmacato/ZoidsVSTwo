#include "types.h"

void sRotY(Vec3 *point, f32 cosine, f32 sine)
{
    f32 x = point->x;
    f32 z = point->z;

    point->x = cosine * x - sine * z;
    point->z = sine * x + cosine * z;
}

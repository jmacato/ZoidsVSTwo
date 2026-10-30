#include "ground_collision.h"

extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 atan2f(f32 y, f32 x);

void sColGetRxRz(Vec3 *normal, f32 heading, f32 *rx, f32 *rz)
{
    f32 normal_y;
    f32 x;
    f32 z;
    f32 side;
    f32 y;
    f32 forward;

    x = -normal->x;
    z = normal->z;
    side = x;
    normal_y = normal->y;
    y = normal_y;
    forward = z;
    side = x * cosf(heading) + z * sinf(heading);
    forward = -x * sinf(heading) + z * cosf(heading);
    *rz = atan2f(side, y);
    *rx = atan2f(forward, y);
    /* MWCC counts this write before removing the unused assignment. */
    heading = 0;
}

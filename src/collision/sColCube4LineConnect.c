#include "types.h"

extern s32 sColCube4Point(Vec3 *point);
extern f32 cube4im(Vec3 *start, Vec3 *end, s32 axis);
extern f32 cube4ip(Vec3 *start, Vec3 *end, s32 axis);

f32 sColCube4LineConnect(Vec3 *start, Vec3 *end)
{
    f32 fraction;
    s32 i;

    if (sColCube4Point(start) != 0)
        return 0.0f;
    fraction = -1.0f;
    for (i = 0; i < 3; i++) {
        fraction = cube4im(start, end, i);
        if (fraction != -1.0f)
            return fraction;
    }
    for (i = 0; i < 3; i++) {
        fraction = cube4ip(start, end, i);
        if (fraction != -1.0f)
            return fraction;
    }
    return fraction;
}

#include "types.h"

extern s32 sColCube4Point(Vec3 *point);
extern s32 sCubeInPoly0(s32 count, Vec3 *vertices, s32 axis, f32 plane);

s32 sCubeInPoly(s32 count, Vec3 *vertices)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (sColCube4Point(&vertices[i]))
            return 1;
    }
    if (sCubeInPoly0(count, vertices, 0, 0.5F))
        return 1;
    if (sCubeInPoly0(count, vertices, 0, -0.5F))
        return 1;
    if (sCubeInPoly0(count, vertices, 1, 0.5F))
        return 1;
    if (sCubeInPoly0(count, vertices, 1, -0.5F))
        return 1;
    if (sCubeInPoly0(count, vertices, 2, 0.5F))
        return 1;
    if (sCubeInPoly0(count, vertices, 2, -0.5F))
        return 1;
    return 0;
}

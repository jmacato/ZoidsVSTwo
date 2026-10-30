#include "types.h"

extern s32 sDivLineVn(Vec3 *start, Vec3 *end, s32 axis, f32 value, Vec3 *out);
extern s32 sColCube4Point(Vec3 *point);

s32 sCubeInPoly0(s32 count, Vec3 *vertices, s32 axis, f32 plane)
{
    s32 axis1 = 1;
    s32 i;
    s32 below;
    s32 axis2 = 2;
    Vec3 cross[4];
    s32 above;
    Vec3 point;

    if (axis == 1)
        axis1 = 0;
    if (axis == 2)
        axis2 = 0;
    below = 0;
    above = 0;
    for (i = 0; i < count; i++) {
        if ((&vertices[i].x)[axis] < plane)
            below++;
        else
            above++;
    }
    if (below == count)
        return 0;
    if (above == count)
        return 0;
    below = -1;
    above = -1;
    i = 0;
    for (; i < count; i++) {
        if (sDivLineVn(&vertices[i], &vertices[(i + 1) % count], axis, plane, &cross[i])) {
            if (sColCube4Point(&cross[i]))
                return 1;
            if (below == -1)
                below = i;
            else if (above == -1)
                above = i;
        }
    }
    if (below == -1 || above == -1)
        return 0;
    if (sDivLineVn(&cross[below], &cross[above], axis1, 0.5f, &point)) {
        if (sColCube4Point(&point))
            return 1;
    } else if (sDivLineVn(&cross[below], &cross[above], axis1, -0.5f, &point) && sColCube4Point(&point)) {
        return 1;
    }
    if (sDivLineVn(&cross[below], &cross[above], axis2, 0.5f, &point)) {
        if (sColCube4Point(&point))
            return 1;
    } else if (sDivLineVn(&cross[below], &cross[above], axis2, -0.5f, &point) && sColCube4Point(&point)) {
        return 1;
    }
    return 0;
}

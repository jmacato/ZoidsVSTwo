#include "types.h"

extern void sLineRotYX(Vec3 *origin, Vec3 *end, Vec3 *point);
extern s32 sLineSurface00(f32 edge_x, f32 edge_y, f32 offset_x, f32 offset_y);

s32 sLineSurface(Vec3 *start, Vec3 *end, Vec3 *points, s32 count)
{
    s32 i;
    s32 negative_edges;

    for (i = 0; i < count; i++)
        sLineRotYX(start, end, &points[i]);
    for (i = 0, negative_edges = 0; i < count; i++) {
        Vec3 *current = &points[i];
        Vec3 *next = &points[(i + 1) % count];
        f32 dx;
        f32 dy;

        dx = next->x - current->x;
        dy = next->y - current->y;
        if (sLineSurface00(dx, dy, -current->x, -current->y) == 0)
            negative_edges++;
    }
    if (negative_edges == 0 || negative_edges == count)
        return 1;
    return 0;
}

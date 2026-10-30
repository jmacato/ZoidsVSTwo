#include "ground_collision.h"

#define NULL 0

extern s32 sLineSurface(Vec3 *start, Vec3 *end, Vec3 *points, s32 count);

static inline f32 fabsf(f32 value)
{
    return (*(u32 *)&value &= 0x7FFFFFFF, value);
}

f32 sColPt00(Vec3 *start, Vec3 *end, GroundPolygon *polygon, GroundMask *mask)
{
    Vec3 pad_stack[2];
    Vec3 points[4];
    GroundMask *material;
    s32 material_index;
    f32 start_distance;
    f32 end_distance;
    f32 fraction;
    s32 i;
    s32 j;

    if (polygon == (void *)0)
        return GROUND_NO_HIT;
    material_index = polygon->material;
    material = &grdcMat[material_index];
    if (!(material->bits[0] & mask->bits[0]) && !(material->bits[1] & mask->bits[1]) &&
        !(material->bits[2] & mask->bits[2]))
        return GROUND_NO_HIT;
    start_distance = polygon->plane_offset +
                     (polygon->normal.x * start->x + polygon->normal.y * start->y + polygon->normal.z * start->z);
    end_distance = polygon->plane_offset +
                   (polygon->normal.x * end->x + polygon->normal.y * end->y + polygon->normal.z * end->z);
    if (start_distance * end_distance > 0.0f)
        return GROUND_NO_HIT;
    if (0.0f == start_distance)
        return 0.0f;
    if (0.0f == end_distance)
        return 1.0f;
    fraction = fabsf(start_distance) / (fabsf(start_distance) + fabsf(end_distance));
    for (i = 0, j = 0; i < 3; i++) {
        if ((&start->x)[i] == (&end->x)[i])
            j++;
    }
    if (j == 3)
        return GROUND_NO_HIT;
    for (i = 0; i < polygon->vertex_count; i++) {
        for (j = 0; j < 3; j++)
            (&points[i].x)[j] = (&grdcVtx[polygon->vertices[i]].x)[j];
    }
    if (sLineSurface(start, end, points, polygon->vertex_count) == 0)
        return GROUND_NO_HIT;
    return fraction;
}

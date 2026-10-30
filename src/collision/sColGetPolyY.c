#include "ground_collision.h"

f32 sColGetPolyY(u16 index, Vec3 *point)
{
    GroundPolygon *polygon = &grdcPolygon[index];
    f32 x = point->x;
    f32 ny = polygon->normal.y;
    f32 z = point->z;
    f32 y = point->y;

    if (ny == 0.0f) {
        return y;
    } else {
        y = (polygon->normal.x * x + polygon->normal.z * z + polygon->plane_offset) / ny;
        return -y;
    }
}

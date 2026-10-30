#include "ground_collision.h"

s32 sColPosPolygonON(Vec3 *point, u16 index)
{
    GroundPolygon *polygon = &grdcPolygon[index];
    s32 i;
    s32 count;
    u16 *vertices;
    f32 first_x, first_z;
    f32 point_x, point_z;
    s32 material = polygon->material;
    s32 normal_index = polygon->normal_index;
    s32 dead_a;
    s32 dead_b;

    count = polygon->vertex_count;
    vertices = polygon->vertices;
    if (index == GROUND_POLYGON_NONE)
        return 0;
    dead_a = 0;
    dead_b = 0;
    point_x = point->x;
    point_z = point->z;
    for (i = 0; i < count; i++) {
        s32 first = vertices[i];
        s32 second = vertices[(i + 1) % count];
        f32 edge_x, edge_z, x, z, cross;
        first_x = grdcVtx[first].x;
        x = point_x - first_x;
        edge_x = grdcVtx[second].x - first_x;
        first_z = grdcVtx[first].z;
        edge_z = grdcVtx[second].z - first_z;
        z = point_z - first_z;
        cross = x * edge_z - z * edge_x;
        if (cross > 0.f)
            return 0;
    }
    return 1;
}

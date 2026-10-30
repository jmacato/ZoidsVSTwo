#include "ground_collision.h"

void sColGetPolygonCenter(u16 index, f32 *center)
{
    GroundPolygon *polygon;
    s32 vertex;
    Vec3 *vertex_pos;
    s32 count;
    s32 i;
    s32 j;


    if (index == GROUND_POLYGON_NONE)
        return;
    if (center == 0)
        return;
    polygon = &grdcPolygon[index];
    count = polygon->vertex_count;
    if (count <= 0)
        return;
    for (j = 0; j < 3; j++) {
        center[j] = 0.0f;
    }
    for (i = 0; i < count; i++) {
        vertex = polygon->vertices[i];
        vertex_pos = &grdcVtx[vertex];

        for (j = 0; j < 3; j++) {
            center[j] += ((f32 *)vertex_pos)[j];
        }
    }
    for (j = 0; j < 3; j++) {
        center[j] /= count;
    }
}

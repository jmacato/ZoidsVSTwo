#include "ground_collision.h"

Vec3 *grdcGetNormal(u16 index)
{
    u16 normal_index;

    if (index == GROUND_POLYGON_NONE)
        return 0;
    normal_index = grdcPolygon[index].normal_index;
    if (normal_index == GROUND_POLYGON_NONE)
        return 0;
    return &grdcNormal[normal_index];
}

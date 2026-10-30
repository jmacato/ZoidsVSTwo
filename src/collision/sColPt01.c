#include "ground_collision.h"

f32 sColPt01(Vec3 *start, Vec3 *end, u16 *indices, GroundMask *mask)
{
    GroundMask mask_copy;
    GroundPolygon *polygon;
    f32 result;

    if (indices == (void *)0)
        return 10000.0f;
    while (*indices != 0xFFFF) {
        polygon = &grdcPolygon[*indices];
        mask_copy = *mask;
        result = sColPt00(start, end, polygon, &mask_copy);
        if (10000.0f != result)
            return result;
        indices++;
    }
    return 10000.0f;
}

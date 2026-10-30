#include "ground_collision.h"

f32 sColRealPt01(Vec3 *start, Vec3 *end, u16 *indices, GroundMask *mask)
{
    GroundPolygon *polygon;
    GroundMask mask_copy;
    f32 fraction;
    f32 best;

    best = GROUND_NO_HIT;
    if (indices == (void *)0)
        return GROUND_NO_HIT;
    while (*indices != GROUND_POLYGON_NONE) {
        polygon = &grdcPolygon[*indices];
        mask_copy = *mask;
        fraction = sColPt00(start, end, polygon, &mask_copy);
        if (fraction != GROUND_NO_HIT) {
            if (best > fraction || best == GROUND_NO_HIT)
                best = fraction;
        }
        indices++;
    }
    return best;
}

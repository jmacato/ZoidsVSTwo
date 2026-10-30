#include "ground_collision.h"

#pragma dont_inline on
f32 sColPt01(Vec3 *start, Vec3 *end, u16 *indices, GroundMask mask);

f32 sColPt(Vec3 *start, Vec3 *end, GroundMask *mask)
{
    u16 *start_indices = sColPosTBL(start);
    u16 *end_indices = sColPosTBL(end);
    f32 fraction = sColPt01(start, end, start_indices, *mask);
    if (fraction != GROUND_NO_HIT)
        return fraction;
    if (start_indices == end_indices)
        return GROUND_NO_HIT;
    {
        return sColPt01(start, end, end_indices, *mask);
    }
}





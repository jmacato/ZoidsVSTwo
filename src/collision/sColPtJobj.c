#include "ground_collision.h"

f32 sColPtJobj(u8 *a, u8 *b, GroundMask *mask)
{
    f32 start[3];
    f32 end[3];
    GroundMask mask_copy;
    s32 i;

    if (a == 0)
        return GROUND_NO_HIT;
    if (b == 0)
        return GROUND_NO_HIT;
    for (i = 0; i < 3; i++) {
        start[i] = *(f32 *)(a + 0x50 + i * 16);
        end[i] = *(f32 *)(b + 0x50 + i * 16);
    }
    mask_copy = *mask;
    return sColPt((Vec3 *)start, (Vec3 *)end, &mask_copy);
}

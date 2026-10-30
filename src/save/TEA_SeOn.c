#include "types.h"

extern u32 TEA_SeOnF(u16 sfx, f32 x, f32 z);

u32 TEA_SeOn(u16 sfx, f32 x, f32 z)
{
    return TEA_SeOnF(sfx, x, z);
}

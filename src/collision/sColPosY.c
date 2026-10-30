#include "ground_collision.h"

f32 sColPosY(Vec3 *point, u16 *selector)
{
    GroundMask mask = (GroundMask){{255, 255, 255, 255}};
    return sColPosAndY(point, selector, &mask);
}

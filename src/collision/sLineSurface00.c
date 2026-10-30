#include "types.h"

s32 sLineSurface00(f32 edge_x, f32 edge_y, f32 offset_x, f32 offset_y)
{
    if (((offset_y * edge_x) - (offset_x * edge_y)) < 0.0f) {
        return 0;
    }
    return 1;
}

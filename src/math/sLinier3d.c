#include "types.h"

extern f32 sLiner(f32 target, f32 current, f32 fraction);

void sLinier3d(f32 *current, f32 *target, f32 fraction)
{
    s32 axis;

    for (axis = 0; axis < 3; axis++) {
        current[axis] = sLiner(target[axis], current[axis], fraction);
    }
}

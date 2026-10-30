#include "types.h"

f32 sLiner(f32 target, f32 current, f32 fraction)
{
    f32 difference = target - current;

    return difference * fraction + current;
}

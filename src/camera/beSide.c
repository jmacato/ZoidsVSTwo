#include "types.h"

f32 beSide(f32 current, f32 target, f32 step)
{
    f32 value;

    value = current;
    if (value > target) {
        value -= step;
        if (value < target) {
            value = target;
        }
    }
    if (value < target) {
        value += step;
        if (value > target) {
            return target;
        }
    }
    return value;
}

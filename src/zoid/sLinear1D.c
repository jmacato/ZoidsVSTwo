#include "types.h"

void sLinear1D(f32 *value, f32 target, f32 step)
{
    f32 current;

    if (value == 0) {
        return;
    }
    current = *value;
    if (step < 0.0f) {
        step = -step;
    }
    if (0.0f == step) {
        return;
    }
    if (current == target) {
        return;
    }
    if (current < target) {
        current += step;
        if (current > target) {
            current = target;
        }
    }
    if (current > target) {
        current -= step;
        if (current < target) {
            current = target;
        }
    }
    *value = current;
}

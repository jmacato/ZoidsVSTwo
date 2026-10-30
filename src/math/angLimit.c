#include "types.h"

void angLimit(f32 *angle)
{
    f32 value = *angle;

    while (1) {
        if (value > 3.1415926) {
            value -= 6.2831852;
        } else if (value < -3.1415926) {
            value += 6.2831852;
        } else {
            break;
        }
    }
    *angle = value;
}

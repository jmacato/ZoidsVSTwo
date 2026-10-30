/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/spline.c. */
#include "types.h"

f32 splGetHelmite(f32 inverse_length, f32 time, f32 value_0, f32 value_1, f32 slope_0,
                  f32 slope_1)
{
    f32 scaled_square;
    f32 scaled_cube;
    f32 cube_over_length_squared;
    f32 square_over_length;
    f32 inverse_length_squared;
    f32 time_squared;

    time_squared = time * time;
    inverse_length_squared = inverse_length * inverse_length;
    square_over_length = time_squared * inverse_length;
    cube_over_length_squared = inverse_length_squared * (time_squared * time);
    scaled_cube = 2.0f * cube_over_length_squared * inverse_length;
    scaled_square = 3.0f * time_squared * inverse_length_squared;

    return slope_1 * (cube_over_length_squared - square_over_length) +
           (slope_0 * (time + ((cube_over_length_squared - square_over_length) -
                               square_over_length)) +
            (value_0 * (1.0f + (scaled_cube - scaled_square)) +
             value_1 * (-scaled_cube + scaled_square)));
}

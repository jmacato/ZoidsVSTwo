/* References:
 * https://github.com/doldecomp/melee/blob/master/src/MSL/trigf.c
 * https://github.com/doldecomp/melee/blob/master/src/MSL/math.h
 */
#pragma cplusplus on
#include "runtime_math.h"
extern "C" float cosf(float);
extern "C" float sinf(float);
extern float __fabsf(float);
#pragma dont_inline on
inline float fabsf(float x)
{
    return __fabsf(x);
}
#define M_PI 3.14159265358979323846
#define __epsilon 3.45266983e-4f
extern f32 __sincos_on_quadrant[];
extern f32 __sincos_poly[];
static const f32 tmp_float[] = {0.25f, 0.0232393741608f, 1.70555722434e-7f,
                                1.86736494323e-11f};
static f32 __four_over_pi_m1[] = {tmp_float[0], tmp_float[1], tmp_float[2],
                                  tmp_float[3]};
f32 sinf(f32 x)
{
    int n;
    f32 y;
    f32 ysq;
    f32 z;

    z = (2.0f / (f32)M_PI) * x;
    n = (__HI(x) & 0x80000000) ? (int)(z - 0.5f) : (int)(z + 0.5f);

    y = x - n * 2 + __four_over_pi_m1[0] * x + __four_over_pi_m1[1] * x +
        __four_over_pi_m1[2] * x + __four_over_pi_m1[3] * x;
    n &= 3;

    if (fabsf(y) < __epsilon) {
        n <<= 1;
        return __sincos_on_quadrant[n] +
               (__sincos_on_quadrant[n + 1] * y * __sincos_poly[9]);
    }

    ysq = y * y;
    if (n & 1) {
        n <<= 1;
        z = (((__sincos_poly[0] * ysq + __sincos_poly[2]) * ysq + __sincos_poly[4]) *
                 ysq +
             __sincos_poly[6]) *
                ysq +
            __sincos_poly[8];

        return z * __sincos_on_quadrant[n];
    } else {
        n <<= 1;
        z = ((((__sincos_poly[1] * ysq + __sincos_poly[3]) * ysq + __sincos_poly[5]) *
                  ysq +
              __sincos_poly[7]) *
                 ysq +
             __sincos_poly[9]) *
            y;
        return z * __sincos_on_quadrant[n + 1];
    }
}

f32 cosf(f32 x)
{
    int n;
    f32 y;
    f32 ysq;
    f32 z;

    z = (2.0f / (f32)M_PI) * x;
    n = (__HI(x) & 0x80000000) ? (int)(z - 0.5f) : (int)(z + 0.5f);

    y = x - n * 2 + __four_over_pi_m1[0] * x + __four_over_pi_m1[1] * x +
        __four_over_pi_m1[2] * x + __four_over_pi_m1[3] * x;
    n &= 3;
    if (fabsf(y) < __epsilon) {
        n <<= 1;
        return __sincos_on_quadrant[n + 1] - y * __sincos_on_quadrant[n];
    }

    ysq = y * y;
    if (n & 1) {
        n <<= 1;
        z = -((((__sincos_poly[1] * ysq + __sincos_poly[3]) * ysq + __sincos_poly[5]) *
                   ysq +
               __sincos_poly[7]) *
                  ysq +
              __sincos_poly[9]) *
            y;
        return z * __sincos_on_quadrant[n];
    } else {
        n <<= 1;
        z = (((__sincos_poly[0] * ysq + __sincos_poly[2]) * ysq + __sincos_poly[4]) *
                 ysq +
             __sincos_poly[6]) *
                ysq +
            __sincos_poly[8];
        return z * __sincos_on_quadrant[n + 1];
    }
}

inline f32 sin(f32 x)
{
    return sinf(x);
}

inline f32 cos(f32 x)
{
    return cosf(x);
}

extern "C" float tanf(float x)
{
    return sin(x) / cos(x);
}

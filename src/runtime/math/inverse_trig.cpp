/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/inverse_trig.c.
 */
#include "runtime_math.h"
extern double __frsqrte(double);
#define M_PI 3.14159265358979323846f
#define HALF_PI 1.57079632679489661923f
#define signbit(x) ((int)(*(int *)&(x) & 0x80000000))
extern "C" unsigned long __float_nan[];
extern "C" unsigned long __float_huge[];
#define NAN (*(float *)__float_nan)
#define HUGE_VALF (*(float *)__float_huge)
#define __PI_O2 ((float)M_PI / 2)

extern "C" float atanf(float);
extern "C" float atan2f(float, float);
extern "C" float acosf(float);
extern "C" inline float _inv_sqrtf(float);
inline float atan(float);

float atan2f(float __y, float __x)
{
    if (signbit(__x) == signbit(__y)) {
        if (signbit(__x) != 0) {
            return atanf(__y / __x) - M_PI;
        } else if (__x) {
            return atanf(__y / __x);
        } else {
            return HALF_PI;
        }
    } else if (__x < 0.0f) {
        return M_PI + atanf(__y / __x);
    } else if (__x) {
        return atanf(__y / __x);
    }
    /* 0x3fc90fdb is HALF_PI */
    __HI(__y) = signbit(__y) + 0x3fc90fdb;

    return __y;
}

float acosf(float x)
{
    return HALF_PI - atan(x * _inv_sqrtf(1.0f - x * x));
}

extern "C" float asinf(float x)
{
    return atan(x * _inv_sqrtf(1.0f - x * x));
}

#pragma dont_inline on
inline float _inv_sqrtf(float x)
{
    const float _half = .5f;
    const float _three = 3.0f;

    if (x > 0.0f) {
        float guess = __frsqrte((double)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        return guess;
    } else if (x) {
        return NAN;
    }
    return HUGE_VALF;
}

inline float atan(float x)
{
    return atanf(x);
}

#pragma dont_inline reset

float atanf(float x)
{
    float z, z_square;
    int index = -1, inv = 0;
    const int sign = (*(int *)&x) & 0x80000000;

    static const float atan_coeff[] = {.999999999f,    -.3333333213f, .19999886356f,
                                       -.14281650536f, .11041179874f, -.084597554152f,
                                       .04714243524f};

    static const float onep_one_over_xisqr_hi[] = {
        6.82842f, 3.239828f, 2.0f, 1.446462f, 1.17157292f, 1.039566130f};

    static const float onep_one_over_xisqr_lo[] = {.000007135f, 0.00000082f, .0f,
                                                   0.00000063f, 0.0f,        0.0f};

    static const float atan_xi_hi[] = {0.0f,     .39269f,   .5890486f, .7853981f,
                                       .981747f, 1.178097f, 1.374446f};
    static const float atan_xi_lo[] = {0.0f,        .000009081698724f, .000000023f,
                                       .000000063f, .000000704f,       .00000025f,
                                       .00000079f};

    static const float one_over_xi_hi[] = {2.414213f,   1.49660575f, 1.00000000f,
                                           .668178618f, .414213568f, .198912367f};

    static const float one_over_xi_lo[] = {.000000562f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    (*(int *)&x) &= 0x7fffffff;

    if (x >= 2.414213565f) {
        z = 1.0f / x;
        inv++;
    } else if (.4142135624f < x) {
        index++;
        switch (__HI(x) & 0x7f800000) {
        case 0x3F000000:
            if ((*(int *)&x) >= 0x3F08D5B9)
                index++;
            if ((*(int *)&x) >= 0x3F521801)
                index++;
            break;
            break;
        case 0x3F800000:
            index += 2;
            if ((*(int *)&x) >= 0x3F9bf7ec)
                index++;
            if ((*(int *)&x) >= 0x3FEF789E)
                index++;
            break;
        case 0x40000000:
            index += 4;
            break;
        }

        z = 1.0f / (one_over_xi_hi[index] + (one_over_xi_lo[index] + x));
        z = (one_over_xi_hi[index] - onep_one_over_xisqr_hi[index] * z) +
            (one_over_xi_lo[index] - onep_one_over_xisqr_lo[index] * z);

    } else
        z = x;

    z_square = z * z;
    z += z * z_square *
         (atan_coeff[1] +
          z_square * (atan_coeff[2] +
                      z_square * (atan_coeff[3] +
                                  z_square * (atan_coeff[4] +
                                              z_square * (atan_coeff[5] +
                                                          z_square * atan_coeff[6])))));
    z += atan_xi_lo[index + 1];
    z += atan_xi_hi[index + 1];

    if (inv) {
        z -= __PI_O2;
        if (sign)
            return z;

        return -z;
    }

    (*(int *)&z) |= sign;
    return z;
}

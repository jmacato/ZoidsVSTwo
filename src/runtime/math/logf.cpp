#include "runtime_math.h"

#pragma peephole off

extern "C" unsigned long __float_nan[];
extern "C" unsigned long __float_huge[];
extern "C" float __ln_F[129];
extern "C" float __one_over_F[129];

extern "C" float logf(float x)
{
    switch (__HI(x) & 0x7f800000) {
    default: {
        unsigned long bits = *(unsigned long *)&x;
        float center;
        float normalized;
        float coefficients[2] = {-0.5000028610229492F, 0.33332985639572144F};
        int exponent = (bits >> 23) - 127;
        unsigned long mantissa = bits & 0x7fffff;
        unsigned long index = mantissa >> 16;
        float residual;
        float polynomial;
        float logarithm;

        if ((bits & 0xffff) != 0) {
            __HI(center) = (bits & 0x7f0000) | 0x3f800000;
            __HI(normalized) = mantissa | 0x3f800000;
            if ((bits & 0x8000) != 0) {
                index++;
                __HI(center) += 0x10000;
            }
            residual = normalized - center;
            residual *= __one_over_F[index];
            logarithm = __fmadds((float)exponent, 0.6931471824645996F, __ln_F[index]);
            polynomial = residual * residual * (residual * coefficients[1] + coefficients[0]);
            return logarithm + (residual + polynomial);
        }
        return __fmadds((float)exponent, 0.6931471824645996F, __ln_F[index]);
    }
    case 0x7f800000:
        if ((__HI(x) & 0x7fffff) != 0)
            return x;
        if ((__HI(x) & 0x80000000) != 0)
            return *(float *)__float_nan;
        return *(float *)__float_huge;
    case 0:
        return -*(float *)__float_huge;
    }
}

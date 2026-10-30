#include "runtime_math.h"
int __fpclassifyd(double __value)
{
    switch (__HI(__value) & 0x7ff00000) {
    case 0x7ff00000: {
        if ((__HI(__value) & 0x000fffff) || (__LO(__value) & 0xffffffff))
            return FP_QNAN;
        else
            return FP_INFINITE;
        break;
    }
    case 0: {
        if ((__HI(__value) & 0x000fffff) || (__LO(__value) & 0xffffffff))
            return FP_SUBNORMAL;
        else
            return FP_ZERO;
        break;
    }
    }
    return FP_NORMAL;
}

double fabs(double value)
{
    return __fabs(value);
}

/* Reference: https://github.com/doldecomp/melee/blob/master/src/MSL/math.h. */
#include "types.h"

extern f64 __fabs(f64 value);

f32 fmodf__Fff(f32 numerator, f32 denominator)
{
    s64 quotient;

    if (__fabs(denominator) > __fabs(numerator)) {
        return numerator;
    }
    quotient = numerator / denominator;
    return numerator - denominator * (f32)quotient;
}

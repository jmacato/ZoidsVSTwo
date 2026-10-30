#include "runtime_math.h"
float floorf(float x)
{
    int integral = (int)x;
    float difference = (float)(int)x - x;
    if (__HI(difference) != 0) {
        if ((__HI(x) & 0x7f800000) >= 0x4b800000)
            return x;
        if (__HI(x) & 0x80000000) {
            --integral;
            return (float)integral;
        } else
            return (float)integral;
    }
    return x;
}

#include "runtime_math.h"

double scalbn(double x, int n)
{
    return ldexp(x, n);
}

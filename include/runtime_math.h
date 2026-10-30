#ifndef ZOIDS_RUNTIME_MATH_H
#define ZOIDS_RUNTIME_MATH_H

#include "types.h"
#define __HI(x) (*(int *)&(x))
#define __LO(x) (*((int *)&(x) + 1))
#define __HIp(x) (*(int *)(x))
#define __LOp(x) (*((int *)(x) + 1))
#define __P(p) p
#define FP_QNAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5
extern double __fabs(double);
#ifdef __cplusplus
extern "C" {
#endif
extern double copysign(double, double);
extern double floor(double);
extern float floorf(float);
extern double ldexp(double, int);
extern double scalbn(double, int);
extern double frexp(double, int *);
extern double cos(double);
extern double sin(double);
extern int __ieee754_rem_pio2(double, double *);
extern int __kernel_rem_pio2(double *, double *, int, int, int, const int *);
extern double __kernel_cos(double, double);
extern double __kernel_sin(double, double, int);

#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
int __fpclassifyd(double);
double fabs(double);
#endif

#endif

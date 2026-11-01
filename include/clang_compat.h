#ifndef ZOIDS_CLANG_COMPAT_H
#define ZOIDS_CLANG_COMPAT_H

#define __declspec(x)

static inline double zc_frsqrte(double x)
{
    double r;
    __asm__("frsqrte %0, %1" : "=f"(r) : "f"(x));
    return r;
}
static inline float zc_fres(float x)
{
    float r;
    __asm__("fres %0, %1" : "=f"(r) : "f"(x));
    return r;
}
static inline float zc_fmadds(float a, float b, float c)
{
    float r;
    __asm__("fmadds %0, %1, %2, %3" : "=f"(r) : "f"(a), "f"(b), "f"(c));
    return r;
}
static inline int zc_cntlzw(unsigned int x)
{
    return x ? __builtin_clz(x) : 32;
}
static inline void zc_dcbz(void *base, int offset)
{
    __asm__ volatile("dcbz %0, %1" : : "b"(base), "r"(offset) : "memory");
}

#define __frsqrte(x) zc_frsqrte(x)
#define __fres(x) zc_fres(x)
#define __fmadds(a, b, c) zc_fmadds(a, b, c)
#define __cntlzw(x) zc_cntlzw(x)
#define __dcbz(base, offset) zc_dcbz((void *)(base), (int)(offset))
#define __fabs(x) __builtin_fabs(x)
#define __fabsf(x) __builtin_fabsf(x)
#define __sync() __asm__ volatile("sync" : : : "memory")
#define __isync() __asm__ volatile("isync" : : : "memory")

#endif

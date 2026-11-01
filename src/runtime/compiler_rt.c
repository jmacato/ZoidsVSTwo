typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

static u64 udivmod(u64 n, u64 d, u64 *remainder)
{
    u64 q = 0;
    u64 r = 0;
    int i;

    for (i = 63; i >= 0; i--) {
        r = (r << 1) | ((n >> i) & 1);
        if (r >= d) {
            r -= d;
            q |= (u64)1 << i;
        }
    }
    if (remainder)
        *remainder = r;
    return q;
}

u64 __udivdi3(u64 n, u64 d)
{
    return udivmod(n, d, 0);
}

u64 __umoddi3(u64 n, u64 d)
{
    u64 r;

    udivmod(n, d, &r);
    return r;
}

s64 __divdi3(s64 n, s64 d)
{
    int negative = (n < 0) != (d < 0);
    u64 q = udivmod(n < 0 ? -(u64)n : (u64)n, d < 0 ? -(u64)d : (u64)d, 0);

    return negative ? -(s64)q : (s64)q;
}

s64 __moddi3(s64 n, s64 d)
{
    u64 r;

    udivmod(n < 0 ? -(u64)n : (u64)n, d < 0 ? -(u64)d : (u64)d, &r);
    return n < 0 ? -(s64)r : (s64)r;
}

double __floatundidf(u64 x)
{
    return (double)(u32)(x >> 32) * 4294967296.0 + (double)(u32)x;
}

double __floatdidf(s64 x)
{
    return x < 0 ? -__floatundidf(-(u64)x) : __floatundidf((u64)x);
}

float __floatundisf(u64 x)
{
    return (float)__floatundidf(x);
}

float __floatdisf(s64 x)
{
    return (float)__floatdidf(x);
}

u64 __fixunsdfdi(double x)
{
    u32 high;
    u32 low;

    if (!(x > 0.0))
        return 0;
    if (x >= 18446744073709551616.0)
        return ~(u64)0;
    high = (u32)(x / 4294967296.0);
    low = (u32)(x - (double)high * 4294967296.0);
    return ((u64)high << 32) | low;
}

s64 __fixdfdi(double x)
{
    return x < 0.0 ? -(s64)__fixunsdfdi(-x) : (s64)__fixunsdfdi(x);
}

u64 __fixunssfdi(float x)
{
    return __fixunsdfdi(x);
}

s64 __fixsfdi(float x)
{
    return __fixdfdi(x);
}

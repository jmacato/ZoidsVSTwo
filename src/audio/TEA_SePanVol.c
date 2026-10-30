#include "types.h"

extern s16 TEA_SEVOL;
extern f64 __frsqrte(f64);

static inline f32 sqrtf(f32 x, volatile f32 *y)
{
    static const f64 _half = 0.5;
    static const f64 _three = 3.0;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        *y = (f32)(x * guess);
        return *y;
    }
    return x;
}

void TEA_SePanVol(s8 *pan_out, u8 *vol_out, f32 x, f32 z)
{
    s32 inside = 1;
    s32 pan;
    f32 dist;
    f32 vol;
    f32 level;
    u8 unused = 10;
    volatile f32 root;

    dist = sqrtf(x * x + z * z, &root);
    if (dist > 500.0f)
        inside = 0;
    if (dist < 1.0f)
        x = 0.0f;
    else
        x = x / dist;
    pan = 64.0f * x + 64.0f;
    if (pan > 127)
        pan = 127;
    if (pan < 0)
        pan = 0;
    *pan_out = pan;
    level = 500.0f - dist - 5.0f;
    vol = level * TEA_SEVOL / 495.0f;
    if (vol > 127.0f)
        vol = 127.0f;
    if (vol < 0.0f)
        vol = 0.0f;
    if (inside == 0)
        vol = 0.0f;
    *vol_out = vol;
}

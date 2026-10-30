#include "musyx_adsr.h"

extern u32 salChangeADSRState(ADSR_VARS *adsr);
extern u32 adsrStartRelease(ADSR_VARS *adsr, u32 time);
extern f32 powf(f32 x, f32 y);

u32 adsrConvertTimeCents(s32 cents)
{
    return 1000.0f * powf(2.0f, 1.2715658e-08f * cents);
}

u32 adsrSetup(ADSR_VARS *adsr)
{
    adsr->state = ADSR_STATE_START;
    return salChangeADSRState(adsr);
}

u32 adsrRelease(ADSR_VARS *adsr)
{
    switch (adsr->mode) {
    case ADSR_MODE_LINEAR:
    case ADSR_MODE_DLS:
        return adsrStartRelease(adsr, adsr->releaseTime);
    }
    return 0;
}

#pragma auto_inline off
const u16 dspAttenuationTab[194] = {
    32767, 30934, 29203, 27569, 26027, 24571, 23197, 21899, 20674, 19518, 18426, 17395, 16422, 15503, 14636, 13817,
    13044, 12315, 11626, 10975, 10361, 9782, 9234, 8718, 8230, 7770, 7335, 6925, 6537, 6172, 5826, 5500,
    5193, 4902, 4628, 4369, 4125, 3894, 3676, 3470, 3276, 3093, 2920, 2757, 2602, 2457, 2319, 2189,
    2067, 1951, 1842, 1739, 1642, 1550, 1463, 1381, 1304, 1231, 1162, 1097, 1036, 978, 923, 871,
    823, 777, 733, 692, 653, 617, 582, 550, 519, 490, 462, 436, 412, 389, 367, 347,
    327, 309, 292, 275, 260, 245, 231, 218, 206, 195, 184, 173, 164, 155, 146, 138,
    130, 123, 116, 109, 103, 97, 92, 87, 82, 77, 73, 69, 65, 61, 58, 55,
    51, 49, 46, 43, 41, 38, 36, 34, 32, 30, 29, 27, 26, 24, 23, 21,
    20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 10, 10, 9, 9, 8,
    8, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3,
    3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

const u8 dspScale2IndexTab[1024] = {
    193, 120, 108, 101, 96, 92, 89, 87, 84, 82, 80, 79, 77, 76, 75, 73,
    72, 71, 70, 69, 68, 68, 67, 66, 65, 64, 64, 63, 63, 62, 61, 61,
    60, 60, 59, 59, 58, 58, 57, 57, 56, 56, 55, 55, 55, 54, 54, 54,
    53, 53, 52, 52, 52, 51, 51, 51, 50, 50, 50, 50, 49, 49, 49, 48,
    48, 48, 48, 47, 47, 47, 47, 46, 46, 46, 46, 45, 45, 45, 45, 44,
    44, 44, 44, 44, 43, 43, 43, 43, 43, 42, 42, 42, 42, 42, 41, 41,
    41, 41, 41, 41, 40, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 39,
    38, 38, 38, 38, 38, 38, 38, 37, 37, 37, 37, 37, 37, 37, 36, 36,
    36, 36, 36, 36, 36, 35, 35, 35, 35, 35, 35, 35, 35, 34, 34, 34,
    34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 33, 33, 33, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29,
    29, 29, 29, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 18, 18, 18, 18, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u32 salChangeADSRState(ADSR_VARS *adsr)
{
    u32 result = 0;
    switch (adsr->mode) {
    case ADSR_MODE_LINEAR:
        switch (adsr->state) {
        case ADSR_STATE_START:
            if ((adsr->count = adsr->attackTime) != 0) {
                adsr->state = ADSR_STATE_ATTACK;
                adsr->volume = 0;
                adsr->delta = 0x7FFF0000UL / adsr->attackTime;
                break;
            }
        case ADSR_STATE_ATTACK:
            if ((adsr->count = adsr->decayTime) != 0) {
                adsr->state = ADSR_STATE_DECAY;
                adsr->volume = 0x7FFF0000;
                adsr->delta = -((0x7FFF0000UL - ((u32)adsr->sustain << 16)) / adsr->decayTime);
                break;
            }
        case ADSR_STATE_DECAY:
            if (adsr->sustain) {
                adsr->state = ADSR_STATE_SUSTAIN;
                adsr->volume = (u32)adsr->sustain << 16;
                adsr->delta = 0;
                break;
            }
        case ADSR_STATE_RELEASE:
            adsr->volume = 0;
            result = 1;
            break;
        }
        break;
    case ADSR_MODE_DLS:
        switch (adsr->state) {
        case ADSR_STATE_START:
            if ((adsr->count = adsr->attackTime) != 0) {
                adsr->state = ADSR_STATE_ATTACK;
                if (!adsr->attackMode) {
                    adsr->volume = 0;
                    adsr->delta = 0x7FFF0000UL / adsr->count;
                } else {
                    adsr->index = 0;
                    adsr->volume = 0;
                    adsr->delta = 0xC10000UL / adsr->count;
                }
                break;
            }
        case ADSR_STATE_ATTACK:
            adsr->count = (adsr->decayTime * (((193UL - adsr->sustain) << 16) / 193)) >> 16;
            if (adsr->count) {
                adsr->state = ADSR_STATE_DECAY;
                adsr->volume = 0x7FFF0000;
                adsr->index = 0xC10000;
                adsr->delta = -(((193UL - adsr->sustain) << 16) / adsr->count);
                break;
            }
        case ADSR_STATE_DECAY:
            if (adsr->sustain) {
                s32 index;
                adsr->state = ADSR_STATE_SUSTAIN;
                adsr->index = (u32)adsr->sustain << 16;
                if ((index = 193 - ((adsr->index + 0x8000) >> 16)) < 0)
                    index = 0;
                adsr->volume = (u32)dspAttenuationTab[index] << 16;
                adsr->delta = 0;
                break;
            }
        case ADSR_STATE_RELEASE:
            adsr->volume = 0;
            result = 1;
            break;
        }
        break;
    }
    return result;
}
u32 adsrStartRelease(ADSR_VARS *adsr, u32 time)
{
    switch (adsr->mode) {
    case ADSR_MODE_LINEAR:
        adsr->state = ADSR_STATE_RELEASE;
        adsr->count = time;
        if (!time) {
            adsr->count = 1;
            adsr->delta = 0;
            return 1;
        }
        adsr->delta = -((u32)adsr->volume / time);
        break;
    case ADSR_MODE_DLS:
        if (!adsr->attackMode && adsr->state == ADSR_STATE_ATTACK)
            adsr->index = (193 - dspScale2IndexTab[adsr->volume >> 21]) << 16;
        adsr->count = (u32)(0.000323834189f * adsr->index * time) >> 12;
        adsr->state = ADSR_STATE_RELEASE;
        if (!adsr->count) {
            adsr->count = 1;
            adsr->volume = 0;
            adsr->index = 0;
            adsr->delta = 0;
            return 1;
        }
        adsr->delta = -((u32)adsr->index / adsr->count);
        break;
    }
    return 0;
}

u32 adsrHandle(ADSR_VARS *adsr, u16 *volume, u16 *delta)
{
    u32 result = 0;
    s32 oldVolume;
    s32 change;
    switch (adsr->mode) {
    case ADSR_MODE_LINEAR:
        if (adsr->state != ADSR_STATE_SUSTAIN) {
            oldVolume = adsr->volume;
            adsr->volume += adsr->delta;
            *volume = oldVolume >> 16;
            if (adsr->delta >= 0)
                *delta = adsr->delta >> 21;
            else
                *delta = -((-adsr->delta) >> 21);
            if (--adsr->count == 0)
                result = salChangeADSRState(adsr);
        } else {
            *volume = adsr->volume >> 16;
            *delta = 0;
        }
        break;
    case ADSR_MODE_DLS:
        if (adsr->state != ADSR_STATE_SUSTAIN) {
            oldVolume = adsr->volume;
            if (!adsr->attackMode && adsr->state == ADSR_STATE_ATTACK) {
                adsr->volume += adsr->delta;
            } else {
                s32 index;
                adsr->index += adsr->delta;
                if ((index = 193 - ((adsr->index + 0x8000) >> 16)) < 0)
                    index = 0;
                adsr->volume = (u32)dspAttenuationTab[index] << 16;
            }
            *volume = oldVolume >> 16;
            change = adsr->volume - oldVolume;
            if (change >= 0)
                *delta = change >> 21;
            else
                *delta = -((-change) >> 21);
            if (--adsr->count == 0)
                result = salChangeADSRState(adsr);
        } else {
            *volume = adsr->volume >> 16;
            *delta = 0;
        }
        break;
    }
    return result;
}
u32 adsrHandleLowPrecision(ADSR_VARS *adsr, u16 *volume, u16 *delta)
{
    u8 index;
    for (index = 0; index < 15; index++) {
        if (adsrHandle(adsr, volume, delta))
            return 1;
    }
    return 0;
}

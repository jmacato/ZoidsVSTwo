/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/controller.c.
 */
#include "controller.h"
extern int OSDisableInterrupts(void);
extern int OSRestoreInterrupts(int enabled);
extern int OSGetResetSwitchState(void);
extern u32 PADRead(PADStatus *status);
extern int PADReset(u32 mask);
extern void HSD_PadRumbleInterpret(void);
extern f32 atan2f(f32 y, f32 x);
extern f64 __frsqrte(f64 value);
#define PAD_BUTTON_LEFT 1
#define PAD_BUTTON_RIGHT 2
#define PAD_BUTTON_DOWN 4
#define PAD_BUTTON_UP 8
#define PAD_CHAN0_BIT 0x80000000
#define PAD_CHAN1_BIT 0x40000000
#define PAD_CHAN2_BIT 0x20000000
#define PAD_CHAN3_BIT 0x10000000
#define PAD_ERR_NONE 0
#define PAD_ERR_NO_CONTROLLER -1
#define PAD_ERR_TRANSFER -3
HSD_PadStatus HSD_PadCopyStatus[4];
HSD_PadStatus HSD_PadMasterStatus[4];
PadLibData HSD_PadLibData;
static const u32 pad_bit[4] = {PAD_CHAN0_BIT, PAD_CHAN1_BIT, PAD_CHAN2_BIT, PAD_CHAN3_BIT};
static inline f32 pad_sqrt(f32 value)
{
    if (value > 0.0F) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;
        estimate = 0.5 * estimate * -((value * (estimate * estimate)) - 3.0);
        estimate = 0.5 * estimate * -((value * (estimate * estimate)) - 3.0);
        estimate = 0.5 * estimate * -((value * (estimate * estimate)) - 3.0);
        result = value * estimate;
        return result;
    }
    return value;
}
static inline void HSD_PadRawQueueShift(u8 qnum, u8 *qptr)
{
    *qptr = (*qptr + 1) % qnum;
}

static inline void HSD_PadRawMerge(PADStatus *src1, PADStatus *src2, PADStatus *dst)
{
    int i;
    for (i = 0; i < 4; i++) {
        dst[i].button = src1[i].button | src2[i].button;
    }
}

void HSD_PadRenewRawStatus(void)
{
    int i;
    u32 mask;
    PadLibData *p = &HSD_PadLibData;
    HSD_PadData *qwrite;
    PADStatus *qread;
    HSD_PadData now;

    HSD_PadRumbleInterpret();
    now.motor_mask = PADRead(now.stat);
    qwrite = &p->queue[p->qwrite];
    if (p->qcount == p->qnum) {
        switch (p->qtype) {
        case 0:
            HSD_PadRawQueueShift(p->qnum, &p->qread);
            qread = p->queue[p->qread].stat;
            if (p->qnum != 1) {
                HSD_PadRawMerge(qwrite->stat, qread, qread);
            } else {
                HSD_PadRawMerge(now.stat, qread, now.stat);
            }
            break;
        case 1:
            HSD_PadRawQueueShift(p->qnum, &p->qread);
            break;
        case 2:
            goto skip;
        }
    } else {
        p->qcount += 1;
    }

    *qwrite = now;
    HSD_PadRawQueueShift(p->qnum, &p->qwrite);

skip:
    mask = 0;
    for (i = 0; i < 4; i++) {
        if (now.stat[i].err == PAD_ERR_NO_CONTROLLER) {
            mask |= pad_bit[i];
        }
    }
    if (mask != 0) {
        PADReset(mask);
    }
    if (OSGetResetSwitchState()) {
        p->reset_switch_status = 1;
    } else {
        if (p->reset_switch_status != 0) {
            p->reset_switch = 1;
            p->reset_switch_status = 0;
        }
    }
}

static void HSD_PadClampCheck1(u8 *val, u8 shift, u8 min, u8 max)
{
    if (*val < min) {
        *val = 0;
        return;
    }
    if (*val > max) {
        *val = max;
    }
    if (shift != 1) {
        return;
    }
    *val = *val - min;
}

static void HSD_PadClampCheck3(s8 *x, s8 *y, u8 shift, s8 min, s8 max)
{
    f32 r;

    r = pad_sqrt(((f32)*x * (f32)*x) + ((f32)*y * (f32)*y));

    if (r < min) {
        *y = 0;
        *x = 0;
        return;
    }
    if (r > max) {
        *x = ((f32)*x * (f32)max) / r;
        *y = ((f32)*y * (f32)max) / r;
        r = pad_sqrt(((f32)*x * (f32)*x) + ((f32)*y * (f32)*y));
    }

    if (shift == 1 && r > 1.000000013351432e-10f) {
        *x = (f32)*x - (((f32)*x * (f32)min) / r);
        *y = (f32)*y - (((f32)*y * (f32)min) / r);
    }
}

static inline void HSD_PadClamp(HSD_PadStatus *mp)
{
    PadLibData *p = &HSD_PadLibData;

    switch (p->clamp_stickType) {
    case 0:
        HSD_PadClampCheck3(&mp->stickX, &mp->stickY, p->clamp_stickShift,
                           p->clamp_stickMin, p->clamp_stickMax);
        HSD_PadClampCheck3(&mp->subStickX, &mp->subStickY, p->clamp_stickShift,
                           p->clamp_stickMin, p->clamp_stickMax);
        break;
    default:
        break;
    }
    HSD_PadClampCheck1(&mp->analogL, HSD_PadLibData.clamp_analogLRShift,
                       p->clamp_analogLRMin, p->clamp_analogLRMax);
    HSD_PadClampCheck1(&mp->analogR, p->clamp_analogLRShift, p->clamp_analogLRMin,
                       p->clamp_analogLRMax);
    HSD_PadClampCheck1(&mp->analogA, p->clamp_analogABShift, p->clamp_analogABMin,
                       p->clamp_analogABMax);
    HSD_PadClampCheck1(&mp->analogB, p->clamp_analogABShift, p->clamp_analogABMin,
                       p->clamp_analogABMax);
}

static void HSD_PadADConvertCheck1(HSD_PadStatus *mp, s8 x, s8 y, u32 up, u32 down,
                                   u32 left, u32 right)
{
    PadLibData *p = &HSD_PadLibData;
    f32 r;
    f32 a;
    f32 ha;

    r = pad_sqrt((f32)x * (f32)x + (f32)y * (f32)y);

    if ((f32)x == 0.0F) {
        int positive_y = (f32)y >= 0.0F;
        if (positive_y)
            a = 1.5707963267948966F;
        else
            a = -1.5707963267948966F;
    } else {
        a = (f32)atan2f(y, x);
    }

    ha = 0.5F * p->adc_angle;
    if (r < p->adc_th) {
        return;
    }

    if (a < -2.356194490192345 + ha) {
        mp->button |= left;
    }
    if (a >= -2.356194490192345 - ha && a <= -0.7853981633974483 + ha) {
        mp->button |= down;
    }
    if (a > -0.7853981633974483 - ha && a < 0.7853981633974483 + ha) {
        mp->button |= right;
    }
    if (a >= 0.7853981633974483 - ha && a <= 2.356194490192345 + ha) {
        mp->button |= up;
    }
    if (a > 2.356194490192345 - ha) {
        mp->button |= left;
    }
}

static inline void HSD_PadADConvert(HSD_PadStatus *mp)
{
    PadLibData *p = &HSD_PadLibData;

    switch (p->adc_type) {
    case 0:
        HSD_PadADConvertCheck1(mp, mp->stickX, mp->stickY, 0x10000, 0x20000, 0x40000,
                               0x80000);
        HSD_PadADConvertCheck1(mp, mp->subStickX, mp->subStickY, 0x100000, 0x200000,
                               0x400000, 0x800000);
        break;
    default:
        return;
    }
}

static inline void HSD_PadScale(HSD_PadStatus *mp)
{
    PadLibData *p = &HSD_PadLibData;

    mp->nml_stickX = (f32)mp->stickX / (f32)p->scale_stick;
    mp->nml_stickY = (f32)mp->stickY / (f32)p->scale_stick;
    mp->nml_subStickX = (f32)mp->subStickX / (f32)p->scale_stick;
    mp->nml_subStickY = (f32)mp->subStickY / (f32)p->scale_stick;
    mp->nml_analogL = (f32)mp->analogL / (f32)p->scale_analogLR;
    mp->nml_analogR = (f32)mp->analogR / (f32)p->scale_analogLR;
    mp->nml_analogA = (f32)mp->analogA / (f32)p->scale_analogAB;
    mp->nml_analogB = (f32)mp->analogB / (f32)p->scale_analogAB;
}

static void HSD_PadCrossDir(HSD_PadStatus *mp)
{
    switch (HSD_PadLibData.cross_dir) {
    case 0:
        break;

    case 1:
        if ((mp->button & (PAD_BUTTON_DOWN | PAD_BUTTON_UP)) == 0) {
            return;
        }
        mp->button = mp->button & ~(PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT);
        return;

    case 2:
        if ((mp->button & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT)) == 0) {
            return;
        }
        mp->button = mp->button & ~(PAD_BUTTON_DOWN | PAD_BUTTON_UP);
        return;

    case 3:
        if ((mp->button & (PAD_BUTTON_DOWN | PAD_BUTTON_UP)) != 0) {
            if ((mp->button & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT)) != 0) {
                if (mp->cross_dir == 1) {
                    mp->button = mp->button & ~(PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT);
                    return;
                }
                mp->button = mp->button & ~(PAD_BUTTON_DOWN | PAD_BUTTON_UP);
                return;
            } else {
                mp->cross_dir = 1;
                return;
            }
        }
        if ((mp->button & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT)) != 0) {
            mp->cross_dir = 2;
            return;
        }
    }
}

void HSD_PadRenewMasterStatus(void)
{
    int repeat_count;
    PadLibData *p;
    HSD_PadStatus *mp;
    PADStatus *qread;
    u32 motor_mask;
    int i;

    int intr;

    p = &HSD_PadLibData;
    mp = &HSD_PadMasterStatus[0];
    intr = OSDisableInterrupts();
    if (p->qcount != 0) {
        qread = p->queue[p->qread].stat;
        motor_mask = p->queue[p->qread].motor_mask;
        HSD_PadRawQueueShift(p->qnum, &p->qread);
        p->qcount -= 1;

        for (i = 0; i < 4; i++, mp += 1, qread += 1) {
            mp->last_button = mp->button;
            mp->err = qread->err;
            mp->support_rumble = (motor_mask & pad_bit[i]) ? 1 : 0;
            if (mp->err == PAD_ERR_NONE) {
                mp->button = qread->button;
                mp->stickX = qread->stickX;
                mp->stickY = qread->stickY;
                mp->subStickX = qread->substickX;
                mp->subStickY = qread->substickY;
                mp->analogL = qread->triggerLeft;
                mp->analogR = qread->triggerRight;
                mp->analogA = qread->analogA;
                mp->analogB = qread->analogB;
                HSD_PadClamp(mp);
                HSD_PadADConvert(mp);
                HSD_PadScale(mp);
                HSD_PadCrossDir(mp);
            } else if (mp->err == PAD_ERR_TRANSFER) {
                mp->err = PAD_ERR_NONE;
            } else {
                mp->button = 0;
                mp->subStickY = 0;
                mp->subStickX = 0;
                mp->stickY = 0;
                mp->stickX = 0;
                mp->analogB = 0;
                mp->analogA = 0;
                mp->analogR = 0;
                mp->analogL = 0;
                mp->nml_subStickY = 0.0;
                mp->nml_subStickX = 0.0;
                mp->nml_stickY = 0.0;
                mp->nml_stickX = 0.0;
                mp->nml_analogB = 0.0;
                mp->nml_analogA = 0.0;
                mp->nml_analogR = 0.0;
                mp->nml_analogL = 0.0;
            }
            mp->trigger = mp->button & (mp->last_button ^ mp->button);
            mp->release = mp->last_button & (mp->last_button ^ mp->button);
            if (mp->last_button ^ mp->button) {
                mp->repeat = mp->trigger;
                mp->repeat_count = p->repeat_start;
            } else {
                repeat_count = mp->repeat_count - 1;
                mp->repeat_count = repeat_count;
                if (repeat_count != 0) {
                    mp->repeat = 0;
                } else {
                    mp->repeat = mp->button;
                    mp->repeat_count = p->repeat_interval;
                }
            }
        }
    }
    OSRestoreInterrupts(intr);
}

static inline void HSD_PadCopyStatusFields(HSD_PadStatus *dst, HSD_PadStatus *src)
{
    dst->button = src->button;
    dst->stickX = src->stickX;
    dst->stickY = src->stickY;
    dst->subStickX = src->subStickX;
    dst->subStickY = src->subStickY;
    dst->analogL = src->analogL;
    dst->analogR = src->analogR;
    dst->analogA = src->analogA;
    dst->analogB = src->analogB;
    dst->nml_stickX = src->nml_stickX;
    dst->nml_stickY = src->nml_stickY;
    dst->nml_subStickX = src->nml_subStickX;
    dst->nml_subStickY = src->nml_subStickY;
    dst->nml_analogL = src->nml_analogL;
    dst->nml_analogR = src->nml_analogR;
    dst->nml_analogA = src->nml_analogA;
    dst->nml_analogB = src->nml_analogB;
}

static inline void HSD_PadClearStatusFields(HSD_PadStatus *dst)
{
    dst->button = 0;
    dst->subStickY = 0;
    dst->subStickX = 0;
    dst->stickY = 0;
    dst->stickX = 0;
    dst->analogB = 0;
    dst->analogA = 0;
    dst->analogR = 0;
    dst->analogL = 0;
    dst->nml_subStickY = 0.0;
    dst->nml_subStickX = 0.0;
    dst->nml_stickY = 0.0;
    dst->nml_stickX = 0.0;
    dst->nml_analogB = 0.0;
    dst->nml_analogA = 0.0;
    dst->nml_analogR = 0.0;
    dst->nml_analogL = 0.0;
}

void HSD_PadRenewCopyStatus(void)
{
    int repeat_count;
    HSD_PadStatus *mp;
    HSD_PadStatus *cp;
    PadLibData *p;

    int i;

    p = &HSD_PadLibData;
    for (i = 0; i < 4; i++) {
        mp = &HSD_PadMasterStatus[i];
        cp = &HSD_PadCopyStatus[i];

        cp->last_button = cp->button;
        cp->err = mp->err;
        cp->support_rumble = mp->support_rumble;
        if (cp->err == PAD_ERR_NONE) {
            HSD_PadCopyStatusFields(cp, mp);
        } else {
            HSD_PadClearStatusFields(cp);
        }
        cp->trigger = cp->button & (cp->last_button ^ cp->button);
        cp->release = cp->last_button & (cp->last_button ^ cp->button);
        if (cp->last_button ^ cp->button) {
            cp->repeat = cp->trigger;
            cp->repeat_count = p->repeat_start;
        } else {
            repeat_count = cp->repeat_count - 1;
            cp->repeat_count = repeat_count;
            if (repeat_count != 0) {
                cp->repeat = 0;
            } else {
                cp->repeat = cp->button;
                cp->repeat_count = p->repeat_interval;
            }
        }
    }
}

void HSD_PadRenewStatus(void)
{
    HSD_PadRenewRawStatus();
    HSD_PadRenewMasterStatus();
    HSD_PadRenewCopyStatus();
}

extern void HSD_PadRumbleInit(u16 count, void *records);
extern int PADInit(void);
void HSD_PadInit(u8 qnum, HSD_PadData *queue, u16 count, void *records)
{
    static HSD_PadStatus default_status_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    static PadLibData default_libinfo_data = {0,    0,    0, 0,    0, 0,    0x2D, 8,
                                              0,    0x1E, 0, 0,    0, 0x7F, 0,    0,
                                              0xFF, 0,    0, 0xFF, 0, 0x7F, 0xFF, 0xFF};
    int i;
    PadLibData *p = &HSD_PadLibData;
    *p = default_libinfo_data;
    p->qnum = qnum;
    p->queue = queue;
    HSD_PadRumbleInit(count, records);
    for (i = 0; i < 4; i++) {
        HSD_PadMasterStatus[i] = default_status_data;
        HSD_PadCopyStatus[i] = default_status_data;
    }
    PADInit();
}

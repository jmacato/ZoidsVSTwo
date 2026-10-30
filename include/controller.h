/* Layouts from
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/controller.h,
 * with the game's motor-mask and motor-support fields. */
#ifndef ZOIDS_CONTROLLER_H
#define ZOIDS_CONTROLLER_H
#include "types.h"
typedef struct PADStatus {
    u16 button;
    s8 stickX, stickY, substickX, substickY;
    u8 triggerLeft, triggerRight, analogA, analogB;
    s8 err;
} PADStatus;
#include "rumble.h"
typedef struct HSD_PadData {
    PADStatus stat[4];
    u32 motor_mask;
} HSD_PadData;

typedef struct HSD_PadStatus {
    u32 button;
    u32 last_button;
    u32 trigger;
    u32 repeat;
    u32 release;
    s32 repeat_count;
    s8 stickX;
    s8 stickY;
    s8 subStickX;
    s8 subStickY;
    u8 analogL;
    u8 analogR;
    u8 analogA;
    u8 analogB;
    f32 nml_stickX;
    f32 nml_stickY;
    f32 nml_subStickX;
    f32 nml_subStickY;
    f32 nml_analogL;
    f32 nml_analogR;
    f32 nml_analogA;
    f32 nml_analogB;
    u8 cross_dir;
    u8 support_rumble;
    s8 err;
} HSD_PadStatus;

typedef struct PadLibData {
    u8 qnum;
    u8 qread;
    u8 qwrite;
    u8 qcount;
    u8 qtype;
    HSD_PadData *queue;
    s32 repeat_start;
    s32 repeat_interval;
    s8 adc_type;
    s8 adc_th;
    f32 adc_angle;
    u8 clamp_stickType;
    u8 clamp_stickShift;
    s8 clamp_stickMax;
    s8 clamp_stickMin;
    u8 clamp_analogLRShift;
    u8 clamp_analogLRMax;
    u8 clamp_analogLRMin;
    u8 clamp_analogABShift;
    u8 clamp_analogABMax;
    u8 clamp_analogABMin;
    s8 scale_stick;
    u8 scale_analogLR;
    u8 scale_analogAB;
    u8 cross_dir;
    u8 reset_switch_status;
    u8 reset_switch;
    struct RumbleInfo rumble_info;
} PadLibData;

extern PadLibData HSD_PadLibData;
extern HSD_PadStatus HSD_PadMasterStatus[4];
extern HSD_PadStatus HSD_PadCopyStatus[4];
extern void HSD_PadRenewRawStatus(void);
extern void HSD_PadRenewMasterStatus(void);
extern void HSD_PadRenewCopyStatus(void);
extern void HSD_PadRenewStatus(void);
extern void HSD_PadInit(u8 queue_count, HSD_PadData *queue, u16 rumble_count,
                        void *rumble_records);
#endif

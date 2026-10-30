#ifndef ZOIDS_FOBJ_H
#define ZOIDS_FOBJ_H

#include "aobj.h"

#define HSD_A_OP_NONE 0
#define HSD_A_OP_CON 1
#define HSD_A_OP_LIN 2
#define HSD_A_OP_SPL0 3
#define HSD_A_OP_SPL 4
#define HSD_A_OP_SLP 5
#define HSD_A_OP_KEY 6

#define HSD_A_FRAC_FLOAT (0 << 5)
#define HSD_A_FRAC_S16 (1 << 5)
#define HSD_A_FRAC_U16 (2 << 5)
#define HSD_A_FRAC_S8 (3 << 5)
#define HSD_A_FRAC_U8 (4 << 5)

#define FOBJ_LOAD_DATA0 1
#define FOBJ_LOAD_DATA 2
#define FOBJ_LOAD_WAIT 3

/* Field names follow https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/fobj.h. */
typedef struct HSD_FObj {
    struct HSD_FObj *next;
    u8 *data;
    u8 *data_head;
    u32 length;
    u8 flags;
    u8 operation;
    u8 interpolation_operation;
    u8 object_type;
    u8 value_format;
    u8 slope_format;
    u16 packed_count;
    s16 start_frame;
    u16 frame_term;
    f32 time;
    f32 value_0;
    f32 value_1;
    f32 slope_0;
    f32 slope_1;
} HSD_FObj;

typedef struct HSD_FObjDesc {
    struct HSD_FObjDesc *next;
    u32 length;
    f32 start_frame;
    u8 object_type;
    u8 value_format;
    u8 slope_format;
    u8 reserved_0F;
    u8 *data;
} HSD_FObjDesc;

extern u32 HSD_FObjSetState(HSD_FObj *function, u32 state);
extern void HSD_FObjInterpretAnim(HSD_FObj *function, void *object,
                                  AnimationUpdate update, f32 rate);

static inline u32 set_frame_state(HSD_FObj *function, u32 state)
{
    if (function != 0) {
        function->flags = (function->flags & 0xF0) | (state & 0xF);
    }
    return state;
}

static inline void request_frame_animation(HSD_FObj *function, f32 frame)
{
    if (function != 0) {
        function->data = function->data_head;
        function->time = (f32)function->start_frame + frame;
        function->operation = 0;
        function->interpolation_operation = 0;
        function->flags &= ~0x40;
        function->packed_count = 0;
        function->frame_term = 0;
        function->value_0 = 0.0f;
        function->value_1 = 0.0f;
        function->slope_0 = 0.0f;
        function->slope_1 = 0.0f;
        set_frame_state(function, FOBJ_LOAD_DATA0);
    }
}

#endif

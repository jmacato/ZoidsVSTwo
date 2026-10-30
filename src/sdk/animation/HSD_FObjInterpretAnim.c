/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/fobj.c. */
#include "fobj.h"

#pragma inline_depth(5)

extern f32 parseFloat(u8 **position, u8 format);
extern u32 parsePackInfo(u8 **position);
extern void FObjUpdateAnim(HSD_FObj *function, void *object, AnimationUpdate update);
extern void __assert(const char *file, u32 line, const char *condition);

#define TRACK_ASSERT(line, condition)                                                  \
    ((condition) ? (void)0 : __assert("fobj.c", line, #condition))

extern u32 HSD_FObjGetState(HSD_FObj *function);

static inline u32 track_state(HSD_FObj *function, u32 state)
{
    if (function != 0) {
        function->flags = (state & 0xF) | (function->flags & 0xF0);
    }
    return state;
}

static inline u32 frame_state(HSD_FObj *function)
{
    if (function == 0) {
        return 0;
    }
    return function->flags & 0xF;
}

static inline u8 parseOpCode(u8 **curr_parse)
{
    return (**curr_parse) & 0xF;
}

extern void FObjLaunchKeyData(HSD_FObj *function);

static inline void launch_key(HSD_FObj *function)
{
    if ((function->flags & 0x40) != 0) {
        function->interpolation_operation = function->operation;
        function->flags &= ~0x40;
        function->flags |= 0x80;
        function->value_0 = function->value_1;
    }
}

static inline s32 parseWait(u8 **adp)
{
    u8 d;
    s32 wait = 0;
    s32 shift = 0;

    do {
        d = *(*adp)++;
        wait |= (d & 0x7f) << shift;
        shift += 7;
    } while (d & 0x80);

    return wait;
}

static inline u32 FObjLoadWait(HSD_FObj *fobj, const char *condition)
{
    u32 st = frame_state(fobj);
    ((st == FOBJ_LOAD_WAIT) ? (void)0 : __assert("fobj.c", 0x16C, condition));

    if ((unsigned)(fobj->data - fobj->data_head) >= fobj->length) {
        return 6;
    } else {
        fobj->frame_term = parseWait(&fobj->data);
        fobj->flags |= 0x20;
        return track_state(fobj, FOBJ_LOAD_DATA);
    }
}

static inline u32 FObjAnimCON(HSD_FObj *fobj)
{
    u32 st = frame_state(fobj);
    TRACK_ASSERT(0x17F, st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->value_0 = fobj->value_1;
    fobj->value_1 = parseFloat(&fobj->data, fobj->value_format);
    if (fobj->interpolation_operation != HSD_A_OP_SLP) {
        fobj->slope_0 = fobj->slope_1;
        fobj->slope_1 = 0.0F;
    }

    return track_state(fobj, st == FOBJ_LOAD_DATA0 ? FOBJ_LOAD_WAIT : 4);
}

static inline u32 FObjAnimLinear(HSD_FObj *fobj)
{
    u32 st = frame_state(fobj);
    TRACK_ASSERT(0x193, st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->value_0 = fobj->value_1;
    fobj->value_1 = parseFloat(&fobj->data, fobj->value_format);
    if (fobj->interpolation_operation != HSD_A_OP_SLP) {
        fobj->slope_0 = fobj->slope_1;
        fobj->slope_1 = 0.0F;
    }

    return track_state(fobj, st == FOBJ_LOAD_DATA0 ? FOBJ_LOAD_WAIT : 4);
}

static inline u32 FObjAnimSPL0(HSD_FObj *fobj)
{
    u32 st = frame_state(fobj);
    TRACK_ASSERT(0x1A7, st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->value_0 = fobj->value_1;
    fobj->slope_0 = fobj->slope_1;
    fobj->value_1 = parseFloat(&fobj->data, fobj->value_format);
    fobj->slope_1 = 0.0F;

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? FOBJ_LOAD_WAIT : 4);
}

static inline u32 FObjAnimSPL(HSD_FObj *fobj)
{
    u32 st = frame_state(fobj);
    TRACK_ASSERT(0x1B9, st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->value_0 = fobj->value_1;
    fobj->value_1 = parseFloat(&fobj->data, fobj->value_format);
    fobj->slope_0 = fobj->slope_1;
    fobj->slope_1 = parseFloat(&fobj->data, fobj->slope_format);

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? FOBJ_LOAD_WAIT : 4);
}

static inline u32 FObjAnimSLP(HSD_FObj *fobj)
{
    u32 st = frame_state(fobj);
    TRACK_ASSERT(0x1CC, st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->slope_0 = fobj->slope_1;
    fobj->slope_1 = parseFloat(&fobj->data, fobj->slope_format);

    return HSD_FObjGetState(fobj);
}

static inline u32 FObjAnimKey(HSD_FObj *fobj)
{
    u32 st = frame_state(fobj);
    TRACK_ASSERT(0x1E9, st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    FObjLaunchKeyData(fobj);
    fobj->value_1 = parseFloat(&fobj->data, fobj->value_format);
    fobj->flags |= 0x40;

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? FOBJ_LOAD_WAIT : 4);
}

static inline u32 FObjLoadData(HSD_FObj *fobj)
{
    if ((unsigned)(fobj->data - fobj->data_head) >= fobj->length) {
        return 6;
    } else {
        fobj->interpolation_operation = fobj->operation;
        if (fobj->packed_count == 0) {
            fobj->operation = parseOpCode(&fobj->data);
            fobj->packed_count = parsePackInfo(&fobj->data);
        }

        fobj->packed_count -= 1;

        switch (fobj->operation) {
        case HSD_A_OP_CON:
            return FObjAnimCON(fobj);

        case HSD_A_OP_LIN:
            return FObjAnimLinear(fobj);

        case HSD_A_OP_SPL0:
            return FObjAnimSPL0(fobj);

        case HSD_A_OP_SPL:
            return FObjAnimSPL(fobj);

        case HSD_A_OP_SLP:
            return FObjAnimSLP(fobj);

        case HSD_A_OP_KEY:
            return FObjAnimKey(fobj);

        default:
            return 0;
        }
    }
}

void HSD_FObjInterpretAnim(HSD_FObj *fobj, void *obj, AnimationUpdate obj_update,
                           f32 rate)
{
    const char *wait_condition = "st == FOBJ_LOAD_WAIT";
    f32 fterm;
    u32 state;
    u32 initial_state;

    fterm = 0.0F;
    initial_state = fobj != 0 ? HSD_FObjGetState(fobj) : 0;
    state = initial_state;
    if (initial_state != 0 && !(fobj->time += rate, (fobj->time < 0.0))) {
        for (;;) {
            switch (state) {
            case 6: {
                fobj->time += fterm;
                launch_key(fobj);
                FObjUpdateAnim(fobj, obj, obj_update);
                return;
            }
            case FOBJ_LOAD_DATA0:
            case FOBJ_LOAD_DATA: {
                state = FObjLoadData(fobj);
                break;
            }
            case FOBJ_LOAD_WAIT: {
                if ((fobj->flags & 0x80) != 0) {
                    FObjUpdateAnim(fobj, obj, obj_update);
                }
                state = FObjLoadWait(fobj, wait_condition);
                break;
            }
            case 4: {
                if (fobj->frame_term <= fobj->time) {
                    state = state = FOBJ_LOAD_WAIT;

                    fterm = fobj->frame_term;
                    fobj->time -= fobj->frame_term;
                    track_state(fobj, state);
                    break;
                }
                FObjUpdateAnim(fobj, obj, obj_update);
                state = state = 5;
                track_state(fobj, state);
                return;
            }
            case 5: {
                state = state = 4;
                track_state(fobj, state);
                break;
            }
            case 0:
                return;
            }
        }
    }
}

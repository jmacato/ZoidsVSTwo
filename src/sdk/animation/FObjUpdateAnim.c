#include "fobj.h"

extern f32 splGetHelmite(f32 inverse_length, f32 time, f32 value_0,
                        f32 value_1, f32 slope_0, f32 slope_1);

void FObjUpdateAnim(HSD_FObj *function, void *object, AnimationUpdate update)
{
    AnimationValue value;

    if (update == 0) {
        return;
    }
    switch (function->interpolation_operation) {
    case HSD_A_OP_KEY:
        if ((function->flags & 0x80) != 0) {
            value.scalar = function->value_0;
            function->flags &= ~0x80;
        } else {
            return;
        }
        break;
    case HSD_A_OP_CON:
        value.scalar = function->time >= (f32)function->frame_term
                       ? function->value_1 : function->value_0;
        break;
    case HSD_A_OP_LIN:
        if ((function->flags & 0x20) != 0) {
            function->flags &= ~0x20;
            if (function->frame_term != 0) {
                function->slope_0 = (function->value_1 - function->value_0)
                                    / (f32)function->frame_term;
            } else {
                function->slope_0 = 0.0f;
                function->value_0 = function->value_1;
            }
        }
        value.scalar = function->slope_0 * function->time + function->value_0;
        break;
    case HSD_A_OP_SPL0:
    case HSD_A_OP_SPL:
    case HSD_A_OP_SLP:
        if (function->frame_term != 0) {
            value.scalar = splGetHelmite(1.0 / function->frame_term, function->time,
                                        function->value_0, function->value_1,
                                        function->slope_0, function->slope_1);
        } else {
            value.scalar = function->value_1;
        }
        break;
    default:
        break;
    }
    update(object, function->object_type, &value);
}

#include "fobj.h"

void HSD_FObjStopAnimAll(HSD_FObj *function, void *object,
                         AnimationUpdate update, f32 rate)
{
    for (; function != 0; function = function->next) {
        if (function != 0) {
            if (function->interpolation_operation == 6) {
                HSD_FObjInterpretAnim(function, object, update, rate);
            }
            set_frame_state(function, 0);
        }
    }
}

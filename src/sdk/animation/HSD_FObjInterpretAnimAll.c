#include "fobj.h"

void HSD_FObjInterpretAnimAll(void *functions, void *object,
                              AnimationUpdate update, f32 rate)
{
    HSD_FObj *function;

    for (function = functions; function != 0; function = function->next) {
        HSD_FObjInterpretAnim(function, object, update, rate);
    }
}

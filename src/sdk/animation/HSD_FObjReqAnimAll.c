#include "fobj.h"

void HSD_FObjReqAnimAll(HSD_FObj *function, f32 frame)
{
    if (function != 0) {
        for (; function != 0; function = function->next) {
            request_frame_animation(function, frame);
        }
    }
}

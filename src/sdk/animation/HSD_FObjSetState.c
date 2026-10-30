#include "fobj.h"

u32 HSD_FObjSetState(HSD_FObj *function, u32 state)
{
    return set_frame_state(function, state);
}

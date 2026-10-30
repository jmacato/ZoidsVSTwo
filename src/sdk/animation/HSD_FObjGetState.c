#include "fobj.h"

u32 HSD_FObjGetState(HSD_FObj *function)
{
    if (function == 0) {
        return 0;
    }
    return function->flags & 0xF;
}

#include "fobj.h"

extern void HSD_FObjFree(HSD_FObj *function);

inline void HSD_FObjRemove(HSD_FObj *function)
{
    if (function != 0) {
        HSD_FObjFree(function);
    }
}



void HSD_FObjRemoveAll(HSD_FObj *function)
{
    if (function != 0) {
        HSD_FObjRemoveAll(function->next);
        HSD_FObjRemove(function);
    }
}

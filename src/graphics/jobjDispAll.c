#include "jobj.h"

#pragma dont_inline on
void jobjDispAll(HSD_JObj *joint)
{
    if (joint != (void *)0)
        HSD_JObjDispAll(joint, 0, HSD_TRSP_ALL, 0);
}

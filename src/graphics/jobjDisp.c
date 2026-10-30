#include "jobj.h"

#pragma dont_inline on
void jobjDisp(HSD_JObj *joint)
{
    if (joint != (void *)0)
        HSD_JObjDisp(joint, 0, HSD_TRSP_ALL, 0);
}

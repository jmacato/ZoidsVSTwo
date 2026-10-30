#include "jobj.h"

extern void psKillJObjAllGenerator(HSD_JObj *joint);

#pragma dont_inline on
void jobjUnrefAll(HSD_JObj *joint)
{
    if (joint != (void *)0) {
        HSD_JObjRemoveAnimAll(joint);
        psKillJObjAllGenerator(joint);
        HSD_JObjUnref(joint);
    }
}

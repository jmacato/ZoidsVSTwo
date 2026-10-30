#include "aobj.h"

extern s32 nb_deadanim;
extern s32 nb_activeanim;

void HSD_AObjInitEndCallBack(void)
{
    nb_deadanim = 0;
    nb_activeanim = 0;
}

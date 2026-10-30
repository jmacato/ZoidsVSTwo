#include "types.h"

extern void ssLoadStageNow(s32 stage, s32 mode);
extern s32 usbForce;

void LoadStageNow(s32 stage)
{
    ssLoadStageNow(stage, usbForce);
}

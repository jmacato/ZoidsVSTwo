#include "types.h"

extern void LoadStageData2__(s32 stage, s32 usb);

void LoadStageData2(s32 stage)
{
    LoadStageData2__(stage, 0);
}

#include "types.h"

extern void LoadStageData2__(s32 stage, s32 usb);

void LoadStageDataUSB2(s32 stage)
{
    LoadStageData2__(stage, 1);
}

#include "types.h"

extern s16 menudispyesnoanm[8];
extern s32 menudispyesnopoint;
extern s32 menudispyesnoflag;
extern s32 MemcardNowSlot;

s32 TEA_mcResult(s32 result)
{
    s32 slot = MemcardNowSlot;
    char buf[0x190];
    s16 pair[2];
    s32 point = menudispyesnopoint;
    s32 flag = menudispyesnoflag;
    s32 zero = 0;
    pair[0] = menudispyesnoanm[0];
    pair[1] = menudispyesnoanm[1];
    return 1;
}

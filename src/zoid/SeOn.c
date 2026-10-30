#include "types.h"

extern s32 SE_TableList[];
extern s32 TEA_SeOn(u16 se, f32 a, f32 b);

s32 SeOn(s32 index)
{
    s32 se = SE_TableList[index];
    if (se < 0)
        return -1;
    return TEA_SeOn(se, 0.0f, 0.0f);
}

#include "types.h"

extern s32 TEA_after_vsync;
extern s32 vsyncN;

void TEA_setvsync(void)
{
    TEA_after_vsync = vsyncN;
}

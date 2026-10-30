#include "types.h"

extern u32 PPCMfhid0(void);
extern void PPCMthid0(u32 value);
#define HID0_SPD 0x00000200u

void PPCDisableSpeculation(void)
{
    PPCMthid0(PPCMfhid0() | HID0_SPD);
}

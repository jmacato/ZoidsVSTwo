#include "types.h"
#include "metrotrk.h"

__declspec(section ".init") void __TRK_reset(void)
{
    __TRK_copy_vectors();
}

void EnableMetroTRKInterrupts(void)
{
    EnableEXI2Interrupts();
}

u32 TRKTargetTranslate(u32 address)
{
    return (address & 0x3FFFFFFF) | 0x80000000;
}

u8 TRKTargetCPUMinorType(void)
{
    return 0x54;
}

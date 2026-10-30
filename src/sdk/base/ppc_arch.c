#include "types.h"
#include "asm_ops.h"

u32 PPCMfmsr(void)
{
    register u32 value;

    ASM_MFMSR(value);
    return value;
}

void PPCMtmsr(register u32 value)
{
    ASM_MTMSR(value);
}

u32 PPCMfhid0(void)
{
    register u32 value;

    ASM_MFSPR(value, 1008);
    return value;
}

void PPCMthid0(register u32 value)
{
    ASM_MTSPR(1008, value);
}

u32 PPCMfl2cr(void)
{
    register u32 value;

    ASM_MFSPR(value, 1017);
    return value;
}

void PPCMtl2cr(register u32 value)
{
    ASM_MTSPR(1017, value);
}

void PPCMtdec(register u32 value)
{
    ASM_MTSPR(22, value);
}

void PPCSync(void)
{
    ASM_SC();
}

u32 PPCMfhid2(void)
{
    register u32 value;

    ASM_MFSPR(value, 920);
    return value;
}

void PPCMthid2(register u32 value)
{
    ASM_MTSPR(920, value);
}

void PPCMtwpar(register u32 value)
{
    ASM_MTSPR(921, value);
}

void PPCSetFpNonIEEEMode(void)
{
    ASM_MTFSB1(29);
}

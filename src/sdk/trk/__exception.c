#include "types.h"

void __TRK_reset(void);
void TRKInterruptHandler(void);
void __TRK_vector_F54(void);

#define PAD(start, end) \
    __declspec(section ".init") const u8 __TRK_pad_##start[end - start] = { 0 }
#define SAVE_AND_JUMP(vector) \
    mtsprg1 r2; mtsprg2 r3; mtsprg3 r4; mfsrr0 r2; mfsrr1 r4; mfmsr r3; \
    ori r3, r3, 0x30; mtsrr1 r3; lis r3, TRKInterruptHandler@h; \
    ori r3, r3, TRKInterruptHandler@l; mtsrr0 r3; li r3, vector; rfi

__declspec(section ".init") const u8 gTRKInterruptVectorTable[0x100] =
    "Metrowerks Target Resident Kernel for PowerPC";

__declspec(section ".init") asm void __TRK_vector_100(void)
{
    nofralloc
    b __TRK_reset
}

PAD(0x104, 0x200);

__declspec(section ".init") asm void __TRK_vector_200(void)
{
    nofralloc
    mtsprg1 r2
    mfsrr0 r2
    icbi r0, r2
    mfdar r2
    dcbi r0, r2
    mfsprg1 r2
    SAVE_AND_JUMP(0x200)
}

PAD(0x24C, 0x300);

__declspec(section ".init") asm void __TRK_vector_300(void)
{
    nofralloc
    SAVE_AND_JUMP(0x300)
}

PAD(0x334, 0x400);

__declspec(section ".init") asm void __TRK_vector_400(void)
{
    nofralloc
    SAVE_AND_JUMP(0x400)
}

PAD(0x434, 0x500);

__declspec(section ".init") asm void __TRK_vector_500(void)
{
    nofralloc
    SAVE_AND_JUMP(0x500)
}

PAD(0x534, 0x600);

__declspec(section ".init") asm void __TRK_vector_600(void)
{
    nofralloc
    SAVE_AND_JUMP(0x600)
}

PAD(0x634, 0x700);

__declspec(section ".init") asm void __TRK_vector_700(void)
{
    nofralloc
    SAVE_AND_JUMP(0x700)
}

PAD(0x734, 0x800);

__declspec(section ".init") asm void __TRK_vector_800(void)
{
    nofralloc
    SAVE_AND_JUMP(0x800)
}

PAD(0x834, 0x900);

__declspec(section ".init") asm void __TRK_vector_900(void)
{
    nofralloc
    SAVE_AND_JUMP(0x900)
}

PAD(0x934, 0xC00);

__declspec(section ".init") asm void __TRK_vector_C00(void)
{
    nofralloc
    SAVE_AND_JUMP(0xC00)
}

PAD(0xC34, 0xD00);

__declspec(section ".init") asm void __TRK_vector_D00(void)
{
    nofralloc
    SAVE_AND_JUMP(0xD00)
}

PAD(0xD34, 0xE00);

__declspec(section ".init") asm void __TRK_vector_E00(void)
{
    nofralloc
    SAVE_AND_JUMP(0xE00)
}

PAD(0xE34, 0xF00);

__declspec(section ".init") asm void __TRK_vector_F00(void)
{
    nofralloc
    b __TRK_vector_F54
}

PAD(0xF04, 0xF20);

__declspec(section ".init") asm void __TRK_vector_F20(void)
{
    nofralloc
    SAVE_AND_JUMP(0xF20)
}

__declspec(section ".init") asm void __TRK_vector_F54(void)
{
    nofralloc
    SAVE_AND_JUMP(0xF00)
}

PAD(0xF88, 0x1000);

__declspec(section ".init") asm void __TRK_vector_1000(void)
{
    nofralloc
    mtsprg1 r2
    mfcr r2
    mtsprg2 r2
    mfmsr r2
    andis. r2, r2, 0x2
    beq restore
    mfmsr r2
    xoris r2, r2, 0x2
    sync
    mtmsr r2
    sync
    mtsprg1 r2
restore:
    mfsprg2 r2
    mtcrf 0xff, r2
    mfsprg1 r2
    SAVE_AND_JUMP(0x1000)
}

PAD(0x1070, 0x1100);

__declspec(section ".init") asm void __TRK_vector_1100(void)
{
    nofralloc
    mtsprg1 r2
    mfcr r2
    mtsprg2 r2
    mfmsr r2
    andis. r2, r2, 0x2
    beq restore
    mfmsr r2
    xoris r2, r2, 0x2
    sync
    mtmsr r2
    sync
    mtsprg1 r2
restore:
    mfsprg2 r2
    mtcrf 0xff, r2
    mfsprg1 r2
    SAVE_AND_JUMP(0x1100)
}

PAD(0x1170, 0x1200);

__declspec(section ".init") asm void __TRK_vector_1200(void)
{
    nofralloc
    mtsprg1 r2
    mfcr r2
    mtsprg2 r2
    mfmsr r2
    andis. r2, r2, 0x2
    beq restore
    mfmsr r2
    xoris r2, r2, 0x2
    sync
    mtmsr r2
    sync
    mtsprg1 r2
restore:
    mfsprg2 r2
    mtcrf 0xff, r2
    mfsprg1 r2
    SAVE_AND_JUMP(0x1200)
}

PAD(0x1270, 0x1300);

__declspec(section ".init") asm void __TRK_vector_1300(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1300)
}

PAD(0x1334, 0x1400);

__declspec(section ".init") asm void __TRK_vector_1400(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1400)
}

PAD(0x1434, 0x1600);

__declspec(section ".init") asm void __TRK_vector_1600(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1600)
}

PAD(0x1634, 0x1700);

__declspec(section ".init") asm void __TRK_vector_1700(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1700)
}

PAD(0x1734, 0x1C00);

__declspec(section ".init") asm void __TRK_vector_1C00(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1C00)
}

PAD(0x1C34, 0x1D00);

__declspec(section ".init") asm void __TRK_vector_1D00(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1D00)
}

PAD(0x1D34, 0x1E00);

__declspec(section ".init") asm void __TRK_vector_1E00(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1E00)
}

PAD(0x1E34, 0x1F00);

__declspec(section ".init") asm void __TRK_vector_1F00(void)
{
    nofralloc
    SAVE_AND_JUMP(0x1F00)
}

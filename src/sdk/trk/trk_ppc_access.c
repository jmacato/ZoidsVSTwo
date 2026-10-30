#include "types.h"
#include "metrotrk.h"

#pragma peephole off

typedef struct u128 {
    u64 hi;
    u64 lo;
} u128;

typedef void (*RegAccessFunc)(void *srcDestPtr, u128 *val);

extern u128 TRKvalue128_temp;

s32 TRKPPCAccessSPR(void *srcDestPtr, u32 spr, s32 read)
{
    u32 instructionData[] = { 0x60000000, 0x60000000, 0x60000000, 0x60000000, 0x60000000 };

    if (read) {
        instructionData[0] = 0x7C000000 | (4 << 21) | ((spr & 0xFE0) << 6) | ((spr & 0x1F) << 16) | 0x2A6;
        instructionData[1] = 0x90000000 | (4 << 21) | (3 << 16) | 0;
    } else {
        instructionData[0] = 0x80000000 | (4 << 21) | (3 << 16) | 0;
        instructionData[1] = 0x7C000000 | (4 << 21) | ((spr & 0xFE0) << 6) | ((spr & 0x1F) << 16) | 0x3A6;
    }

    return TRKPPCAccessSpecialReg(srcDestPtr, instructionData, read);
}

s32 TRKPPCAccessPairedSingleRegister(void *srcDestPtr, u32 psr, s32 read)
{
    u32 instructionData[] = { 0x60000000, 0x60000000, 0x60000000, 0x60000000, 0x60000000 };

    if (read) {
        instructionData[0] = 0xF0000000 | (psr << 21) | (3 << 16) | (0 << 15) | (0 << 12) | 0;
    } else {
        instructionData[0] = 0xE0000000 | (psr << 21) | (3 << 16) | (0 << 15) | (0 << 12) | 0;
    }

    return TRKPPCAccessSpecialReg(srcDestPtr, instructionData, read);
}

s32 TRKPPCAccessFPRegister(void *srcDestPtr, u32 fpr, s32 read)
{
    s32 error = 0;
    u32 instructionData1[] = { 0x60000000, 0x60000000, 0x60000000, 0x60000000, 0x60000000 };

    if (fpr < 32) {
        if (read) {
            instructionData1[0] = 0xD8000000 | (fpr << 21) | (3 << 16) | 0;
        } else {
            instructionData1[0] = 0xC8000000 | (fpr << 21) | (3 << 16) | 0;
        }

        error = TRKPPCAccessSpecialReg(srcDestPtr, instructionData1, read);
    } else if (fpr == TRK_FLOAT_FPSCR) {
        if (read) {
            instructionData1[0] = 0xD8240000;
            instructionData1[1] = 0xFC20048E;
            instructionData1[2] = 0xD8230000;
            instructionData1[3] = 0xC8240000;
        } else {
            instructionData1[0] = 0xD8240000;
            instructionData1[1] = 0xD8230000;
            instructionData1[2] = 0xFDFE0D8E;
            instructionData1[3] = 0xC8240000;
        }

        error = TRKPPCAccessSpecialReg(srcDestPtr, instructionData1, read);
        *(u64 *)srcDestPtr &= 0xFFFFFFFF;
    } else if (fpr == TRK_FLOAT_FPECR) {
        if (!read) {
            *(u32 *)srcDestPtr = *(u32 *)((u32)srcDestPtr + 4);
        }

        error = TRKPPCAccessSPR(srcDestPtr, SPR_FPECR, read);

        if (read) {
            *(u64 *)srcDestPtr = *(u32 *)srcDestPtr & 0xFFFFFFFFLL;
        }
    }

    return error;
}

s32 TRKPPCAccessSpecialReg(void *srcDestPtr, u32 *instructionData, s32 read)
{
    instructionData[4] = 0x4E800020;
    TRK_flush_cache((u32)instructionData, 0x14);
    ((RegAccessFunc)instructionData)(srcDestPtr, &TRKvalue128_temp);
    return 0;
}

/* Reference: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/OSCache.c.
 */
#include "os_cache.h"
#include "os_error.h"
#include "db.h"
#include "asm_ops.h"
#include "os_interrupt.h"
extern u32 ICEnable(void);
extern u32 DCEnable(void);
extern void LCStoreBlocks(void *, void *, u32);
extern u32 PPCMfhid0(void);
extern u32 PPCMfhid2(void);
extern u32 PPCMfl2cr(void);
extern void PPCMthid2(u32);
extern void PPCMtl2cr(u32);
extern u32 PPCMfmsr(void);
extern void PPCMtmsr(u32);
extern void PPCHalt(void);
#define MSR_IR 0x00000020
#define MSR_DR 0x00000010
#define HID0_ICE 0x00008000u
#define HID0_DCE 0x00004000u
#define HID2_DCHERR 0x00800000
#define HID2_DNCERR 0x00400000
#define HID2_DCMERR 0x00200000
#define HID2_DQOERR 0x00100000
#define SRR1_DMA_BIT 0x00200000
#define L2CR_L2E 0x80000000
#define L2CR_L2I 0x00200000
static inline void L2Disable(void)
{
    __sync();
    PPCMtl2cr(PPCMfl2cr() & ~0x80000000);
    __sync();
}
static inline void L2Enable(void)
{
    PPCMtl2cr((PPCMfl2cr() | L2CR_L2E) & ~L2CR_L2I);
}
static inline void L2Init(void)
{
    u32 oldMSR;
    oldMSR = PPCMfmsr();
    __sync();
    PPCMtmsr(MSR_IR | MSR_DR);
    __sync();
    L2Disable();
    L2GlobalInvalidate();
    PPCMtmsr(oldMSR);
}
void DCInvalidateRange(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbi r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
}

void DCFlushRange(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbf r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
    ASM_INSN(sc);
}

void DCStoreRange(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbst r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
    ASM_INSN(sc);
}

void DCFlushRangeNoSync(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbf r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
}

void DCStoreRangeNoSync(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbst r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
}

void DCZeroRange(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbz r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
}

void ICInvalidateRange(void *addr, u32 nBytes)
{
    ASM_INSN(cmplwi r4, 0);
    ASM_INSN(blelr);
    ASM_INSN(clrlwi. r5, r3, 27);
    ASM_INSN(beq skip);
    ASM_INSN(addi r4, r4, 32);
    ASM_INSN(skip: addi r4, r4, 31);
    ASM_INSN(srwi r4, r4, 5);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: icbi r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
    ASM_INSN(sync);
    ASM_INSN(isync);
}

static void __LCEnable(void)
{
    ASM_INSN(mfmsr r5);
    ASM_INSN(ori r5, r5, 0x1000);
    ASM_INSN(mtmsr r5);
    ASM_INSN(lis r3, 0x8000);
    ASM_INSN(li r4, 1024);
    ASM_INSN(mtctr r4);
    ASM_INSN(touch: dcbt r0, r3);
    ASM_INSN(dcbst r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz touch);
    ASM_INSN(mfspr r4, 920);
    ASM_INSN(oris r4, r4, 0x100F);
    ASM_INSN(mtspr 920, r4);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(lis r3, 0xE000);
    ASM_INSN(ori r3, r3, 0x0002);
    ASM_INSN(mtdbatl 3, r3);
    ASM_INSN(ori r3, r3, 0x01FE);
    ASM_INSN(mtdbatu 3, r3);
    ASM_INSN(isync);
    ASM_INSN(lis r3, 0xE000);
    ASM_INSN(li r6, 512);
    ASM_INSN(mtctr r6);
    ASM_INSN(li r6, 0);
    ASM_INSN(lock: dcbz_l r6, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz lock);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
    ASM_INSN(nop);
}

void LCEnable(void)
{
    s32 enabled;

    enabled = OSDisableInterrupts();
    __LCEnable();
    OSRestoreInterrupts(enabled);
}
void LCDisable(void)
{
    ASM_INSN(lis r3, 0xE000);
    ASM_INSN(li r4, 512);
    ASM_INSN(mtctr r4);
    ASM_INSN(loop: dcbi r0, r3);
    ASM_INSN(addi r3, r3, 32);
    ASM_INSN(bdnz loop);
    ASM_INSN(mfspr r4, 920);
    ASM_INSN(rlwinm r4, r4, 0, 4, 2);
    ASM_INSN(mtspr 920, r4);
}

#pragma peephole off
u32 LCStoreData(void *destAddr, void *srcAddr, u32 nBytes)
{
    u32 numBlocks = (nBytes + 31) / 32;
    u32 numTransactions = (numBlocks + 128 - 1) / 128;

    while (numBlocks > 0) {
        if (numBlocks < 128) {
            LCStoreBlocks(destAddr, srcAddr, numBlocks);
            numBlocks = 0;
        } else {
            LCStoreBlocks(destAddr, srcAddr, 0);
            numBlocks -= 128;
            destAddr = (void *)((u32)destAddr + 4096);
            srcAddr = (void *)((u32)srcAddr + 4096);
        }
    }

    return numTransactions;
}
void LCStoreBlocks(void *destAddr, void *srcAddr, u32 numBlocks)
{
    ASM_INSN(rlwinm r6, r5, 30, 27, 31);
    ASM_INSN(clrlwi r3, r3, 4);
    ASM_INSN(or r6, r6, r3);
    ASM_INSN(mtspr 922, r6);
    ASM_INSN(rlwinm r6, r5, 2, 28, 29);
    ASM_INSN(or r6, r6, r4);
    ASM_INSN(ori r6, r6, 0x2);
    ASM_INSN(mtspr 923, r6);
}

#pragma peephole on
void LCQueueWait(u32 len)
{
    ASM_INSN(addi r3, r3, 1);
    ASM_INSN(loop: mfspr r4, 920);
    ASM_INSN(rlwinm r4, r4, 8, 28, 31);
    ASM_INSN(cmpw cr2, r4, r3);
    ASM_INSN(bge cr2, loop);
}

#pragma peephole off
void L2GlobalInvalidate(void)
{
    L2Disable();
    PPCMtl2cr(PPCMfl2cr() | 0x00200000);
    while (PPCMfl2cr() & 0x00000001u)
        ;
    PPCMtl2cr(PPCMfl2cr() & ~0x00200000);
    while (PPCMfl2cr() & 0x00000001u) {
        DBPrintf(">>> L2 INVALIDATE : SHOULD NEVER HAPPEN\n");
    }
}
void DMAErrorHandler(OSError error, OSContext *context, ...)
{
    u32 hid2 = PPCMfhid2();

    OSReport("Machine check received\n");
    OSReport("HID2 = 0x%x   SRR1 = 0x%x\n", hid2, context->srr1);
    if (!(hid2 & (HID2_DCHERR | HID2_DNCERR | HID2_DCMERR | HID2_DQOERR)) ||
        !(context->srr1 & SRR1_DMA_BIT)) {
        OSReport("Machine check was not DMA/locked cache related\n");
        OSDumpContext(context);
        PPCHalt();
    }

    OSReport("DMAErrorHandler(): An error occurred while processing DMA.\n");
    OSReport("The following errors have been detected and cleared :\n");

    if (hid2 & HID2_DCHERR) {
        OSReport("\t- Requested a locked cache tag that was already in the cache\n");
    }

    if (hid2 & HID2_DNCERR) {
        OSReport("\t- DMA attempted to access normal cache\n");
    }

    if (hid2 & HID2_DCMERR) {
        OSReport("\t- DMA missed in data cache\n");
    }

    if (hid2 & HID2_DQOERR) {
        OSReport("\t- DMA queue overflowed\n");
    }

    PPCMthid2(hid2);
}
void __OSCacheInit(void)
{
    if (!(PPCMfhid0() & HID0_ICE)) {
        ICEnable();
        DBPrintf("L1 i-caches initialized\n");
    }
    if (!(PPCMfhid0() & HID0_DCE)) {
        DCEnable();
        DBPrintf("L1 d-caches initialized\n");
    }

    if (!(PPCMfl2cr() & L2CR_L2E)) {
        L2Init();
        L2Enable();
        DBPrintf("L2 cache initialized\n");
    }

    OSSetErrorHandler(OS_ERROR_MACHINE_CHECK, DMAErrorHandler);
    DBPrintf("Locked cache machine check handler installed\n");
}

u32 DCEnable(void)
{
    register u32 hid0;

    __sync();
    ASM_MFSPR(hid0, 1008);
    hid0 |= HID0_DCE;
    ASM_MTSPR(1008, hid0);
    return hid0;
}

u32 ICFlashInvalidate(void)
{
    register u32 hid0;

    ASM_MFSPR(hid0, 1008);
    hid0 |= 0x800;
    ASM_MTSPR(1008, hid0);
    return hid0;
}

u32 ICEnable(void)
{
    register u32 hid0;

    __isync();
    ASM_MFSPR(hid0, 1008);
    hid0 |= HID0_ICE;
    ASM_MTSPR(1008, hid0);
    return hid0;
}

/* Reference: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/OSRtc.c. */
#include "exi.h"
#include "os_sram.h"
#include "os_interrupt.h"
#define NULL 0
#define TRUE 1
#define FALSE 0
extern void DCInvalidateRange(void *, u32);
struct SramControl {
    unsigned char sram[64];
    unsigned long offset;
    int enabled;
    int locked;
    int sync;
    void (*callback)();
};

static struct SramControl Scb __attribute__((aligned(32)));

static inline int ReadSram(void *buffer);
static void WriteSramCallback(s32 channel, OSContext *context);
static int WriteSram(void *buffer, u32 offset, u32 size);
static inline void *LockSram(u32 offset);
static int UnlockSram(int commit, u32 offset);

static inline int ReadSram(void *buffer)
{
    int err;
    u32 cmd;

    DCInvalidateRange(buffer, 0x40);
    if (!EXILock(0, 1, NULL)) {
        return 0;
    }
    if (!EXISelect(0, 1, EXI_FREQ_8M)) {
        EXIUnlock(0);
        return 0;
    }
    cmd = 0x20000100;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, EXI_WRITE, 0);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, 0x40, EXI_READ, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

static void WriteSramCallback(s32 channel, OSContext *context)
{
    Scb.sync = WriteSram(&Scb.sram[Scb.offset], Scb.offset, 0x40 - Scb.offset);
    if (Scb.sync != 0) {
        Scb.offset = 0x40;
    }
}

static int WriteSram(void *buffer, u32 offset, u32 size)
{
    int err;
    u32 cmd;

    if (!EXILock(0, 1, WriteSramCallback)) {
        return 0;
    }
    if (!EXISelect(0, 1, EXI_FREQ_8M)) {
        EXIUnlock(0);
        return 0;
    }
    offset <<= 6;
    cmd = ((offset + 0x100) | 0xA0000000);
    err = 0;
    err |= !EXIImm(0, &cmd, 4, EXI_WRITE, 0);
    err |= !EXISync(0);
    err |= !EXIImmEx(0, buffer, size, EXI_WRITE);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

void __OSInitSram()
{
    Scb.locked = Scb.enabled = 0;
    Scb.sync = ReadSram(&Scb);
    Scb.offset = 0x40;
}

static inline void *LockSram(u32 offset)
{
    int enabled;

    enabled = OSDisableInterrupts();
    if (Scb.locked) {
        OSRestoreInterrupts(enabled);
        return NULL;
    }
    Scb.enabled = enabled;
    Scb.locked = 1;
    return &Scb.sram[offset];
}

OSSram *__OSLockSram()
{
    return (OSSram *)LockSram(0);
}

OSSramEx *__OSLockSramEx(void)
{
    return (OSSramEx *)LockSram(0x14);
}

static int UnlockSram(int commit, u32 offset)
{
    u16 *p;

    if (commit != 0) {
        if (offset == 0) {
            OSSram *sram = (OSSram *)Scb.sram;

            if (2u < (sram->flags & 3)) {
                sram->flags &= ~3;
            }

            sram->checkSum = sram->checkSumInv = 0;
            for (p = (u16 *)&sram->counterBias; p < ((u16 *)&Scb.sram[sizeof(OSSram)]);
                 p++) {
                sram->checkSum += *p;
                sram->checkSumInv += ~(*p);
            }
        }
        if (offset < Scb.offset) {
            Scb.offset = offset;
        }
        Scb.sync = WriteSram(&Scb.sram[Scb.offset], Scb.offset, 0x40 - Scb.offset);
        if (Scb.sync != 0) {
            Scb.offset = 0x40;
        }
    }
    Scb.locked = 0;
    OSRestoreInterrupts(Scb.enabled);
    return Scb.sync;
}

int __OSUnlockSram(int commit)
{
    return UnlockSram(commit, 0);
}

int __OSUnlockSramEx(int commit)
{
    return UnlockSram(commit, 0x14);
}

int __OSSyncSram()
{
    return Scb.sync;
}

int __OSReadROM(void *buffer, s32 length, s32 offset)
{
    int err;
    u32 cmd;

    DCInvalidateRange(buffer, length);
    if (EXILock(0, 1, NULL) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, EXI_FREQ_8M) == 0) {
        EXIUnlock(0);
        return 0;
    }
    cmd = offset << 6;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, EXI_WRITE, 0);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, length, EXI_READ, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

u32 OSGetSoundMode()
{
    struct OSSram *sram = __OSLockSram();
    u32 mode = (sram->flags & 4) ? 1 : 0;

    __OSUnlockSram(0);
    return mode;
}

u16 OSGetWirelessID(s32 channel)
{
    OSSramEx *sram;
    u16 id;

    sram = __OSLockSramEx();
    id = sram->wirelessPadID[channel];
    __OSUnlockSramEx(FALSE);
    return id;
}

void OSSetWirelessID(s32 channel, u16 id)
{
    OSSramEx *sram;

    sram = __OSLockSramEx();
    if (sram->wirelessPadID[channel] != id) {
        sram->wirelessPadID[channel] = id;
        __OSUnlockSramEx(TRUE);
        return;
    }

    __OSUnlockSramEx(FALSE);
}

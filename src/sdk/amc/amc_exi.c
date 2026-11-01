#include "exi.h"
#include "os.h"
#include "os_interrupt.h"

#define TRUE 1
#define FALSE 0

#define STATE_BUSY 0x03
#define STATE_IMM 0x02
#define STATE_SELECTED 0x04

extern volatile u32 __PIRegs[16];
__asm__(".set __PIRegs, 0xCC003000");
extern volatile u32 __EXIRegs[15];
__asm__(".set __EXIRegs, 0xCC006800");
extern s32 OSDisableInterrupts(void);
extern s32 OSRestoreInterrupts(s32);
extern void *memset(void *, int, u32);
extern int printf(const char *, ...);

typedef struct AmcEXIControl {
    EXICallback exiCallback;
    EXICallback tcCallback;
    EXICallback extCallback;
    volatile u32 state;
    int immLen;
    u8 *immBuf;
} AmcEXIControl;

static AmcEXIControl Ecb;
static AmcEXIControl *exi = &Ecb;

void AmcEXIClearInterrupts(s32 debug, s32 tc);
s32 AmcEXISelect(u32 freq);
s32 AmcEXIDeselect(void);

s32 AmcEXIImm(void *buf, s32 len, u32 type, EXICallback callback)
{
    s32 chan = 2;
    s32 enabled;

    enabled = OSDisableInterrupts();
    if ((exi->state & STATE_BUSY) || !(exi->state & STATE_SELECTED)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }
    exi->tcCallback = callback;
    if (exi->tcCallback) {
        AmcEXIClearInterrupts(FALSE, TRUE);
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_TC >> (3 * chan));
    }
    exi->state |= STATE_IMM;
    if (type != EXI_READ) {
        u32 data;
        long i;

        data = 0;
        for (i = 0; i < len; i++)
            data |= ((u8 *)buf)[i] << ((3 - i) * 8);
        __EXIRegs[14] = data;
    }
    exi->immBuf = buf;
    exi->immLen = (type != EXI_WRITE) ? len : 0;
    __EXIRegs[13] = (type << 2) | 1 | ((len - 1) << 4);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

static inline void CompleteTransfer(void)
{
    u8 *buf;
    u32 data;
    int i;
    int len;

    if (exi->state & STATE_BUSY) {
        if ((exi->state & STATE_IMM) && (len = exi->immLen)) {
            buf = exi->immBuf;
            data = __EXIRegs[14];
            for (i = 0; i < len; i++)
                *buf++ = (u8)((data >> ((3 - i) * 8)) & 0xff);
        }
        exi->state &= ~STATE_BUSY;
    }
}

void AmcEXISync(void)
{
    s32 enabled;

    while (exi->state & STATE_SELECTED) {
        if (((__EXIRegs[13] & 1) >> 0) == 0) {
            enabled = OSDisableInterrupts();
            CompleteTransfer();
            OSRestoreInterrupts(enabled);
            break;
        }
    }
}

void AmcEXIClearInterrupts(s32 debug, s32 tc)
{
    if (debug)
        __PIRegs[0] = (__PIRegs[0] & 0x1000) | 0x1000;
    if (tc)
        __EXIRegs[10] = (__EXIRegs[10] & 0x7f5) | 8;
}

EXICallback AmcEXISetExiCallback(EXICallback callback)
{
    EXICallback prev;
    s32 enabled;

    enabled = OSDisableInterrupts();
    if (callback)
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_DEBUG);
    else
        __OSMaskInterrupts(OS_INTERRUPTMASK_PI_DEBUG);
    prev = exi->exiCallback;
    exi->exiCallback = callback;
    OSRestoreInterrupts(enabled);
    return prev;
}

s32 AmcEXISelect(u32 freq)
{
    u32 cpr;
    s32 enabled;

    enabled = OSDisableInterrupts();
    if (exi->state & STATE_SELECTED) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }
    exi->state |= STATE_SELECTED;
    cpr = __EXIRegs[10];
    cpr &= 0x405;
    cpr |= (1 << 7) | (freq << 4);
    __EXIRegs[10] = cpr;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

s32 AmcEXIDeselect(void)
{
    s32 enabled;

    enabled = OSDisableInterrupts();
    if (!(exi->state & STATE_SELECTED)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }
    exi->state &= ~STATE_SELECTED;
    __EXIRegs[10] &= 0x405;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

void AmcDebugIntHandler(__OSInterrupt interrupt, OSContext *context)
{
    EXICallback callback;

    __PIRegs[0] = (__PIRegs[0] & 0x1000) | 0x1000;
    callback = exi->exiCallback;
    if (callback)
        callback(2, context);
}

void AmcEXIEnableInterrupts(void)
{
    __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_2_TC);
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_DEBUG, AmcDebugIntHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_DEBUG);
}

void AmcEXIInit(void)
{
    memset(&Ecb, 0, sizeof(Ecb));
    __EXIRegs[10] = 0;
    *(volatile u32 *)0xCC00643C = 0;
}

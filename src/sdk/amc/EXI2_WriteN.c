#include "exi.h"
#include "os.h"
#include "os_interrupt.h"

#define FALSE 0

extern volatile u32 __PIRegs[16];
__asm__(".set __PIRegs, 0xCC003000");
extern s32 OSDisableInterrupts(void);
extern s32 OSRestoreInterrupts(s32);
extern int printf(const char *, ...);
extern s32 AmcEXIImm(void *buf, s32 len, u32 type, EXICallback callback);
extern void AmcEXISync(void);
extern EXICallback AmcEXISetExiCallback(EXICallback callback);
extern s32 AmcEXISelect(u32 freq);
extern s32 AmcEXIDeselect(void);
extern void AmcEXIEnableInterrupts(void);
extern void AmcEXIInit(void);

static volatile u8 ucEXI2InputPending;
static volatile u8 *pucEXI2InputPending = &ucEXI2InputPending;
static s32 fExi2Selected;
static EXICallback TRK_Callback;

static inline void exi2_send(u32 *buffer, u32 words, u32 *length)
{
    u32 i;
    s32 size;
    u32 last;

    last = words - 1;
    for (i = 0; i < words; i++) {
        if (i < last)
            size = 4;
        else if ((*length & 3) + (*length & 1) == 2)
            size = 2;
        else
            size = 4;
        AmcEXIImm(buffer++, size, EXI_WRITE, 0);
        AmcEXISync();
    }
}

s32 EXI2_WriteN(const void *data, u32 count)
{
    u32 length = count;
    u32 reply;
    u32 pad0[2];
    u32 cmd;
    u32 pad1[1];
    u32 status;
    s32 ret;
    u32 *buffer;

    buffer = (u32 *)data;
    ret = FALSE;
    while (ret == FALSE)
        ret = AmcEXISelect(5);
    if (ret == FALSE)
        return 1;
    cmd = 0x80000000;
    AmcEXIImm(&cmd, 2, EXI_WRITE, 0);
    AmcEXISync();
    AmcEXIImm(&length, 4, EXI_WRITE, 0);
    AmcEXISync();
    exi2_send(buffer, length / 4 + ((length & 3) ? 1 : 0), &length);
    AmcEXIDeselect();
    ret = FALSE;
    while (ret == FALSE)
        ret = AmcEXISelect(5);
    if (ret == FALSE)
        return 1;
    status = 0x10000;
    AmcEXIImm(&status, 2, EXI_WRITE, 0);
    AmcEXISync();
    AmcEXIImm(&reply, 2, EXI_READ, 0);
    AmcEXISync();
    do {
        AmcEXIImm(&reply, 2, EXI_READ, 0);
        AmcEXISync();
    } while (!((reply >> 16) & 1));
    AmcEXIDeselect();
    return 0;
}


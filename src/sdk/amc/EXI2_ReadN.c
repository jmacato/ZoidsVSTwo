#include "exi.h"
#include "os.h"
#include "os_interrupt.h"

#define FALSE 0

extern volatile u32 __PIRegs[16] : 0xCC003000;
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

static inline void exi2_recv(u32 *data, u8 *buffer, u32 last, s32 length)
{
    u32 offset;
    s32 rest;
    u32 odd;
    u32 i;
    s32 count;
    s32 k;

    odd = (length & 3) + (length & 1);
    rest = length % 4;
    for (i = offset = 0; offset < length; i++, offset += 4) {
        if (i < last)
            count = 4;
        else if (odd == 2)
            count = 2;
        else
            count = 4;
        AmcEXIImm(data, count, EXI_READ, 0);
        AmcEXISync();
        if ((u32)(k = length - offset) >= 4)
            k = 4;
        else
            k = rest;
        for (count = 0; count < k; count++)
            buffer[(s32)offset + count] = *data >> ((3 - count) * 8);
    }
}

s32 EXI2_ReadN(u8 *buffer, s32 length)
{
    u32 data;
    u32 status;
    u32 unused;
    u32 cmd;
    s32 ret;
    s32 sel;
    u32 words;

    if (fExi2Selected) {
        ret = 1;
    } else {
        sel = FALSE;
        while (sel == FALSE)
            sel = AmcEXISelect(5);
        ret = sel;
        if (ret == FALSE) {
            printf("Can't select EXI2 port!\n");
            ret = FALSE;
        } else {
            fExi2Selected = 1;
            cmd = 0;
            AmcEXIImm(&cmd, 2, EXI_WRITE, 0);
            AmcEXISync();
            AmcEXIImm(&status, 2, EXI_READ, 0);
            AmcEXISync();
        }
    }
    if (ret == FALSE)
        return 1;
    words = (u32)length / 4 + ((length & 3) ? 1 : 0);
    exi2_recv(&data, buffer, words - 1, length);
    AmcEXIDeselect();
    fExi2Selected = 0;
    *pucEXI2InputPending = 0;
    return 0;
}

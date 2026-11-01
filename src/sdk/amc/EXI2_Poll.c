#include "exi.h"
#include "os.h"
#include "os_interrupt.h"

#define FALSE 0
#define TRUE 1

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

static u8 ucEXI2InputPending;
static u8 *pucEXI2InputPending = &ucEXI2InputPending;
static s32 fExi2Selected;
static void (*TRK_Callback)(s32, OSContext *);

void EXI2_CallBack(s32 chan, OSContext *context)
{
    *pucEXI2InputPending = 1;
    if (TRK_Callback)
        TRK_Callback(0, context);
}

void EXI2_EnableInterrupts(void)
{
    AmcEXISetExiCallback(EXI2_CallBack);
    AmcEXIEnableInterrupts();
}

s32 EXI2_Poll(void)
{
    u32 len;
    u32 status;
    s32 unused[2];
    u32 cmd;
    s32 ret;

    if (!(__PIRegs[0] & 0x1000) && *pucEXI2InputPending == 0) {
        len = 0;
    } else {
        if (fExi2Selected == FALSE) {
            ret = FALSE;
            while (ret == FALSE)
                ret = AmcEXISelect(5);
            if (ret == FALSE) {
                printf("Can't select EXI2 port!\n");
            } else {
                fExi2Selected = 1;
                cmd = 0;
                AmcEXIImm(&cmd, 2, EXI_WRITE, 0);
                AmcEXISync();
                AmcEXIImm(&status, 2, EXI_READ, 0);
                AmcEXISync();
            }
        }
        AmcEXIImm(&len, 4, EXI_READ, 0);
        AmcEXISync();
        if (*pucEXI2InputPending == 0) {
            u32 v = __PIRegs[0] & 0x1000;
            *pucEXI2InputPending = 1;
            __PIRegs[0] = v;
        }
    }
    return len;
}

void EXI2_Reserve(void)
{
}

void EXI2_Unreserve(void)
{
}

s32 AMC_IsStub(void)
{
    return 1;
}


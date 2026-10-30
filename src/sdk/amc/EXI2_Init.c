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

static inline void exi2_cmd(void)
{
    u32 cmd[3];

    cmd[0] = 0x80000000;
    AmcEXIImm(cmd, 2, EXI_WRITE, 0);
    AmcEXISync();
}

static inline void exi2_open(u32 data, u32 id)
{
    s32 selected = 0;

    while (selected == 0)
        selected = AmcEXISelect(5);
    exi2_cmd();
    AmcEXIImm(&id, 4, EXI_WRITE, 0);
    AmcEXISync();
    AmcEXIImm(&data, 4, EXI_WRITE, 0);
    AmcEXISync();
    AmcEXIDeselect();
}

void EXI2_Init(volatile u8 **inputPendingPtrRef, EXICallback monitorCallback)
{
    s32 enabled;

    *inputPendingPtrRef = pucEXI2InputPending;
    enabled = OSDisableInterrupts();
    TRK_Callback = monitorCallback;
    OSRestoreInterrupts(enabled);
    AmcEXIInit();
    exi2_open(0, 0x01000003);
}

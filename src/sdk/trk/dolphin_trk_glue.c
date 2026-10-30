#include "types.h"
#include "metrotrk.h"

void TRKUARTInterruptHandler(void)
{
}

typedef struct DBCommTable {
    void (*initialize)(void **pending, void (*callback)(s32, void *));
    void (*interrupts)(void);
    s32 (*poll)(void);
    s32 (*read)(void *data, u32 length);
    s32 (*write)(const void *data, u32 length);
    void (*open)(void);
    void (*close)(void);
} DBCommTable;

extern DBCommTable gDBCommTable;
extern void OSReport(const char *format, ...);

s32 TRKInitializeIntDrivenUART(u32 baudRate, s32 interruptDriven, s32 reserved, void **pending)
{
    gDBCommTable.initialize(pending, TRKEXICallBack);
    return 0;
}

void EnableEXI2Interrupts(void)
{
    gDBCommTable.interrupts();
}

s32 TRKPollUART(void)
{
    return gDBCommTable.poll();
}

s32 TRK_ReadUARTN(void *data, u32 length)
{
    if (gDBCommTable.read(data, length) == 0)
        return 0;
    return -1;
}

s32 TRK_WriteUARTN(const void *data, u32 length)
{
    if (gDBCommTable.write(data, length) == 0)
        return 0;
    return -1;
}

void ReserveEXI2Port(void)
{
    gDBCommTable.open();
}

void UnreserveEXI2Port(void)
{
    gDBCommTable.close();
}

void TRK_board_display(char *text)
{
    OSReport(text);
}

extern void OSEnableScheduler(void);
extern void TRKLoadContext(void *context, u32 exception);

void TRKEXICallBack(s32 interrupt, void *context)
{
    OSEnableScheduler();
    TRKLoadContext(context, PPC_EXTERNALINTERRUPT);
}

extern s32 Hu_IsStub(void);
extern s32 AMC_IsStub(void);
extern void DBInitComm(void **pending, void (*callback)(s32, void *));
extern void DBInitInterrupts(void);
extern u32 DBQueryData(void);
extern u32 DBRead(void *data, u32 length);
extern u32 DBWrite(const void *data, u32 length);
extern void DBOpen(void);
extern void DBClose(void);
extern void EXI2_Init(void **pending, void (*callback)(s32, void *));
extern void EXI2_EnableInterrupts(void);
extern s32 EXI2_Poll(void);
extern s32 EXI2_ReadN(void *data, u32 length);
extern s32 EXI2_WriteN(const void *data, u32 length);
extern void EXI2_Reserve(void);
extern void EXI2_Unreserve(void);

/* Interface selection: https://github.com/doldecomp/melee/blob/master/src/MetroTRK/dolphin_trk_glue.c */
s32 InitMetroTRKCommTable(s32 hardware)
{
    s32 isStub;
    if (hardware == 1) {
        isStub = Hu_IsStub();
        gDBCommTable.initialize = DBInitComm;
        gDBCommTable.interrupts = DBInitInterrupts;
        gDBCommTable.poll = (s32 (*)(void))DBQueryData;
        gDBCommTable.read = (s32 (*)(void *, u32))DBRead;
        gDBCommTable.write = (s32 (*)(const void *, u32))DBWrite;
        gDBCommTable.open = DBOpen;
        gDBCommTable.close = DBClose;
    } else {
        isStub = AMC_IsStub();
        gDBCommTable.initialize = EXI2_Init;
        gDBCommTable.interrupts = EXI2_EnableInterrupts;
        gDBCommTable.poll = EXI2_Poll;
        gDBCommTable.read = EXI2_ReadN;
        gDBCommTable.write = EXI2_WriteN;
        gDBCommTable.open = EXI2_Reserve;
        gDBCommTable.close = EXI2_Unreserve;
    }
    return isStub;
}

#include "types.h"
#include "metrotrk.h"

s32 TRKTerminateNub(void)
{
    TRKTerminateSerialHandler();
    return 0;
}

void TRKNubWelcome(void)
{
    TRK_board_display("MetroTRK for Dolphin v0.5");
}

s32 TRKInitializeEndian(void)
{
    s32 error = 0;
    u8 bytes[4];
    gTRKBigEndian = 1;
    bytes[0] = 0x12;
    bytes[1] = 0x34;
    bytes[2] = 0x56;
    bytes[3] = 0x78;
    if (*(u32 *)bytes == 0x12345678)
        gTRKBigEndian = 1;
    else if (*(u32 *)bytes == 0x78563412)
        gTRKBigEndian = 0;
    else
        error = 1;
    return error;
}

extern void *gTRKInputPendingPtr;

s32 TRKInitializeNub(void)
{
    s32 error;
    s32 uartError;
    error = TRKInitializeEndian();
    if (error == 0)
        usr_put_initialize();
    if (error == 0)
        error = TRKInitializeEventQueue();
    if (error == 0)
        error = TRKInitializeMessageBuffers();
    if (error == 0)
        error = TRKInitializeDispatcher();
    if (error == 0) {
        uartError = TRKInitializeIntDrivenUART(57600, 1, 0, &gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
        if (uartError != 0)
            error = uartError;
    }
    if (error == 0)
        error = TRKInitializeSerialHandler();
    if (error == 0)
        error = TRKInitializeTarget();
    return error;
}

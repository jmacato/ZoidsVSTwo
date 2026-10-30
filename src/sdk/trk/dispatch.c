#include "types.h"
#include "metrotrk.h"

extern u32 gTRKDispatchTableSize;

s32 TRKInitializeDispatcher(void)
{
    gTRKDispatchTableSize = 32;
    return 0;
}

extern s32 (*gTRKDispatchTable[33])(void *buffer);

s32 TRKDispatchMessage(TRKBuffer *buffer)
{
    s32 error = kDispatchError;
    u8 command;
    TRKSetBufferPosition(buffer, 0);
    TRKReadBuffer1_ui8(buffer, &command);
    if (command < gTRKDispatchTableSize)
        error = gTRKDispatchTable[command](buffer);
    return error;
}

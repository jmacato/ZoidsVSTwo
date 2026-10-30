#include "types.h"
#include "metrotrk.h"

extern s32 TRK_mainError;

s32 TRK_main(void)
{
    TRK_mainError = TRKInitializeNub();
    if (TRK_mainError == 0) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }
    return TRK_mainError = TRKTerminateNub();
}

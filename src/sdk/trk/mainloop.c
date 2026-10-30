#include "types.h"
#include "metrotrk.h"

void TRKHandleRequestEvent(TRKEvent *event)
{
    TRKDispatchMessage(TRKGetBuffer(event->msgBufID));
}

void TRKHandleSupportEvent(TRKEvent *event)
{
    TRKTargetSupportRequest();
}

void TRKIdle(void)
{
    if (TRKTargetStopped() == 0)
        TRKTargetContinue();
}

extern u8 *gTRKInputPendingPtr;

void TRKNubMainLoop(void)
{
    TRKEvent event;
    s32 shutdown = 0;
    s32 inputChecked = 0;
    while (!shutdown) {
        if (TRKGetNextEvent(&event)) {
            inputChecked = 0;
            switch (event.eventType) {
            case kNullEvent:
                break;
            case kRequestEvent:
                TRKHandleRequestEvent(&event);
                break;
            case kShutdownEvent:
                shutdown = 1;
                break;
            case kBreakpointEvent:
            case kExceptionEvent:
                TRKTargetInterrupt(&event);
                break;
            case kSupportEvent:
                TRKHandleSupportEvent(&event);
                break;
            }
            TRKDestructEvent(&event);
        } else if (!inputChecked || *gTRKInputPendingPtr) {
            inputChecked = 1;
            TRKGetInput();
        } else {
            TRKIdle();
            inputChecked = 0;
        }
    }
}

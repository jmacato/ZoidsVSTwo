#include "types.h"

typedef struct PadState3 {
    s8 connected;
    s8 previous_connected;
    u8 reserved_02[2];
    u32 buttons;
    s32 field_08;
    s32 reset_hold_frames;
    u8 reserved_10[0x74];
    s8 rumble;
    s8 previous_rumble;
    u8 reserved_86[2];
} PadState3;

extern PadState3 *sPad3;
extern PadState3 *sPad4;
extern s32 (*sPad4dbgFunc)(void);
extern s32 reset2;
extern s32 noPush;
extern s32 df999;
extern s32 sPadResetSTOP;
extern s32 sPad4dbgMenu(void);
extern void dInt(void);
extern void HSD_PadRenewStatus(void);
extern void sPadGet1(s32 port);
extern s32 IsReset(void);
extern void TEA_HotReset(u32 code);

void sPadAll(void)
{
    s32 i;
    s32 reset;

    dInt();
    HSD_PadRenewStatus();
    for (i = 0; i < 2; i++) {
        sPadGet1(i);
    }
    if (sPad4->connected == 0) {
        sPad4dbgFunc = 0;
    } else if (sPad4->previous_connected == 0) {
        sPad4dbgFunc = sPad4dbgMenu;
    }
    sPad3->reset_hold_frames = 0;
    sPad4->reset_hold_frames = 0;
    if (IsReset() != 0) {
        reset2 = 1;
    } else {
        if (reset2 != 0) {
            reset2 = 0;
            TEA_HotReset(0);
        }
    }
}

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

extern PadState3 sPad[4];
extern s32 reset2;
extern void ZeroMemory(void *address, u32 size);

void sPadInit(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        ZeroMemory(&sPad[i], sizeof(PadState3));
        sPad[i].field_08 = 0;
    }
    reset2 = 0;
}

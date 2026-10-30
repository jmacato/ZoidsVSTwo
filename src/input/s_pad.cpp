extern "C" {
#pragma readonly_strings on
#pragma bool off
#include "types.h"
#include "controller.h"

typedef struct PadState {
    s8 connected;
    s8 previous_connected;
    u8 reserved_02[2];
    u32 buttons;
    s32 field_08;
    s32 reset_hold_frames;
    u32 repeat_dpad_up;
    u32 repeat_dpad_down;
    u32 repeat_dpad_left;
    u32 repeat_dpad_right;
    u32 repeat_stick_up;
    u32 repeat_stick_down;
    u32 repeat_stick_left;
    u32 repeat_stick_right;
    s8 dpad_up;
    s8 dpad_down;
    s8 dpad_left;
    s8 dpad_right;
    s8 stick_up;
    s8 stick_down;
    s8 stick_left;
    s8 stick_right;
    s8 up;
    s8 down;
    s8 held_left;
    s8 held_right;
    s8 last_stick_up;
    s8 last_stick_down;
    s8 last_stick_left;
    s8 last_stick_right;
    s8 l;
    s8 r;
    s8 a;
    s8 b;
    u8 reserved_44;
    s8 x;
    s8 y;
    s8 z;
    s8 last_a;
    s8 last_b;
    u8 reserved_4A;
    s8 last_x;
    s8 last_y;
    s8 last_z;
    s8 start;
    s8 pressed_dpad_up;
    s8 pressed_dpad_down;
    s8 pressed_dpad_left;
    s8 pressed_dpad_right;
    s8 pressed_stick_up;
    s8 pressed_stick_down;
    s8 pressed_stick_left;
    s8 pressed_stick_right;
    s8 pressed_up;
    s8 pressed_down;
    s8 left;
    s8 right;
    s8 pressed_l;
    s8 pressed_r;
    s8 pressed_a;
    s8 pressed_b;
    u8 reserved_5F;
    s8 pressed_x;
    s8 pressed_y;
    s8 pressed_z;
    s8 pressed_start;
    f32 stick_x;
    f32 stick_y;
    f32 substick_x;
    f32 substick_y;
    f32 analog_l;
    f32 analog_r;
    f32 analog_a;
    f32 analog_b;
    s8 rumble;
    s8 previous_rumble;
    u8 reserved_86[2];
} PadState;

typedef char PadStateSizeCheck[sizeof(PadState) == 0x88 ? 1 : -1];

typedef struct GameModeState {
    u8 reserved_00[4];
    s32 game_type;
    u8 reserved_08[0x34];
    s32 field_44;
    u8 reserved_40[8];
} GameModeState;

typedef char GameModeStateSizeCheck[sizeof(GameModeState) == 0x48 ? 1 : -1];

extern PadState sPad[9];
extern GameModeState GameMode;
extern s32 sPadRep;
extern void TEA_HotReset(u32 code);

inline f32 absf(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}
#pragma force_active on
static const char lit_613[23] = "addr = %X - %X prg%X \n";
#pragma force_active off

s32 sPad4dbgMenu(void)
{
    return -1;
}

void sPadGet1(s32 port)
{
    PadState *pad;
    s32 button;
    f32 x;
    f32 y;
    f32 ax;
    f32 ay;
    s32 reset;

    if (port < 0)
        return;
    if (port > 3)
        return;
    if (port < 2 && GameMode.game_type == 9 && GameMode.field_44 != 0) {
        button = HSD_PadCopyStatus[(u8)port].button;
        if (button & 0x1000)
            GameMode.field_44 = 2;
        return;
    }
    pad = &sPad[port];
    pad->previous_connected = pad->connected;
    pad->connected = HSD_PadCopyStatus[(u8)port].err == 0;
    button = HSD_PadCopyStatus[(u8)port].trigger;
    pad->pressed_a = (button & 0x100) != 0;
    pad->pressed_b = (button & 0x200) != 0;
    pad->pressed_l = (button & 0x40) != 0;
    pad->pressed_r = (button & 0x20) != 0;
    pad->pressed_x = (button & 0x400) != 0;
    pad->pressed_y = (button & 0x800) != 0;
    pad->pressed_z = (button & 0x10) != 0;
    pad->pressed_start = (button & 0x1000) != 0;
    pad->pressed_dpad_left = (button & 0x1) != 0;
    pad->pressed_dpad_right = (button & 0x2) != 0;
    pad->pressed_dpad_up = (button & 0x8) != 0;
    pad->pressed_dpad_down = (button & 0x4) != 0;
    button = HSD_PadCopyStatus[(u8)port].button;
    pad->buttons = button;
    pad->last_a = pad->a;
    pad->last_b = pad->b;
    pad->last_x = pad->x;
    pad->last_y = pad->y;
    pad->last_z = pad->z;
    pad->a = (button & 0x100) != 0;
    pad->b = (button & 0x200) != 0;
    pad->l = (button & 0x40) != 0;
    pad->r = (button & 0x20) != 0;
    pad->x = (button & 0x400) != 0;
    pad->y = (button & 0x800) != 0;
    pad->z = (button & 0x10) != 0;
    pad->start = (button & 0x1000) != 0;
    pad->dpad_left = (button & 0x1) != 0;
    pad->dpad_right = (button & 0x2) != 0;
    pad->dpad_up = (button & 0x8) != 0;
    pad->dpad_down = (button & 0x4) != 0;
    pad->stick_x = HSD_PadMasterStatus[(u8)port].nml_stickX;
    pad->stick_y = HSD_PadMasterStatus[(u8)port].nml_stickY;
    pad->substick_x = HSD_PadMasterStatus[(u8)port].nml_subStickX;
    pad->substick_y = HSD_PadMasterStatus[(u8)port].nml_subStickY;
    pad->analog_l = HSD_PadMasterStatus[(u8)port].nml_analogL;
    pad->analog_r = HSD_PadMasterStatus[(u8)port].nml_analogR;
    pad->analog_a = HSD_PadMasterStatus[(u8)port].nml_analogA;
    pad->analog_b = HSD_PadMasterStatus[(u8)port].nml_analogB;
    x = pad->stick_x;
    y = pad->stick_y;
    ax = absf(x);
    ay = absf(y);
    if (ax < 0.2) {
        pad->stick_left = 0;
        pad->stick_right = 0;
    }
    if (ay < 0.2) {
        pad->stick_up = 0;
        pad->stick_down = 0;
    }
    if (x > 0.5) {
        pad->stick_right = 1;
        pad->stick_left = 0;
    }
    if (x < -0.5) {
        pad->stick_right = 0;
        pad->stick_left = 1;
    }
    if (y > 0.5) {
        pad->stick_up = 1;
        pad->stick_down = 0;
    }
    if (y < -0.5) {
        pad->stick_up = 0;
        pad->stick_down = 1;
    }
    pad->pressed_stick_right = pad->stick_right && !pad->last_stick_right;
    pad->pressed_stick_left = pad->stick_left && !pad->last_stick_left;
    pad->pressed_stick_up = pad->stick_up && !pad->last_stick_up;
    pad->pressed_stick_down = pad->stick_down && !pad->last_stick_down;
    pad->last_stick_right = pad->stick_right;
    pad->last_stick_left = pad->stick_left;
    pad->last_stick_up = pad->stick_up;
    pad->last_stick_down = pad->stick_down;
    if (sPadRep != 0) {
        if (pad->dpad_up)
            pad->repeat_dpad_up++;
        else
            pad->repeat_dpad_up = 0;
        if (pad->stick_up)
            pad->repeat_stick_up++;
        else
            pad->repeat_stick_up = 0;
        if (pad->dpad_down)
            pad->repeat_dpad_down++;
        else
            pad->repeat_dpad_down = 0;
        if (pad->stick_down)
            pad->repeat_stick_down++;
        else
            pad->repeat_stick_down = 0;
        if (pad->dpad_right)
            pad->repeat_dpad_right++;
        else
            pad->repeat_dpad_right = 0;
        if (pad->stick_right)
            pad->repeat_stick_right++;
        else
            pad->repeat_stick_right = 0;
        if (pad->dpad_left)
            pad->repeat_dpad_left++;
        else
            pad->repeat_dpad_left = 0;
        if (pad->stick_left)
            pad->repeat_stick_left++;
        else
            pad->repeat_stick_left = 0;
        if ((pad->dpad_up || pad->stick_up) && (pad->dpad_down || pad->stick_down)) {
            pad->repeat_dpad_up = 0;
            pad->repeat_dpad_down = 0;
            pad->repeat_stick_up = 0;
            pad->repeat_stick_down = 0;
            pad->pressed_dpad_up = 0;
            pad->pressed_stick_up = 0;
            pad->pressed_dpad_down = 0;
            pad->pressed_stick_down = 0;
        }
        if ((pad->dpad_right || pad->stick_right) && (pad->dpad_left || pad->stick_left)) {
            pad->repeat_dpad_right = 0;
            pad->repeat_dpad_left = 0;
            pad->repeat_stick_right = 0;
            pad->repeat_stick_left = 0;
            pad->pressed_dpad_right = 0;
            pad->pressed_stick_right = 0;
            pad->pressed_dpad_left = 0;
            pad->pressed_stick_left = 0;
        }
        if (pad->repeat_dpad_up > 30 && (pad->repeat_dpad_up & 3) == 0)
            pad->pressed_dpad_up = 1;
        if (pad->repeat_stick_up > 30 && (pad->repeat_stick_up & 3) == 0)
            pad->pressed_stick_up = 1;
        if (pad->repeat_dpad_down > 30 && (pad->repeat_dpad_down & 3) == 0)
            pad->pressed_dpad_down = 1;
        if (pad->repeat_stick_down > 30 && (pad->repeat_stick_down & 3) == 0)
            pad->pressed_stick_down = 1;
        if (pad->repeat_dpad_right > 30 && (pad->repeat_dpad_right & 3) == 0)
            pad->pressed_dpad_right = 1;
        if (pad->repeat_stick_right > 30 && (pad->repeat_stick_right & 3) == 0)
            pad->pressed_stick_right = 1;
        if (pad->repeat_dpad_left > 30 && (pad->repeat_dpad_left & 3) == 0)
            pad->pressed_dpad_left = 1;
        if (pad->repeat_stick_left > 30 && (pad->repeat_stick_left & 3) == 0)
            pad->pressed_stick_left = 1;
    } else {
        pad->repeat_dpad_up = 0;
        pad->repeat_dpad_down = 0;
        pad->repeat_stick_up = 0;
        pad->repeat_stick_down = 0;
    }
    pad->up = pad->dpad_up | pad->stick_up;
    pad->down = pad->dpad_down | pad->stick_down;
    pad->held_left = pad->dpad_left | pad->stick_left;
    pad->held_right = pad->dpad_right | pad->stick_right;
    pad->pressed_up = pad->pressed_dpad_up | pad->pressed_stick_up;
    pad->pressed_down = pad->pressed_dpad_down | pad->pressed_stick_down;
    pad->left = pad->pressed_dpad_left | pad->pressed_stick_left;
    pad->right = pad->pressed_dpad_right | pad->pressed_stick_right;
    if (pad->rumble != pad->previous_rumble) {
        if (pad->rumble != 0)
            HSD_PadRumbleOn(port);
        else
            HSD_PadRumbleOffN(port);
        pad->previous_rumble = pad->rumble;
    }
    reset = pad->x && pad->b && pad->start;
    if (reset) {
        pad->reset_hold_frames++;
        if (pad->reset_hold_frames > 30)
            TEA_HotReset(0);
    } else {
        pad->reset_hold_frames = 0;
    }
    if (pad->connected == 0) {
        pad->stick_x = 0.0f;
        pad->stick_y = 0.0f;
    }
}
#pragma force_active on
s32 sPadRep;
static u32 reset2;
static u32 sPad4dbgFunc;
static u32 gap_802EF8E4;
#pragma force_active off
}

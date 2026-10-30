#ifndef ZOIDS_GAME_PAD_H
#define ZOIDS_GAME_PAD_H

#include "types.h"

typedef struct PadState {
    s8 connected;
    s8 previous_connected;
    u8 reserved_02[2];
    u32 buttons;
    s32 field_08;
    s32 reset_hold_frames;
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
    u8 reserved_24;
    s8 x;
    s8 y;
    s8 z;
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
    u8 reserved_39;
    s8 pressed_x;
    s8 pressed_y;
    s8 pressed_z;
    s8 pressed_start;
    u8 reserved_3E[2];
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
    u8 reserved_62[2];
} PadState;

typedef char PadStateSizeCheck[sizeof(PadState) == 0x64 ? 1 : -1];

extern PadState sPad[4];

#endif

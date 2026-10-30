/* Source and layouts:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/thp/THPAudio.c
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/include/dolphin/thp.h */
#include "types.h"

typedef struct THPAudioDecodeState {
    u8 *input;
    u32 nibble_index;
    u8 predictor;
    u8 scale;
    /* The SDK state retains history fields that this decoder keeps in local variables. */
    s16 previous, older;
} THPAudioDecodeState;

typedef struct THPAudioFrame {
    u32 channel_size, sample_count;
    s16 left_coefficients[8][2], right_coefficients[8][2];
    s16 left_history[2], right_history[2];
} THPAudioFrame;

static void __THPAudioInitialize(THPAudioDecodeState *state, u8 *input);
static s32 __THPAudioGetNewSample(THPAudioDecodeState *state);

#pragma dont_inline on
u32 THPAudioDecode(s16 *output, u8 *input, s32 layout)
{
    THPAudioFrame *frame;
    THPAudioDecodeState state;
    u8 *left_input;
    u8 *right_input;
    s16 *left_output, *right_output;
    s16 previous, older;
    s32 i;
    s32 stride;
    s32 excitation;
    s64 value;

    if (output == 0 || input == 0)
        return 0;
    frame = (THPAudioFrame *)input;
    left_input = input + sizeof(THPAudioFrame);
    right_input = left_input + frame->channel_size;
    if (layout == 1) {
        left_output = output + frame->sample_count;
        right_output = output;
        stride = 1;
    } else {
        left_output = output + 1;
        right_output = output;
        stride = 2;
    }
    if (frame->channel_size == 0) {
        __THPAudioInitialize(&state, left_input);
        previous = frame->left_history[0];
        older = frame->left_history[1];
        for (i = 0; i < frame->sample_count; i++) {
            excitation = __THPAudioGetNewSample(&state);
            value = frame->left_coefficients[state.predictor][1] * older;
            value += frame->left_coefficients[state.predictor][0] * previous;
            value += (s32)((u32)excitation << state.scale << 11);
            value <<= 5;
            if ((u16)(value & 0xffff) > 0x8000)
                value += 0x10000;
            else if ((u16)(value & 0xffff) == 0x8000 && (value & 0x10000))
                value += 0x10000;
            if (value > 0x7fffffffLL)
                value = 0x7fffffffLL;
            if (value < -0x80000000LL)
                value = -0x80000000LL;
            *left_output = (s16)(value >> 16);
            left_output += stride;
            *right_output = (s16)(value >> 16);
            right_output += stride;
            older = previous;
            previous = (s16)(value >> 16);
        }
    } else {
        __THPAudioInitialize(&state, left_input);
        previous = frame->left_history[0];
        older = frame->left_history[1];
        for (i = 0; i < frame->sample_count; i++) {
            excitation = __THPAudioGetNewSample(&state);
            value = frame->left_coefficients[state.predictor][1] * older;
            value += frame->left_coefficients[state.predictor][0] * previous;
            value += (s32)((u32)excitation << state.scale << 11);
            value <<= 5;
            if ((u16)(value & 0xffff) > 0x8000)
                value += 0x10000;
            else if ((u16)(value & 0xffff) == 0x8000 && (value & 0x10000))
                value += 0x10000;
            if (value > 0x7fffffffLL)
                value = 0x7fffffffLL;
            if (value < -0x80000000LL)
                value = -0x80000000LL;
            *left_output = (s16)(value >> 16);
            left_output += stride;
            older = previous;
            previous = (s16)(value >> 16);
        }
        __THPAudioInitialize(&state, right_input);
        previous = frame->right_history[0];
        older = frame->right_history[1];
        for (i = 0; i < frame->sample_count; i++) {
            excitation = __THPAudioGetNewSample(&state);
            value = frame->right_coefficients[state.predictor][1] * older;
            value += frame->right_coefficients[state.predictor][0] * previous;
            value += (s32)((u32)excitation << state.scale << 11);
            value <<= 5;
            if ((u16)(value & 0xffff) > 0x8000)
                value += 0x10000;
            else if ((u16)(value & 0xffff) == 0x8000 && (value & 0x10000))
                value += 0x10000;
            if (value > 0x7fffffffLL)
                value = 0x7fffffffLL;
            if (value < -0x80000000LL)
                value = -0x80000000LL;
            *right_output = (s16)(value >> 16);
            right_output += stride;
            older = previous;
            previous = (s16)(value >> 16);
        }
    }
    return frame->sample_count;
}

static s32 __THPAudioGetNewSample(THPAudioDecodeState *state)
{
    s32 sample;
    if ((state->nibble_index & 15) == 0) {
        state->predictor = (*state->input >> 4) & 7;
        state->scale = *state->input & 15;
        state->input++;
        state->nibble_index += 2;
    }
    if (state->nibble_index & 1) {
        sample = (s32)((u32)(*state->input & 15) << 28) >> 28;
        state->input++;
    } else {
        sample = (s32)((u32)(*state->input & 0xf0) << 24) >> 28;
    }
    state->nibble_index++;
    return sample;
}

static void __THPAudioInitialize(THPAudioDecodeState *state, u8 *input)
{
    state->input = input;
    state->nibble_index = 2;
    state->predictor = (*state->input >> 4) & 7;
    state->scale = *state->input & 15;
    state->input++;
}

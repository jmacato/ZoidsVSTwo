#include "types.h"

f32 cube4im(f32 *start, f32 *end, s32 axis)
{
    s32 first;
    s32 second;
    f32 t;
    s32 i;
    f32 delta[3];

    first = 1;
    second = 2;
    if (axis == 1) {
        first = 0;
    }
    if (axis == 2) {
        second = 0;
    }
    if (start[axis] > -0.5) {
        return -1.0f;
    }
    if (end[axis] < -0.5) {
        return -1.0f;
    }
    if (-0.5 == start[axis]) {
        if (start[first] < -0.5) {
            return -1.0f;
        }
        if (start[second] < -0.5) {
            return -1.0f;
        }
        if (start[first] > 0.5) {
            return -1.0f;
        }
        if (start[second] > 0.5) {
            return -1.0f;
        }
        return 0.0f;
    }
    if (-0.5 == end[axis]) {
        if (end[first] < -0.5) {
            return -1.0f;
        }
        if (end[second] < -0.5) {
            return -1.0f;
        }
        if (end[first] > 0.5) {
            return -1.0f;
        }
        if (end[second] > 0.5) {
            return -1.0f;
        }
        return 1.0f;
    }
    for (i = 0; i < 3; i++) {
        delta[i] = end[i] - start[i];
    }
    t = (-0.5 - start[axis]) / delta[axis];
    for (i = 0; i < 3; i++) {
        delta[i] = t * delta[i] + start[i];
    }
    if (delta[first] < -0.5) {
        return -1.0f;
    }
    if (delta[second] < -0.5) {
        return -1.0f;
    }
    if (delta[first] > 0.5) {
        return -1.0f;
    }
    if (delta[second] > 0.5) {
        return -1.0f;
    }
    return t;
}

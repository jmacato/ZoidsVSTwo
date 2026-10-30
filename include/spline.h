#ifndef ZOIDS_SPLINE_H
#define ZOIDS_SPLINE_H

#include "types.h"

/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/spline.h. */
#define SPLINE_LINEAR 0
#define SPLINE_BEZIER 1
#define SPLINE_BSPLINE 2
#define SPLINE_CARDINAL 3

typedef struct HSD_Spline {
    u8 type;
    s16 control_count;
    f32 tension;
    Vec3 *controls;
    f32 total_length;
    f32 *segment_lengths;
    f32 (*segment_polynomials)[5];
} HSD_Spline;

extern f32 splArcLengthGetParameter(HSD_Spline *spline, f32 distance);
extern void splGetSplinePoint(Vec3 *point, HSD_Spline *spline, f32 parameter);

#endif

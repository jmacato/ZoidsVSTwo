/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/spline.c. */
#include "spline.h"

static inline void splGetCardinalPoint(Vec3 *point, Vec3 *controls, f32 tension,
                                       f32 parameter)
{
    f32 square = parameter * parameter;
    f32 cube = square * parameter;
    f32 weight_0 = tension * (-cube + 2.0F * square - parameter);
    f32 weight_1 = ((2.0F - tension) * cube) + ((tension - 3.0F) * square) + 1.0F;
    f32 weight_2 = ((tension - 2.0F) * cube) + ((3.0F - (2.0F * tension)) * square) +
                   (tension * parameter);
    f32 weight_3 = tension * (cube - square);

    point->x = (controls[0].x * weight_0) + (controls[1].x * weight_1) +
               (controls[2].x * weight_2) + (controls[3].x * weight_3);
    point->y = (controls[0].y * weight_0) + (controls[1].y * weight_1) +
               (controls[2].y * weight_2) + (controls[3].y * weight_3);
    point->z = (controls[0].z * weight_0) + (controls[1].z * weight_1) +
               (controls[2].z * weight_2) + (controls[3].z * weight_3);
}

static inline void splGetBSplinePoint(Vec3 *point, Vec3 *controls, f32 parameter)
{
    f32 square = parameter * parameter;
    f32 cube = square * parameter;
    f32 complement = 1.0F - parameter;
    f32 one_sixth = (1.0F / 6.0F);
    f32 weight_0 = one_sixth * complement * complement * complement;
    f32 weight_1 = one_sixth * (4.0F + (3.0F * cube - 6.0F * square));
    f32 weight_2 = one_sixth * (3.0F * (-cube + square + parameter) + 1.0F);
    f32 weight_3 = one_sixth * cube;

    point->x = (controls[0].x * weight_0) + (controls[1].x * weight_1) +
               (controls[2].x * weight_2) + (controls[3].x * weight_3);
    point->y = (controls[0].y * weight_0) + (controls[1].y * weight_1) +
               (controls[2].y * weight_2) + (controls[3].y * weight_3);
    point->z = (controls[0].z * weight_0) + (controls[1].z * weight_1) +
               (controls[2].z * weight_2) + (controls[3].z * weight_3);
}

static inline void splGetBezierPoint(Vec3 *point, Vec3 *controls, f32 parameter)
{
    f32 complement = 1.0F - parameter;
    f32 square = parameter * parameter;
    f32 complement_squared = complement * complement;
    f32 weight_0 = complement_squared * complement;
    f32 weight_1 = 3.0F * parameter * complement_squared;
    f32 weight_2 = 3.0F * square * complement;
    f32 weight_3 = square * parameter;

    point->x = (controls[0].x * weight_0) + (controls[1].x * weight_1) +
               (controls[2].x * weight_2) + (controls[3].x * weight_3);
    point->y = (controls[0].y * weight_0) + (controls[1].y * weight_1) +
               (controls[2].y * weight_2) + (controls[3].y * weight_3);
    point->z = (controls[0].z * weight_0) + (controls[1].z * weight_1) +
               (controls[2].z * weight_2) + (controls[3].z * weight_3);
}

void splGetSplinePoint(Vec3 *point, HSD_Spline *spline, f32 parameter)
{
    Vec3 *controls;
    s16 segment_index;
    if (parameter < 0.0F || parameter > 1.0F) {
        return;
    }

    if (parameter < 1.0F) {
        f32 time = (parameter * (spline->control_count - 1));
        segment_index = time;
        time = time - (f32)segment_index;
        switch (spline->type) {
        case SPLINE_LINEAR:

            controls = &spline->controls[segment_index];
            point->x = (time * (controls[1].x - controls[0].x)) + controls[0].x;
            point->y = (time * (controls[1].y - controls[0].y)) + controls[0].y;
            point->z = (time * (controls[1].z - controls[0].z)) + controls[0].z;
            return;
        case SPLINE_BEZIER:
            controls = &spline->controls[segment_index * 3];
            splGetBezierPoint(point, controls, time);
            return;
        case SPLINE_BSPLINE:
            controls = &spline->controls[segment_index];
            splGetBSplinePoint(point, controls, time);
            return;
        case SPLINE_CARDINAL: {
            controls = &spline->controls[segment_index];
            splGetCardinalPoint(point, controls, spline->tension, time);
            return;
        }
        }
    } else {
        segment_index = spline->control_count - 1;
        switch (spline->type) {
        case SPLINE_LINEAR:
            *point = spline->controls[segment_index];
            return;
        case SPLINE_BEZIER:
            *point = spline->controls[segment_index * 3];
            return;
        case SPLINE_BSPLINE:
            controls = &spline->controls[segment_index] - 1;
            splGetBSplinePoint(point, controls, 1.0f);
            return;
        case SPLINE_CARDINAL:
            controls = &spline->controls[segment_index];
            *point = controls[1];
            return;
        }
    }
}

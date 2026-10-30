/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/spline.c. */
#include "spline.h"

void splArcLengthPoint(Vec3 *point, HSD_Spline *spline, f32 distance)
{
    splGetSplinePoint(point, spline, splArcLengthGetParameter(spline, distance));
}

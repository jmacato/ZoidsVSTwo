/* Source: https://github.com/doldecomp/dolsdk2001/blob/main/src/mtx/mtx.c */
#include "types.h"
/* Preserve separate rounding and the original saved-register order. */
#pragma fp_contract off
#pragma peephole off

typedef f32 Mtx[3][4];
extern void PSVECNormalize(Vec3 *source, Vec3 *destination);
extern void PSVECCrossProduct(Vec3 *first, Vec3 *second, Vec3 *destination);

void C_MTXLookAt(Mtx matrix, Vec3 *position, Vec3 *up, Vec3 *target)
{
    Vec3 look;
    Vec3 right;
    Vec3 vertical;

    look.x = position->x - target->x;
    look.y = position->y - target->y;
    look.z = position->z - target->z;
    PSVECNormalize(&look, &look);
    PSVECCrossProduct(up, &look, &right);
    PSVECNormalize(&right, &right);
    PSVECCrossProduct(&look, &right, &vertical);
    matrix[0][0] = right.x;
    matrix[0][1] = right.y;
    matrix[0][2] = right.z;
    matrix[0][3] = -((position->z * right.z) + ((position->x * right.x) + (position->y * right.y)));
    matrix[1][0] = vertical.x;
    matrix[1][1] = vertical.y;
    matrix[1][2] = vertical.z;
    matrix[1][3] = -((position->z * vertical.z) + ((position->x * vertical.x) + (position->y * vertical.y)));
    matrix[2][0] = look.x;
    matrix[2][1] = look.y;
    matrix[2][2] = look.z;
    matrix[2][3] = -((position->z * look.z) + ((position->x * look.x) + (position->y * look.y)));
}

#pragma peephole reset

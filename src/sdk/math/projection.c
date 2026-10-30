/* Source:
 * https://github.com/doldecomp/dolsdk2001/blob/main/src/mtx/mtx44.c
 * https://github.com/doldecomp/dolsdk2001/blob/main/src/mtx/mtx.c */
#include "types.h"

/* Keep the original separate rounding of multiplies and adds. */
#pragma fp_contract off

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

extern f32 tanf(f32 angle);

void C_MTXFrustum(Mtx44 matrix, f32 top, f32 bottom, f32 left, f32 right,
                  f32 near_z, f32 far_z)
{
    f32 inverse_range;
    inverse_range = 1 / (right - left);
    matrix[0][0] = (2 * near_z * inverse_range);
    matrix[0][1] = 0;
    matrix[0][2] = (inverse_range * (right + left));
    matrix[0][3] = 0;
    inverse_range = 1 / (top - bottom);
    matrix[1][0] = 0;
    matrix[1][1] = (2 * near_z * inverse_range);
    matrix[1][2] = (inverse_range * (top + bottom));
    matrix[1][3] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    inverse_range = 1 / (far_z - near_z);
    matrix[2][2] = (-near_z * inverse_range);
    matrix[2][3] = (inverse_range * -(far_z * near_z));
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = -1;
    matrix[3][3] = 0;
}

void C_MTXOrtho(Mtx44 matrix, f32 top, f32 bottom, f32 left, f32 right,
                  f32 near_z, f32 far_z)
{
    f32 inverse_range;
    inverse_range = 1 / (right - left);
    matrix[0][0] = 2 * inverse_range;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    matrix[0][3] = (inverse_range * -(right + left));
    inverse_range = 1 / (top - bottom);
    matrix[1][0] = 0;
    matrix[1][1] = 2 * inverse_range;
    matrix[1][2] = 0;
    matrix[1][3] = (inverse_range * -(top + bottom));
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    inverse_range = 1 / (far_z - near_z);
    matrix[2][2] = (-1 * inverse_range);
    matrix[2][3] = (-far_z * inverse_range);
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1;
}

/* Preserve the ninth argument's original stack load order. */
#pragma peephole off
void C_MTXLightFrustum(Mtx matrix, f32 top, f32 bottom, f32 left, f32 right, f32 near_z,
                       f32 scale_s, f32 scale_t, f32 translate_s, f32 translate_t)
{
    f32 inverse_range;

    inverse_range = 1 / (right - left);
    matrix[0][0] = (scale_s * (2 * near_z * inverse_range));
    matrix[0][1] = 0;
    matrix[0][2] = (scale_s * (inverse_range * (right + left))) - translate_s;
    matrix[0][3] = 0;
    inverse_range = 1 / (top - bottom);
    matrix[1][0] = 0;
    matrix[1][1] = (scale_t * (2 * near_z * inverse_range));
    matrix[1][2] = (scale_t * (inverse_range * (top + bottom))) - translate_t;
    matrix[1][3] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = -1;
    matrix[2][3] = 0;
}
#pragma peephole reset

void C_MTXLightOrtho(Mtx matrix, f32 top, f32 bottom, f32 left, f32 right,
                       f32 scale_s, f32 scale_t, f32 translate_s, f32 translate_t)
{
    f32 inverse_range;
    inverse_range = 1 / (right - left);
    matrix[0][0] = (2 * inverse_range * scale_s);
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    matrix[0][3] = (translate_s + (scale_s * (inverse_range * -(right + left))));
    inverse_range = 1 / (top - bottom);
    matrix[1][0] = 0;
    matrix[1][1] = (2 * inverse_range * scale_t);
    matrix[1][2] = 0;
    matrix[1][3] = (translate_t + (scale_t * (inverse_range * -(top + bottom))));
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = 0;
    matrix[2][3] = 1;
}

void C_MTXPerspective(Mtx44 matrix, f32 fov_y, f32 aspect,
                      f32 near_z, f32 far_z)
{
    f32 angle;
    f32 cotangent;
    f32 inverse_range;

    angle = (0.5f * fov_y);
    angle = angle * 0.017453293f;
    cotangent = 1 / tanf(angle);
    matrix[0][0] = (cotangent / aspect);
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    matrix[0][3] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = (cotangent);
    matrix[1][2] = 0;
    matrix[1][3] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    inverse_range = 1 / (far_z - near_z);
    matrix[2][2] = (-near_z * inverse_range);
    matrix[2][3] = (inverse_range * -(far_z * near_z));
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = -1;
    matrix[3][3] = 0;
}

/* Keep the original saved-register order around the tangent call. */
#pragma peephole off
void C_MTXLightPerspective(Mtx matrix, f32 fov_y, f32 aspect,
                           f32 scale_s, f32 scale_t, f32 translate_s, f32 translate_t)
{
    f32 angle;
    f32 cotangent;

    angle = (0.5f * fov_y);
    angle = angle * 0.017453293f;
    cotangent = 1 / tanf(angle);
    matrix[0][0] = (scale_s * (cotangent / aspect));
    matrix[0][1] = 0;
    matrix[0][2] = -translate_s;
    matrix[0][3] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = (cotangent * scale_t);
    matrix[1][2] = -translate_t;
    matrix[1][3] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = -1;
    matrix[2][3] = 0;
}

#pragma peephole reset

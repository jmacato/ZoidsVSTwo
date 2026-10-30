/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/mtx.c.
 */
#include "matrix.h"
#include "constraint_math.h"

extern f32 sinf(f32 value);
extern f32 cosf(f32 value);
extern f32 asinf(f32 value);
extern f32 atan2f(f32 y, f32 x);
#define FLOAT_MIN 1.1754943E-38f
#define M_PI 3.14159265358979323846
static inline f32 HSD_CalcDeterminantMatrix3x4(Mtx matrix)
{
    return matrix[0][0] * matrix[1][1] * matrix[2][2] +
           matrix[0][1] * matrix[1][2] * matrix[2][0] +
           matrix[0][2] * matrix[1][0] * matrix[2][1] -
           matrix[2][0] * matrix[1][1] * matrix[0][2] -
           matrix[1][0] * matrix[0][1] * matrix[2][2] -
           matrix[0][0] * matrix[2][1] * matrix[1][2];
}

s32 HSD_MtxInverseConcat(Mtx inverse, Mtx source, Mtx destination)
{
    Mtx temporary;
    f32 determinant;
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    f32 temp5;
    f32 temp6;
    f32 temp7;
    f32 temp8;
    f32 temp9;
    f32 temp10;
    f32 temp11;
    f32 temp12;

    determinant = HSD_CalcDeterminantMatrix3x4(inverse);

    if (determinant == 0.0f) {
        return 0;
    } else {
        determinant = 1.0f / determinant;
        temp1 = ((inverse[1][1] * inverse[2][2]) - (inverse[2][1] * inverse[1][2])) *
                determinant;
        temp2 = (-((inverse[0][1] * inverse[2][2]) - (inverse[2][1] * inverse[0][2]))) *
                determinant;
        temp7 = ((inverse[0][1] * inverse[1][2]) - (inverse[1][1] * inverse[0][2])) *
                determinant;
        temp3 = (-((inverse[1][0] * inverse[2][2]) - (inverse[2][0] * inverse[1][2]))) *
                determinant;
        temp4 = ((inverse[0][0] * inverse[2][2]) - (inverse[2][0] * inverse[0][2])) *
                determinant;
        temp8 = (-((inverse[0][0] * inverse[1][2]) - (inverse[1][0] * inverse[0][2]))) *
                determinant;
        temp5 = ((inverse[1][0] * inverse[2][1]) - (inverse[2][0] * inverse[1][1])) *
                determinant;
        temp6 = (-((inverse[0][0] * inverse[2][1]) - (inverse[2][0] * inverse[0][1]))) *
                determinant;
        temp9 = ((inverse[0][0] * inverse[1][1]) - (inverse[1][0] * inverse[0][1])) *
                determinant;
        temp10 = -((temp7 * inverse[2][3]) -
                   (((-temp1) * inverse[0][3]) - (temp2 * inverse[1][3])));
        temp11 = -((temp8 * inverse[2][3]) -
                   (((-temp3) * inverse[0][3]) - (temp4 * inverse[1][3])));
        temp12 = -((temp9 * inverse[2][3]) -
                   (((-temp5) * inverse[0][3]) - (temp6 * inverse[1][3])));

        if (inverse == destination || source == destination) {
            temporary[0][0] =
                temp7 * source[2][0] + (temp1 * source[0][0] + temp2 * source[1][0]);
            temporary[0][1] =
                temp7 * source[2][1] + (temp1 * source[0][1] + temp2 * source[1][1]);
            temporary[0][2] =
                temp7 * source[2][2] + (temp1 * source[0][2] + temp2 * source[1][2]);
            temporary[0][3] = temp7 * source[2][3] +
                              (temp1 * source[0][3] + temp2 * source[1][3]) + temp10;
            temporary[1][0] =
                temp8 * source[2][0] + (temp3 * source[0][0] + temp4 * source[1][0]);
            temporary[1][1] =
                temp8 * source[2][1] + (temp3 * source[0][1] + temp4 * source[1][1]);
            temporary[1][2] =
                temp8 * source[2][2] + (temp3 * source[0][2] + temp4 * source[1][2]);
            temporary[1][3] = temp8 * source[2][3] +
                              (temp3 * source[0][3] + temp4 * source[1][3]) + temp11;
            temporary[2][0] =
                temp9 * source[2][0] + (temp5 * source[0][0] + temp6 * source[1][0]);
            temporary[2][1] =
                temp9 * source[2][1] + (temp5 * source[0][1] + temp6 * source[1][1]);
            temporary[2][2] =
                temp9 * source[2][2] + (temp5 * source[0][2] + temp6 * source[1][2]);
            temporary[2][3] = temp9 * source[2][3] +
                              (temp5 * source[0][3] + temp6 * source[1][3]) + temp12;

            PSMTXCopy(temporary, destination);
        } else {
            destination[0][0] =
                temp7 * source[2][0] + (temp1 * source[0][0] + temp2 * source[1][0]);
            destination[0][1] =
                temp7 * source[2][1] + (temp1 * source[0][1] + temp2 * source[1][1]);
            destination[0][2] =
                temp7 * source[2][2] + (temp1 * source[0][2] + temp2 * source[1][2]);
            destination[0][3] = temp7 * source[2][3] +
                                (temp1 * source[0][3] + temp2 * source[1][3]) + temp10;
            destination[1][0] =
                temp8 * source[2][0] + (temp3 * source[0][0] + temp4 * source[1][0]);
            destination[1][1] =
                temp8 * source[2][1] + (temp3 * source[0][1] + temp4 * source[1][1]);
            destination[1][2] =
                temp8 * source[2][2] + (temp3 * source[0][2] + temp4 * source[1][2]);
            destination[1][3] = temp8 * source[2][3] +
                                (temp3 * source[0][3] + temp4 * source[1][3]) + temp11;
            destination[2][0] =
                temp9 * source[2][0] + (temp5 * source[0][0] + temp6 * source[1][0]);
            destination[2][1] =
                temp9 * source[2][1] + (temp5 * source[0][1] + temp6 * source[1][1]);
            destination[2][2] =
                temp9 * source[2][2] + (temp5 * source[0][2] + temp6 * source[1][2]);
            destination[2][3] = temp9 * source[2][3] +
                                (temp5 * source[0][3] + temp6 * source[1][3]) + temp12;
        }
    }
    return 1;
}

static inline f32 calcVal(f32 x, f32 y)
{
    if (x == 0.0f) {
        if (y >= 0) {
            return M_PI / 2;
        } else {
            return -M_PI / 2;
        }
    } else {
        return atan2f(y, x);
    }
}

void HSD_MtxGetRotation(Mtx matrix, Vec3 *rotation)
{
    f32 length_x;
    f32 length_y;
    f32 length_z;
    f32 sin_y;
    f32 angle_y;

    length_x =
        constraint_root(matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0] +
                        matrix[2][0] * matrix[2][0]);
    if (!(length_x < FLOAT_MIN)) {
        length_y =
            constraint_root(matrix[0][1] * matrix[0][1] + matrix[1][1] * matrix[1][1] +
                            matrix[2][1] * matrix[2][1]);
        if (!(length_y < FLOAT_MIN)) {
            length_z = constraint_root(matrix[0][2] * matrix[0][2] +
                                       matrix[1][2] * matrix[1][2] +
                                       matrix[2][2] * matrix[2][2]);
            if (!(length_z < FLOAT_MIN)) {
                sin_y = -matrix[2][0];
                sin_y /= length_x;

                if (sin_y >= 1.0f) {
                    angle_y = M_PI / 2;
                } else if (sin_y <= -1) {
                    angle_y = -M_PI / 2;
                } else {
                    angle_y = asinf(sin_y);
                }

                rotation->y = angle_y;

                if (cosf(rotation->y) >= FLOAT_MIN) {
                    f32 cos_x = matrix[2][2] / length_z;
                    f32 sin_x = matrix[2][1] / length_y;

                    rotation->x = calcVal(cos_x, sin_x);
                    rotation->z = calcVal(matrix[0][0], matrix[1][0]);
                    return;
                }

                rotation->x = calcVal(matrix[1][1], matrix[0][1]);
                rotation->z = 0;
                return;
            }
        }
    }

    rotation->x = 0;
    rotation->y = 0;
    rotation->z = 0;
}

void HSD_MtxGetTranslate(Mtx matrix, Vec3 *translation)
{
    translation->x = matrix[0][3];
    translation->y = matrix[1][3];
    translation->z = matrix[2][3];
}

void HSD_MtxGetScale(Mtx matrix, Vec3 *scale)
{
    f64 sign;
    f64 factor;

    Vec3 basis_x;
    Vec3 basis_y;
    Vec3 basis_z;
    Vec3 projection;

    basis_x.x = matrix[0][0];
    basis_x.y = matrix[1][0];
    basis_x.z = matrix[2][0];

    scale->x = PSVECMag(&basis_x);
    PSVECNormalize(&basis_x, &basis_x);

    basis_y.x = matrix[0][1];
    basis_y.y = matrix[1][1];
    basis_y.z = matrix[2][1];

    PSVECScale(&basis_x, &projection, PSVECDotProduct(&basis_x, &basis_y));
    PSVECSubtract(&basis_y, &projection, &basis_y);
    scale->y = PSVECMag(&basis_y);
    PSVECNormalize(&basis_y, &basis_y);

    basis_z.x = matrix[0][2];
    basis_z.y = matrix[1][2];
    basis_z.z = matrix[2][2];

    PSVECScale(&basis_y, &projection, PSVECDotProduct(&basis_y, &basis_z));
    PSVECSubtract(&basis_z, &projection, &basis_z);
    PSVECScale(&basis_x, &projection, PSVECDotProduct(&basis_x, &basis_z));
    PSVECSubtract(&basis_z, &projection, &basis_z);
    scale->z = PSVECMag(&basis_z);
    PSVECNormalize(&basis_z, &basis_z);
    PSVECCrossProduct(&basis_y, &basis_z, &projection);

    if (PSVECDotProduct(&basis_x, &projection) < 0.0) {
        sign = -1.0;
        factor = sign;
        scale->x *= factor;
        scale->y *= factor;
        scale->z *= factor;
    }
}

void HSD_MkRotationMtx(Mtx matrix, Vec3 *rotation)
{
    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;
    f32 temp1;
    f32 temp2;

    sinX = sinf(rotation->x);
    cosX = cosf(rotation->x);
    sinY = sinf(rotation->y);
    cosY = cosf(rotation->y);
    sinZ = sinf(rotation->z);
    cosZ = cosf(rotation->z);

    temp1 = sinX * sinY;
    matrix[0][0] = cosY * cosZ;
    matrix[1][0] = cosY * sinZ;
    matrix[2][0] = -sinY;
    temp2 = cosX * sinY;
    matrix[0][1] = (cosZ * temp1) - (cosX * sinZ);
    matrix[1][1] = (sinZ * temp1) + (cosX * cosZ);
    matrix[2][1] = sinX * cosY;
    matrix[0][2] = (cosZ * temp2) + (sinX * sinZ);
    matrix[1][2] = (sinZ * temp2) - (sinX * cosZ);
    matrix[2][2] = cosX * cosY;
    matrix[0][3] = 0;
    matrix[1][3] = 0;
    matrix[2][3] = 0;
}

void HSD_MtxSRT(Mtx matrix, Vec3 *scale, Vec3 *rotation, Vec3 *translation,
                Vec3 *parent_scale)
{
    f32 vec1x_2;
    f32 vec1y_2;
    f32 vec1z_2;
    f32 vec1x_1;
    f32 vec1y_1;
    f32 vec1z_1;
    f32 vec1x;
    f32 vec1y;
    f32 vec1z;

    f32 sinX = sinf(rotation->x);
    f32 cosX = cosf(rotation->x);
    f32 sinY = sinf(rotation->y);
    f32 cosY = cosf(rotation->y);
    f32 sinZ = sinf(rotation->z);
    f32 cosZ = cosf(rotation->z);

    vec1x_2 = vec1x_1 = vec1x = scale->x;
    vec1y_2 = vec1y_1 = vec1y = scale->y;
    vec1z_2 = vec1z_1 = vec1z = scale->z;

    if (parent_scale != 0) {
        f32 temp1 = 1.0 / parent_scale->x;
        f32 temp2 = 1.0 / parent_scale->y;
        f32 temp3 = 1.0 / parent_scale->z;

        vec1y_2 *= parent_scale->y * temp1;
        vec1z_2 *= parent_scale->z * temp1;
        vec1x_1 *= parent_scale->x * temp2;
        vec1z_1 *= parent_scale->z * temp2;
        vec1x *= parent_scale->x * temp3;
        vec1y *= parent_scale->y * temp3;
    }

    matrix[0][0] = cosZ * (vec1x_2 * cosY);
    matrix[1][0] = sinZ * (vec1x_1 * cosY);
    matrix[2][0] = -vec1x * sinY;
    matrix[0][1] = vec1y_2 * ((cosZ * (sinX * sinY)) - (cosX * sinZ));
    matrix[1][1] = vec1y_1 * ((sinZ * (sinX * sinY)) + (cosX * cosZ));
    matrix[2][1] = cosY * (vec1y * sinX);
    matrix[0][2] = vec1z_2 * ((cosZ * (cosX * sinY)) + (sinX * sinZ));
    matrix[1][2] = vec1z_1 * ((sinZ * (cosX * sinY)) - (sinX * cosZ));
    matrix[2][2] = cosY * (vec1z * cosX);
    matrix[0][3] = translation->x;
    matrix[1][3] = translation->y;
    matrix[2][3] = translation->z;
}

void HSD_MtxSRTQuat(Mtx matrix, Vec3 *scale, Quaternion *rotation, Vec3 *translation,
                    Vec3 *parent_scale)
{
    Mtx temporary;

    PSMTXScale(matrix, scale->x, scale->y, scale->z);

    if (parent_scale != 0) {
        PSMTXScale(temporary, parent_scale->x, parent_scale->y, parent_scale->z);
        PSMTXConcat(temporary, matrix, matrix);
    }

    PSMTXQuat(temporary, rotation);
    PSMTXConcat(temporary, matrix, matrix);

    if (parent_scale != 0) {
        PSMTXScale(temporary, 1.0 / parent_scale->x, 1.0 / parent_scale->y,
                   1.0 / parent_scale->z);
        PSMTXConcat(temporary, matrix, matrix);
    }

    PSMTXTrans(temporary, translation->x, translation->y, translation->z);
    PSMTXConcat(temporary, matrix, matrix);
}

void HSD_MtxScaledAdd(Mtx first, Mtx second, Mtx result, f32 factor)
{
    f32 *first_element = (&first[0][0]);
    f32 *second_element = (&second[0][0]);
    f32 *result_element = (&result[0][0]);

    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);

    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);

    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
    *result_element++ = *second_element++ + (factor * *first_element++);
}

extern HSD_ObjAllocData vec_alloc_data;
extern HSD_ObjAllocData mtx_alloc_data;
extern void HSD_ObjAllocInit(HSD_ObjAllocData *data, u32 size, u32 alignment);
extern void *HSD_ObjAlloc(HSD_ObjAllocData *data);
extern void HSD_ObjFree(HSD_ObjAllocData *data, void *object);
Vec3 *HSD_VecAlloc(void)
{
    Vec3 *vector = HSD_ObjAlloc(&vec_alloc_data);
    if (vector == 0)
        __assert("mtx.c", 823, "vec");
    return vector;
}
void HSD_VecFree(Vec3 *vector)
{
    if (vector != 0)
        HSD_ObjFree(&vec_alloc_data, vector);
}
Mtx *HSD_MtxAlloc(void)
{
    Mtx *matrix = HSD_ObjAlloc(&mtx_alloc_data);
    if (matrix == 0)
        __assert("mtx.c", 854, "mtx");
    return matrix;
}
void HSD_MtxFree(Mtx *matrix)
{
    if (matrix != 0)
        HSD_ObjFree(&mtx_alloc_data, matrix);
}

void HSD_VecInitAllocData(void)
{
    HSD_ObjAllocInit(&vec_alloc_data, sizeof(Vec3), 4);
}

void HSD_MtxInitAllocData(void)
{
    HSD_ObjAllocInit(&mtx_alloc_data, sizeof(Mtx), 4);
}

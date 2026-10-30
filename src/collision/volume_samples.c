#include "matrix.h"

extern void PSMTXMultVecArray(Mtx matrix, Vec3 *source, Vec3 *destination, u32 count);
extern s32 sColCube4Point(Vec3 *point);
extern s32 sColSphere4Point(Vec3 *point);

/* unit cube: 8 corners and the center, then edge midpoints and 6 face centers */
Vec3 CubeTBL[27] = {
    { -0.5F, -0.5F, -0.5F },
    { -0.5F, 0.5F, -0.5F },
    { 0.5F, -0.5F, -0.5F },
    { 0.5F, 0.5F, -0.5F },
    { -0.5F, -0.5F, 0.5F },
    { -0.5F, 0.5F, 0.5F },
    { 0.5F, -0.5F, 0.5F },
    { 0.5F, 0.5F, 0.5F },
    { 0.0F, 0.0F, 0.0F },
    { -0.5F, -0.5F, 0.0F },
    { 0.0F, -0.5F, -0.5F },
    { -0.5F, 0.0F, -0.5F },
    { 0.5F, -0.5F, 0.0F },
    { 0.0F, 0.5F, -0.5F },
    { 0.5F, 0.0F, -0.5F },
    { 0.5F, 0.0F, 0.5F },
    { -0.5F, 0.0F, 0.5F },
    { -0.5F, 0.5F, 0.0F },
    { 0.5F, 0.5F, 0.0F },
    /* The original table repeats sample 13 here; edge midpoint (0, -0.5, 0.5) is never sampled. */
    { 0.0F, 0.5F, -0.5F },
    { 0.0F, 0.5F, 0.5F },
    { 0.5F, 0.0F, 0.0F },
    { -0.5F, 0.0F, 0.0F },
    { 0.0F, 0.5F, 0.0F },
    { 0.0F, -0.5F, 0.0F },
    { 0.0F, 0.0F, 0.5F },
    { 0.0F, 0.0F, -0.5F },
};

s32 sChkCubeInCube(Mtx transform, Mtx inverse)
{
    Vec3 world[9];
    Vec3 local[9];
    s32 i;

    PSMTXMultVecArray(transform, CubeTBL, world, 9);
    PSMTXMultVecArray(inverse, world, local, 9);
    for (i = 0; i < 9; i++) {
        if (sColCube4Point(&local[i]))
            return 1;
    }
    return 0;
}

s32 sChkCubeInSphere(Mtx transform, Mtx inverse)
{
    Vec3 world[27];
    Vec3 local[27];
    s32 i;

    PSMTXMultVecArray(transform, CubeTBL, world, 27);
    PSMTXMultVecArray(inverse, world, local, 27);
    for (i = 0; i < 27; i++) {
        if (sColSphere4Point(&local[i]))
            return 1;
    }
    return 0;
}

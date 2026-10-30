#include "ground_collision.h"

extern void sColPosGetXZ(s32 *column, s32 *row, f32 x, f32 z);

s32 sColPos(Vec3 *point)
{
    s32 column;
    s32 row;
    register f32 x = point->x;
    register f32 z = point->z;

    if (x < grdcStartX) {
        column = 0;
    } else if (grdcEndX < x) {
        column = grdcDivX - 1;
    } else {
        column = (s32)((x - grdcStartX) / grdcDiffX);
    }
    if (z < grdcStartZ) {
        row = 0;
    } else if (grdcEndZ < z) {
        row = grdcDivZ - 1;
    } else {
        row = (s32)((z - grdcStartZ) / grdcDiffZ);
    }
    if (column >= grdcDivX) {
        column = grdcDivX - 1;
    }
    if (row >= grdcDivZ) {
        row = grdcDivZ - 1;
    }
    return column + row * grdcDivX;
}

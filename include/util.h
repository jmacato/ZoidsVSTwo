#ifndef ZOIDS_UTIL_H
#define ZOIDS_UTIL_H
#include "matrix.h"
#include "gx_tev.h"
extern Mtx HSD_identityMtx;
extern void HSD_MulColor(GXColor *first, GXColor *second, GXColor *result);
extern u32 HSD_GetNbBits(u32 value);
extern s32 HSD_Index2PosNrmMtx(u32 index);
#endif

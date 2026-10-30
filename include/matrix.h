#ifndef ZOIDS_MATRIX_H
#define ZOIDS_MATRIX_H

#include "objalloc.h"

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];

typedef struct Quaternion {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Quaternion;

extern s32 HSD_MtxInverseConcat(Mtx inverse, Mtx source, Mtx destination);
extern void HSD_MtxGetRotation(Mtx matrix, Vec3 *rotation);
extern void HSD_MtxGetTranslate(Mtx matrix, Vec3 *translation);
extern void HSD_MtxGetScale(Mtx matrix, Vec3 *scale);
extern void HSD_MkRotationMtx(Mtx matrix, Vec3 *rotation);
extern void HSD_MtxSRT(Mtx matrix, Vec3 *scale, Vec3 *rotation, Vec3 *translation,
                       Vec3 *parent_scale);
extern void HSD_MtxSRTQuat(Mtx matrix, Vec3 *scale, Quaternion *rotation,
                           Vec3 *translation, Vec3 *parent_scale);
extern void HSD_MtxScaledAdd(Mtx first, Mtx second, Mtx result, f32 factor);
extern Vec3 *HSD_VecAlloc(void);
extern void HSD_VecFree(Vec3 *vector);
extern Mtx *HSD_MtxAlloc(void);
extern void HSD_MtxFree(Mtx *matrix);
extern HSD_ObjAllocData *HSD_VecGetAllocData(void);
extern void HSD_VecInitAllocData(void);
extern HSD_ObjAllocData *HSD_MtxGetAllocData(void);
extern void HSD_MtxInitAllocData(void);

extern void PSMTXCopy(Mtx source, Mtx destination);
extern void PSMTXIdentity(Mtx matrix);
extern void PSMTXScale(Mtx matrix, f32 x, f32 y, f32 z);
extern void PSMTXTrans(Mtx matrix, f32 x, f32 y, f32 z);
extern void PSMTXConcat(Mtx first, Mtx second, Mtx result);
extern void PSMTXQuat(Mtx matrix, Quaternion *quaternion);
extern f32 PSVECMag(Vec3 *vector);
extern void PSVECNormalize(Vec3 *source, Vec3 *destination);

#endif

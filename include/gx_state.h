/* GX register and FIFO references:
 * https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/__gx.h
 * and https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/gx/GXVert.h.
 */
#ifndef ZOIDS_GX_STATE_H
#define ZOIDS_GX_STATE_H
#include "types.h"
#include "gx_texture_types.h"
typedef struct GXData {
    u16 vcdClear;
    u16 bpSent;
    u16 vNum;
    u16 vLim;
    u32 cpEnable;
    u32 cpStatus;
    u32 cpClr;
    u32 vcdLo;
    u32 vcdHi;
    unsigned long vatA[8];
    unsigned long vatB[8];
    u32 vatC[8];
    u32 lpSize;
    u32 matIdxA;
    u32 matIdxB;
    u32 indexBase[4];
    u32 indexStride[4];
    u32 ambColor[2];
    u32 matColor[2];
    u32 suTs0[8];
    u32 suTs1[8];
    u32 suScis0;
    u32 suScis1;
    u32 tref[8];
    u32 iref;
    u32 bpMask;
    u32 IndTexScale0;
    u32 IndTexScale1;
    u32 tevc[16];
    u32 teva[16];
    u32 tevKsel[8];
    u32 cmode0;
    u32 cmode1;
    u32 zmode;
    u32 peCtrl;
    u32 cpDispSrc;
    u32 cpDispSize;
    u32 cpDispStride;
    u32 cpDisp;
    u32 cpTexSrc;
    u32 cpTexSize;
    u32 cpTexStride;
    u32 cpTex;
    u8 cpTexZ;
    u8 reserved_201[3];
    u32 genMode;
    GXTexRegion TexRegions[8];
    GXTexRegion TexRegionsCI[4];
    u32 nextTexRgn;
    u32 nextTexRgnCI;
    GXTlutRegion TlutRegions[20];
    GXTexRegionCallback texRegionCallback;
    GXTlutRegionCallback tlutRegionCallback;
    s32 nrmType;
    u8 hasNrms;
    u8 hasBiNrms;
    u8 reserved_41E[2];
    u32 projType;
    f32 projMtx[6];
    f32 vpLeft;
    f32 vpTop;
    f32 vpWd;
    f32 vpHt;
    f32 vpNearz;
    f32 vpFarz;
    u8 fgRange;
    u8 reserved_455[3];
    f32 fgSideX;
    u32 tImage0[8];
    u32 tMode0[8];
    u32 texmapId[16];
    u32 tcsManEnab;
    u32 tevTcEnab;
    s32 perf0;
    s32 perf1;
    u32 perfSel;
    u8 inDispList;
    u8 dlSaveContext;
    u8 dirtyVAT;
    u8 reserved_4F3;
    u32 dirtyState;
} GXData;
extern GXData *gx;
typedef union GXFifo {
    u8 u8;
    u16 u16;
    u32 u32;
    u64 u64;
    s8 s8;
    s16 s16;
    s32 s32;
    s64 s64;
    f32 f32;
    f64 f64;
} GXFifo;
#ifdef __MWERKS__
extern volatile GXFifo GXWGFifo : 0xCC008000;
#else
#define GXWGFifo (*(volatile GXFifo *)0xCC008000)
#endif
#define GX_SET_FIELD(reg, width, shift, value)                                         \
    do {                                                                               \
        (reg) = ((u32)(reg) & ~(((1U << (width)) - 1) << (shift))) |                   \
                ((u32)(value) << (shift));                                             \
    } while (0)
#define GX_WRITE_U8(value) (GXWGFifo.u8 = (u8)(value))
#define GX_WRITE_U16(value) (GXWGFifo.u16 = (u16)(value))
#define GX_WRITE_U32(value) (GXWGFifo.u32 = (u32)(value))
#define GX_WRITE_F32(value) (GXWGFifo.f32 = (f32)(value))
#define GX_WRITE_RAS_REG(value)                                                        \
    do {                                                                               \
        GX_WRITE_U8(0x61);                                                             \
        GX_WRITE_U32(value);                                                           \
    } while (0)
#define GX_WRITE_XF_REG(addr, value)                                                   \
    do {                                                                               \
        GX_WRITE_U8(0x10);                                                             \
        GX_WRITE_U32(0x1000 + (addr));                                                 \
        GX_WRITE_U32(value);                                                           \
    } while (0)
extern void __GXSetSUTexRegs(void);
extern void __GXUpdateBPMask(void);
extern void __GXSetGenMode(void);
extern void __GXSetVCD(void);
extern void __GXSetVAT(void);
extern void __GXCalculateVLim(void);
extern void __GXSetDirtyState(void);
extern void __GXSendFlushPrim(void);
extern void __GXSetMatrixIndex(s32 attribute);
#define GX_GET_FIELD(reg, width, shift) ((int)((reg) >> (shift)) & ((1 << (width)) - 1))
#define GX_WRITE_CP_REG(reg, value)                                                    \
    do {                                                                               \
        GX_WRITE_U8(8);                                                                \
        GX_WRITE_U8(reg);                                                              \
        GX_WRITE_U32(value);                                                           \
    } while (0)
#define GX_WRITE_ARRAY_BASE(reg, value, index)                                         \
    do {                                                                               \
        long slot;                                                                     \
        GX_WRITE_CP_REG(reg, value);                                                   \
        slot = (index);                                                                \
        if (slot >= 0 && slot < 4)                                                     \
            gx->indexBase[slot] = (value);                                             \
    } while (0)
#define GX_WRITE_ARRAY_STRIDE(reg, value, index)                                       \
    do {                                                                               \
        long slot;                                                                     \
        GX_WRITE_CP_REG(reg, value);                                                   \
        slot = (index);                                                                \
        if (slot >= 0 && slot < 4)                                                     \
            gx->indexStride[slot] = (value);                                           \
    } while (0)
#endif

#ifndef ZOIDS_TOBJ_H
#define ZOIDS_TOBJ_H
/* Layouts follow
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/tobj.h. */
#include "texp.h"
#include "aobj.h"
#include "object.h"
#include "gx_texture_types.h"
typedef s32 GXTexGenSrc;
typedef s32 GXTexMtx;
#define TOBJ_ANIM 0x10

#define HSD_A_T_TIMG 1
#define HSD_A_T_TRAU 2
#define HSD_A_T_TRAV 3
#define HSD_A_T_SCAU 4
#define HSD_A_T_SCAV 5
#define HSD_A_T_ROTX 6
#define HSD_A_T_ROTY 7
#define HSD_A_T_ROTZ 8
#define HSD_A_T_BLEND 9
#define HSD_A_T_TCLT 10
#define HSD_A_T_LOD_BIAS 11
#define HSD_A_T_KONST_R 12
#define HSD_A_T_KONST_G 13
#define HSD_A_T_KONST_B 14
#define HSD_A_T_KONST_A 15
#define HSD_A_T_TEV0_R 16
#define HSD_A_T_TEV0_G 17
#define HSD_A_T_TEV0_B 18
#define HSD_A_T_TEV0_A 19
#define HSD_A_T_TEV1_R 20
#define HSD_A_T_TEV1_G 21
#define HSD_A_T_TEV1_B 22
#define HSD_A_T_TEV1_A 23
#define HSD_A_T_TS_BLEND 24

#define TOBJ_TEV_CC_KONST_RGB (0x01 << 7 | 0)
#define TOBJ_TEV_CC_KONST_RRR (0x01 << 7 | 1)
#define TOBJ_TEV_CC_KONST_GGG (0x01 << 7 | 2)
#define TOBJ_TEV_CC_KONST_BBB (0x01 << 7 | 3)
#define TOBJ_TEV_CC_KONST_AAA (0x01 << 7 | 4)
#define TOBJ_TEV_CC_TEX0_RGB (0x01 << 7 | 5)
#define TOBJ_TEV_CC_TEX0_AAA (0x01 << 7 | 6)
#define TOBJ_TEV_CC_TEX1_RGB (0x01 << 7 | 7)
#define TOBJ_TEV_CC_TEX1_AAA (0x01 << 7 | 8)

#define TOBJ_TEV_CA_KONST_R (0x01 << 6 | 0)
#define TOBJ_TEV_CA_KONST_G (0x01 << 6 | 1)
#define TOBJ_TEV_CA_KONST_B (0x01 << 6 | 2)
#define TOBJ_TEV_CA_KONST_A (0x01 << 6 | 3)
#define TOBJ_TEV_CA_TEX0_A (0x01 << 6 | 4)
#define TOBJ_TEV_CA_TEX1_A (0x01 << 6 | 5)

#define TOBJ_TEVREG_ACTIVE_KONST_R (0x01 << 0)
#define TOBJ_TEVREG_ACTIVE_KONST_G (0x01 << 1)
#define TOBJ_TEVREG_ACTIVE_KONST_B (0x01 << 2)
#define TOBJ_TEVREG_ACTIVE_KONST_A (0x01 << 3)
#define TOBJ_TEVREG_ACTIVE_KONST                                                       \
    (TOBJ_TEVREG_ACTIVE_KONST_R | TOBJ_TEVREG_ACTIVE_KONST_G |                         \
     TOBJ_TEVREG_ACTIVE_KONST_B | TOBJ_TEVREG_ACTIVE_KONST_A)
#define TOBJ_TEVREG_ACTIVE_TEV0_R (0x01 << 4)
#define TOBJ_TEVREG_ACTIVE_TEV0_G (0x01 << 5)
#define TOBJ_TEVREG_ACTIVE_TEV0_B (0x01 << 6)
#define TOBJ_TEVREG_ACTIVE_TEV0_A (0x01 << 7)
#define TOBJ_TEVREG_ACTIVE_TEV0                                                        \
    (TOBJ_TEVREG_ACTIVE_TEV0_R | TOBJ_TEVREG_ACTIVE_TEV0_G |                           \
     TOBJ_TEVREG_ACTIVE_TEV0_B | TOBJ_TEVREG_ACTIVE_TEV0_A)
#define TOBJ_TEVREG_ACTIVE_TEV1_R (0x01 << 8)
#define TOBJ_TEVREG_ACTIVE_TEV1_G (0x01 << 9)
#define TOBJ_TEVREG_ACTIVE_TEV1_B (0x01 << 10)
#define TOBJ_TEVREG_ACTIVE_TEV1_A (0x01 << 11)
#define TOBJ_TEVREG_ACTIVE_TEV1                                                        \
    (TOBJ_TEVREG_ACTIVE_TEV1_R | TOBJ_TEVREG_ACTIVE_TEV1_G |                           \
     TOBJ_TEVREG_ACTIVE_TEV1_B | TOBJ_TEVREG_ACTIVE_TEV1_A)
#define TOBJ_TEVREG_ACTIVE_COLOR_TEV (0x01 << 30)
#define TOBJ_TEVREG_ACTIVE_ALPHA_TEV (0x01U << 31)

#define TEX_COORD_UV 0
#define TEX_COORD_REFLECTION 1
#define TEX_COORD_HILIGHT 2
#define TEX_COORD_SHADOW 3
#define TEX_COORD_TOON 4
#define TEX_COORD_GRADATION 5
#define TEX_COORD_BACKLIGHT 6
#define TEX_COORD_MASK (0x0f)
#define tobj_coord(T) ((T)->flags & TEX_COORD_MASK)

#define TEX_COLORMAP_NONE (0 << 16)
#define TEX_COLORMAP_ALPHA_MASK (1 << 16)
#define TEX_COLORMAP_RGB_MASK (2 << 16)
#define TEX_COLORMAP_BLEND (3 << 16)
#define TEX_COLORMAP_MODULATE (4 << 16)
#define TEX_COLORMAP_REPLACE (5 << 16)
#define TEX_COLORMAP_PASS (6 << 16)
#define TEX_COLORMAP_ADD (7 << 16)
#define TEX_COLORMAP_SUB (8 << 16)
#define TEX_COLORMAP_MASK (0x0f << 16)
#define tobj_colormap(T) ((T)->flags & TEX_COLORMAP_MASK)

#define TEX_ALPHAMAP_NONE (0 << 20)
#define TEX_ALPHAMAP_ALPHA_MASK (1 << 20)
#define TEX_ALPHAMAP_BLEND (2 << 20)
#define TEX_ALPHAMAP_MODULATE (3 << 20)
#define TEX_ALPHAMAP_REPLACE (4 << 20)
#define TEX_ALPHAMAP_PASS (5 << 20)
#define TEX_ALPHAMAP_ADD (6 << 20)
#define TEX_ALPHAMAP_SUB (7 << 20)
#define TEX_ALPHAMAP_MASK (0x0f << 20)
#define tobj_alphamap(T) ((T)->flags & TEX_ALPHAMAP_MASK)

#define TEX_LIGHTMAP_DIFFUSE (0x1 << 4)
#define TEX_LIGHTMAP_SPECULAR (0x1 << 5)
#define TEX_LIGHTMAP_AMBIENT (0x1 << 6)
#define TEX_LIGHTMAP_EXT (0x1 << 7)
#define TEX_LIGHTMAP_SHADOW (0x1 << 8)
#define TEX_LIGHTMAP_MASK                                                              \
    (TEX_LIGHTMAP_DIFFUSE | TEX_LIGHTMAP_SPECULAR | TEX_LIGHTMAP_AMBIENT |             \
     TEX_LIGHTMAP_EXT | TEX_LIGHTMAP_SHADOW)
#define tobj_lightmap(T) ((T)->flags & TEX_LIGHTMAP_MASK)

#define TEX_BUMP (0x1 << 24)
#define tobj_bump(T) ((T)->flags & TEX_BUMP)
#define TEX_MTX_DIRTY (1U << 31)

typedef struct HSD_ImageDesc {
    void *image_ptr;
    u16 width, height;
    u32 format;
    u32 mipmap;
    f32 minLOD, maxLOD;
} HSD_ImageDesc;
typedef struct HSD_Tlut {
    void *lut;
    s32 fmt;
    u32 tlut_name;
    u16 n_entries;
} HSD_Tlut;
typedef struct HSD_TlutDesc {
    u16 *lut;
    s32 fmt;
    u32 tlut_name;
    u16 n_entries;
} HSD_TlutDesc;
typedef struct HSD_TexLODDesc {
    s32 minFilt;
    f32 LODBias;
    u8 bias_clamp;
    u8 edgeLODEnable;
    s32 max_anisotropy;
} HSD_TexLODDesc;
typedef struct HSD_TObjTev {
    u8 color_op, alpha_op, color_bias, alpha_bias, color_scale, alpha_scale,
        color_clamp, alpha_clamp;
    u8 color_a, color_b, color_c, color_d;
    u8 alpha_a, alpha_b, alpha_c, alpha_d;
    GXColor konst, tev0, tev1;
    u32 active;
} HSD_TObjTev;
typedef HSD_TObjTev HSD_TObjTevDesc;
typedef struct HSD_TObjDesc {
    char *class_name;
    struct HSD_TObjDesc *next;
    GXTexMapID id;
    s32 src;
    Vec3 rotate, scale, translate;
    s32 wrap_s, wrap_t;
    u8 repeat_s, repeat_t;
    u32 blend_flags;
    f32 blending;
    s32 magFilt;
    HSD_ImageDesc *imagedesc;
    HSD_TlutDesc *tlutdesc;
    HSD_TexLODDesc *lod;
    HSD_TObjTevDesc *tev;
} HSD_TObjDesc;
typedef struct HSD_TObjInfo {
    HSD_ClassInfo parent;
    void (*make_mtx)(HSD_TObj *);
    s32 (*load)(HSD_TObj *, HSD_TObjDesc *);
    void (*make_texp)(HSD_TObj *, u32, u32, HSD_TExp **, HSD_TExp **, HSD_TExp **);
    AnimationUpdate update;
} HSD_TObjInfo;
typedef struct HSD_TexAnim {
    struct HSD_TexAnim *next;
    GXTexMapID id;
    HSD_AObjDesc *aobjdesc;
    HSD_ImageDesc **imagetbl;
    HSD_TlutDesc **tluttbl;
    u16 n_imagetbl, n_tluttbl;
} HSD_TexAnim;
extern HSD_TObjInfo hsdTObj;
extern HSD_TObj *HSD_TObjAlloc(void);
extern HSD_TObj *HSD_TObjLoadDesc(HSD_TObjDesc *);
extern HSD_Tlut *HSD_TlutAlloc(void);
extern HSD_Tlut *HSD_TlutLoadDesc(HSD_TlutDesc *);
extern HSD_TObjTev *HSD_TObjTevAlloc(void);
extern HSD_TObjTev *HSD_TObjTevLoadDesc(HSD_TObjTevDesc *);
extern void HSD_TlutRemove(HSD_Tlut *);
extern void HSD_TObjRemoveAnimAll(HSD_TObj *texture);
extern void HSD_TObjAddAnim(HSD_TObj *texture, HSD_TexAnim *animation);
extern void HSD_TObjAddAnimAll(HSD_TObj *texture, HSD_TexAnim *animation);
extern void HSD_TObjReqAnimAllByFlags(HSD_TObj *texture, f32 frame, u32 flags);
extern void HSD_TObjAnimAll(HSD_TObj *texture);
extern HSD_TObj *_HSD_TObjGetCurrentByType(HSD_TObj *from, u32 mapping);
extern void HSD_TObjSetupTextureCoordGen(HSD_TObj *texture);
extern void HSD_TObjSetupVolatileTev(HSD_TObj *texture, u32 render_mode);
extern s32 HSD_TObjAssignResources(HSD_TObj *texture);
extern void HSD_TObjSetup(HSD_TObj *texture);
extern void HSD_TObjRemoveAll(HSD_TObj *texture);
extern void HSD_TObjFree(HSD_TObj *texture);
extern HSD_ImageDesc *HSD_ImageDescAlloc(void);
extern void HSD_ImageDescFree(HSD_ImageDesc *image);
extern u32 HSD_TGTex2Index(GXTexGenSrc source);
extern GXTexGenSrc HSD_TexCoordID2TexGenSrc(GXTexCoordID coordinate);
extern u32 HSD_TexCoord2Index(GXTexCoordID coordinate);
extern GXTexCoordID HSD_Index2TexCoord(u32 index);
extern u32 HSD_TexMtx2Index(GXTexMtx matrix);
extern GXTexMtx HSD_Index2TexMtx(u32 index);
extern GXTexMapID HSD_Index2TexMap(u32 index);
extern u32 HSD_TexMap2Index(GXTexMapID map);
#endif

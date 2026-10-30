#ifndef ZOIDS_STATE_H
#define ZOIDS_STATE_H
/* Layout follows
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/state.h. */
#include "mobj.h"
#include "objalloc.h"
typedef struct HSD_Chan {
    struct HSD_Chan *next;
    GXChannelID chan;
    u32 flags;
    GXColor amb_color;
    GXColor mat_color;
    u8 enable;
    s32 amb_src;
    s32 mat_src;
    s32 light_mask;
    s32 diff_fn;
    s32 attn_fn;
    HSD_AObj *aobj;
} HSD_Chan;
extern HSD_ObjAllocData *HSD_RenderGetAllocData(void);
extern HSD_ObjAllocData *HSD_TevRegGetAllocData(void);
extern HSD_ObjAllocData *HSD_ChanGetAllocData(void);
extern void HSD_SetupChannel(HSD_Chan *);
extern void HSD_StateSetNumChans(s32);
extern s32 HSD_Channel2Num(s32);
extern s32 HSD_TevStage2Index(s32);
extern s32 HSD_TevStage2Num(s32);
extern void HSD_StateSetNumTevStages(void);
extern void HSD_StateSetNumTexGens(void);
extern s32 HSD_StateGetNumTevStages(void);
extern s32 HSD_TexCoordID2Num(s32);
extern void HSD_ObjAllocInit(HSD_ObjAllocData *, u32, u32);
extern void HSD_SetupChannelMode(u32 flags);
extern void HSD_SetupPEMode(u32 flags, HSD_PEDesc *description);
extern void HSD_SetupRenderModeWithCustomPE(u32 flags, HSD_PEDesc *description);
extern void HSD_SetMaterialColor(GXColor ambient, GXColor diffuse, GXColor specular,
                                 f32 alpha);
extern void HSD_SetMaterialShininess(f32 shininess);
extern void HSD_StateSetCullMode(s32 mode);
extern void HSD_StateSetBlendMode(s32 type, s32 source, s32 destination, s32 operation);
extern void HSD_StateSetZMode(s32 enabled, s32 function, s32 update);
extern void HSD_StateSetAlphaCompare(s32 function_0, u8 reference_0, s32 operation,
                                     s32 function_1, u8 reference_1);
extern void HSD_StateSetColorUpdate(s32 enabled);
extern void HSD_StateSetAlphaUpdate(s32 enabled);
extern void HSD_StateSetDstAlpha(s32 enabled, u8 alpha);
extern void HSD_StateSetZCompLoc(s32 before_texture);
extern void HSD_StateSetDither(s32 enabled);
extern void HSD_RenderInitAllocData(void);
extern void HSD_DisableChannelLighting(GXChannelID channel);
extern void HSD_SetupChannelAll(HSD_Chan *channel);
extern void HSD_StateRegisterTexGen(s32 coordinate);
extern void HSD_StateInitTev(void);
extern void HSD_SetupTevStageAll(HSD_TevDesc *description);
extern void HSD_SetTevRegAll(void);
extern void ChanUpdateFunc(HSD_Chan *channel, s32 type, const f32 *value);
#endif

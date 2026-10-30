#ifndef ZOIDS_MOBJ_H
#define ZOIDS_MOBJ_H
/* Layouts follow https://github.com/doldecomp/melee/tree/master/src/sysdolphin/baselib.
 */
#include "class.h"
#include "aobj.h"
#include "texp.h"
#define HSD_A_M_AMBIENT_R 1
#define HSD_A_M_AMBIENT_G 2
#define HSD_A_M_AMBIENT_B 3
#define HSD_A_M_DIFFUSE_R 4
#define HSD_A_M_DIFFUSE_G 5
#define HSD_A_M_DIFFUSE_B 6
#define HSD_A_M_SPECULAR_R 7
#define HSD_A_M_SPECULAR_G 8
#define HSD_A_M_SPECULAR_B 9
#define HSD_A_M_ALPHA 10
#define HSD_A_M_PE_REF0 11
#define HSD_A_M_PE_REF1 12
#define HSD_A_M_PE_DSTALPHA 13
typedef struct HSD_Material {
    GXColor ambient;
    GXColor diffuse;
    GXColor specular;
    f32 alpha;
    f32 shininess;
} HSD_Material;
typedef struct HSD_PEDesc {
    u8 flags;
    u8 ref0;
    u8 ref1;
    u8 dst_alpha;
    u8 type;
    u8 src_factor;
    u8 dst_factor;
    u8 logic_op;
    u8 z_comp;
    u8 alpha_comp0;
    u8 alpha_op;
    u8 alpha_comp1;
} HSD_PEDesc;
typedef struct HSD_MObj {
    HSD_Class parent;
    u32 rendermode;
    HSD_TObj *tobj;
    HSD_Material *mat;
    HSD_PEDesc *pe;
    HSD_AObj *aobj;
    HSD_TExpTevDesc *tevdesc;
    HSD_TExp *texp;
} HSD_MObj;
typedef struct HSD_MObjDesc {
    char *class_name;
    u32 rendermode;
    void *texdesc;
    HSD_Material *mat;
    void *renderdesc;
    HSD_PEDesc *pedesc;
} HSD_MObjDesc;
typedef struct HSD_MObjInfo {
    HSD_ClassInfo parent;
    void (*setup)(HSD_MObj *, u32);
    s32 (*load)(HSD_MObj *, HSD_MObjDesc *);
    HSD_TExp *(*make_texp)(HSD_MObj *, HSD_TObj *, HSD_TExp **);
    void (*setup_tev)(HSD_MObj *, HSD_TObj *, u32);
    AnimationUpdate update;
    void (*unset)(HSD_MObj *, u32);
} HSD_MObjInfo;
typedef struct HSD_MatAnim {
    struct HSD_MatAnim *next;
    HSD_AObjDesc *aobjdesc;
    void *texanim;
    void *renderanim;
} HSD_MatAnim;
extern HSD_MObjInfo hsdMObj;
extern void HSD_MObjSetCurrent(HSD_MObj *material);
extern void HSD_MObjRemoveAnimByFlags(HSD_MObj *material, u32 flags);
extern void HSD_MObjAddAnim(HSD_MObj *material, HSD_MatAnim *animation);
extern void HSD_MObjReqAnimByFlags(HSD_MObj *material, f32 frame, u32 flags);
extern void HSD_MObjAnim(HSD_MObj *material);
extern HSD_MObj *HSD_MObjLoadDesc(HSD_MObjDesc *description);
extern void HSD_MObjRemove(HSD_MObj *material);
extern HSD_MObj *HSD_MObjAlloc(void);
extern HSD_Material *HSD_MaterialAlloc(void);
extern void HSD_MObjCompileTev(HSD_MObj *material);
extern void HSD_MObjSetup(HSD_MObj *material, u32 render_mode);
extern void HSD_MObjUnset(HSD_MObj *material, u32 render_mode);
extern void HSD_MObjAddShadowTexture(HSD_TObj *texture);
extern void HSD_MObjDeleteShadowTexture(HSD_TObj *texture);
#endif

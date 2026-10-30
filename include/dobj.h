#ifndef ZOIDS_DOBJ_H
#define ZOIDS_DOBJ_H
/* Layouts follow https://github.com/doldecomp/melee/tree/master/src/sysdolphin/baselib.
 */
#include "class.h"
#include "mobj.h"
#include "pobj.h"
typedef f32 DObjMatrix[3][4];
struct HSD_ShapeAnimDObj {
    struct HSD_ShapeAnimDObj *next;
    HSD_ShapeAnim *shapeanim;
};
typedef struct HSD_DObj {
    HSD_Class parent;
    struct HSD_DObj *next;
    HSD_MObj *mobj;
    HSD_PObj *pobj;
    HSD_AObj *aobj;
    u32 flags;
} HSD_DObj;
typedef struct HSD_DObjDesc {
    char *class_name;
    struct HSD_DObjDesc *next;
    HSD_MObjDesc *mobjdesc;
    HSD_PObjDesc *pobjdesc;
} HSD_DObjDesc;
typedef struct HSD_DObjInfo {
    HSD_ClassInfo parent;
    void (*disp)(HSD_DObj *, DObjMatrix, DObjMatrix, u32);
    s32 (*load)(HSD_DObj *, HSD_DObjDesc *);
    void (*update)(void *, s32, AnimationValue *);
} HSD_DObjInfo;
extern HSD_DObjInfo hsdDObj;
extern HSD_DObj *HSD_DObjAlloc(void);
extern HSD_DObj *HSD_DObjLoadDesc(HSD_DObjDesc *desc);
extern void HSD_DObjSetCurrent(HSD_DObj *object);
extern void HSD_DObjRemoveAnimAllByFlags(HSD_DObj *object, u32 flags);
extern void HSD_DObjAddAnimAll(HSD_DObj *object, HSD_MatAnim *material_animation,
                               HSD_ShapeAnimDObj *shape_animation);
extern void HSD_DObjReqAnimAllByFlags(HSD_DObj *object, f32 frame, u32 flags);
extern void HSD_DObjAnimAll(HSD_DObj *object);
extern void HSD_DObjRemoveAll(HSD_DObj *object);
extern void HSD_DObjResolveRefsAll(HSD_DObj *object, HSD_DObjDesc *description);
extern void HSD_DObjDisp(HSD_DObj *object, DObjMatrix view, DObjMatrix projection,
                         u32 render_mode);
#endif

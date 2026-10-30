/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/dobj.c. */
#include "dobj.h"
#define NULL 0
#define Mtx DObjMatrix
#define next_p(p) ((p) ? (p)->next : NULL)
#define HSD_DOBJ(o) ((HSD_DObj *)(o))
#define HSD_DOBJ_METHOD(o) ((HSD_DObjInfo *)((o)->parent.class_info))
#define HSD_MOBJ_METHOD(o) ((HSD_MObjInfo *)((o)->parent.class_info))
#define HSD_POBJ_METHOD(o) ((HSD_PObjInfo *)((o)->parent.class_info))
#define HSD_CLASS_INFO(o) ((HSD_ClassInfo *)(o))
#define HSD_DOBJ_INFO(o) ((HSD_DObjInfo *)(o))
#define HSD_PARENT_INFO(o) ((o)->parent.head.parent)
#define DOBJ_TRSP_SHIFT 1
#define RENDER_SHADOW (1 << 26)
#define RENDER_NO_ZUPDATE (1 << 29)
#define RENDER_XLU (1 << 30)
#define RENDER_BLENDING (RENDER_XLU | RENDER_NO_ZUPDATE)
static void DObjInfoInit(void);
HSD_DObjInfo hsdDObj = {{{DObjInfoInit}}};
extern HSD_ClassInfo *default_class;
extern HSD_DObj *current_dobj;
extern HSD_ClassInfo hsdClass;
extern void __assert(const char *, u32, const char *);
extern void HSD_Panic(const char *, u32, const char *);
extern void OSReport(const char *, ...);
extern void HSD_PObjRemoveAnimAllByFlags(HSD_PObj *, u32);
extern void HSD_PObjAddAnimAll(HSD_PObj *, HSD_ShapeAnim *);
extern void HSD_PObjReqAnimAllByFlags(HSD_PObj *, f32, u32);
extern void HSD_PObjAnimAll(HSD_PObj *);
extern HSD_PObj *HSD_PObjLoadDesc(HSD_PObjDesc *);
extern void HSD_PObjResolveRefsAll(HSD_PObj *, HSD_PObjDesc *);
extern void HSD_PObjRemoveAll(HSD_PObj *);
extern void HSD_AObjRemove(HSD_AObj *);
extern void HSD_AObjInterpretAnim(HSD_AObj *, void *, AnimationUpdate);
static inline void hsdDelete(HSD_DObj *object)
{
    if (object != NULL) {
        object->parent.class_info->release((HSD_Class *)object);
        object->parent.class_info->destroy((HSD_Class *)object);
    }
}
void HSD_DObjSetCurrent(HSD_DObj *dobj)
{
    current_dobj = dobj;
}

static inline void HSD_DObjModifyFlags(HSD_DObj *dobj, u32 flags, u32 mask)
{
    if (dobj == NULL) {
        return;
    }

    dobj->flags = (dobj->flags & ~mask) | (flags & mask);
}

static inline void HSD_DObjRemoveAnimByFlags(HSD_DObj *dobj, u32 flags)
{
    if (dobj == NULL) {
        return;
    }

    if ((flags & 2) != 0) {
        HSD_AObjRemove(dobj->aobj);
        dobj->aobj = NULL;
    }
    HSD_PObjRemoveAnimAllByFlags(dobj->pobj, flags);
    HSD_MObjRemoveAnimByFlags(dobj->mobj, flags);
}

void HSD_DObjRemoveAnimAllByFlags(HSD_DObj *dobj, u32 flags)
{
    HSD_DObj *dp;

    if (dobj == NULL) {
        return;
    }

    for (dp = dobj; dp != NULL; dp = dp->next) {
        HSD_DObjRemoveAnimByFlags(dp, flags);
    }
}

static inline void HSD_DObjAddAnim(HSD_DObj *dobj, HSD_MatAnim *mat_anim,
                                   HSD_ShapeAnimDObj *sh_anim)
{
    HSD_ShapeAnim *shapeanim;

    if (dobj == NULL) {
        return;
    }

    if (sh_anim != NULL) {
        shapeanim = sh_anim->shapeanim;
    } else {
        shapeanim = NULL;
    }

    HSD_PObjAddAnimAll(dobj->pobj, shapeanim);
    HSD_MObjAddAnim(dobj->mobj, mat_anim);
}

void HSD_DObjAddAnimAll(HSD_DObj *dobj, HSD_MatAnim *matanim,
                        HSD_ShapeAnimDObj *shapeanimdobj)
{
    HSD_DObj *dp;
    HSD_MatAnim *ma;
    HSD_ShapeAnimDObj *sd;

    if (dobj == NULL) {
        return;
    }

    for (dp = dobj, ma = matanim, sd = shapeanimdobj; dp != NULL;
         dp = dp->next, ma = next_p(ma), sd = next_p(sd)) {
        HSD_DObjAddAnim(dp, ma, sd);
    }
}

static inline void HSD_DObjReqAnimByFlags(HSD_DObj *dobj, f32 startframe, u32 flags)
{
    if (dobj == NULL) {
        return;
    }

    HSD_PObjReqAnimAllByFlags(dobj->pobj, startframe, flags);
    HSD_MObjReqAnimByFlags(dobj->mobj, startframe, flags);
}

void HSD_DObjReqAnimAllByFlags(HSD_DObj *dobj, f32 startframe, u32 flags)
{
    HSD_DObj *dp;

    if (dobj == NULL) {
        return;
    }

    for (dp = dobj; dp != NULL; dp = dp->next) {
        HSD_DObjReqAnimByFlags(dp, startframe, flags);
    }
}

static inline void HSD_DObjAnim(HSD_DObj *dobj)
{
    if (dobj == NULL) {
        return;
    }

    HSD_AObjInterpretAnim(dobj->aobj, dobj, HSD_DOBJ_METHOD(dobj)->update);
    HSD_PObjAnimAll(dobj->pobj);
    HSD_MObjAnim(dobj->mobj);
}

void HSD_DObjAnimAll(HSD_DObj *dobj)
{
    HSD_DObj *dp;

    if (dobj == NULL) {
        return;
    }

    for (dp = dobj; dp != NULL; dp = dp->next) {
        HSD_DObjAnim(dp);
    }
}

static int DObjLoad(HSD_DObj *dobj, HSD_DObjDesc *desc)
{
    dobj->next = HSD_DObjLoadDesc(desc->next);
    dobj->mobj = HSD_MObjLoadDesc(desc->mobjdesc);
    dobj->pobj = HSD_PObjLoadDesc(desc->pobjdesc);

    if (dobj->mobj != NULL) {
        switch (dobj->mobj->rendermode & RENDER_BLENDING) {
        case 0:
            HSD_DObjModifyFlags(dobj, HSD_TRSP_OPA << DOBJ_TRSP_SHIFT,
                                HSD_TRSP_ALL << DOBJ_TRSP_SHIFT);
            break;
        case RENDER_XLU:
            HSD_DObjModifyFlags(dobj, HSD_TRSP_TEXEDGE << DOBJ_TRSP_SHIFT,
                                HSD_TRSP_ALL << DOBJ_TRSP_SHIFT);
            break;
        case RENDER_BLENDING:
            HSD_DObjModifyFlags(dobj, HSD_TRSP_XLU << DOBJ_TRSP_SHIFT,
                                HSD_TRSP_ALL << DOBJ_TRSP_SHIFT);
            break;
        default:
            OSReport("mobj has unexpected blending flags (0x%x).",
                     dobj->mobj->rendermode);
            HSD_Panic("dobj.c", 319, "");
        }
    }
    return 0;
}

HSD_DObj *HSD_DObjLoadDesc(HSD_DObjDesc *desc)
{
    HSD_DObj *dobj;
    HSD_ClassInfo *info;

    if (desc == NULL) {
        return NULL;
    }

    if (desc->class_name == NULL ||
        (info = hsdSearchClassInfo(desc->class_name)) == NULL) {
        dobj = HSD_DObjAlloc();
    } else {
        dobj = HSD_DOBJ(hsdNew(info));
        if (dobj == NULL) {
            __assert("dobj.c", 385, "dobj");
        }
    }
    HSD_DOBJ_METHOD(dobj)->load(dobj, desc);

    return dobj;
}

static inline void HSD_DObjRemove(HSD_DObj *dobj)
{
    hsdDelete(dobj);
}

void HSD_DObjRemoveAll(HSD_DObj *dobj)
{
    HSD_DObj *next;

    for (; dobj != NULL; dobj = next) {
        next = dobj->next;
        HSD_DObjRemove(dobj);
    }
}

#pragma push
#pragma force_active on
/* The release retains this unused class assertion. */
static char default_class_assertion[] = "hsdIsDescendantOf(info, &hsdDObj)";
#pragma pop

HSD_DObj *HSD_DObjAlloc(void)
{
    HSD_DObj *dobj = hsdNew(default_class ? default_class : &hsdDObj.parent);
    if (dobj == NULL) {
        __assert("dobj.c", 532, "dobj");
    }
    return dobj;
}

static inline void HSD_DObjResolveRefs(HSD_DObj *dobj, HSD_DObjDesc *desc)
{
    if (dobj == NULL || desc == NULL) {
        return;
    }
    HSD_PObjResolveRefsAll(dobj->pobj, desc->pobjdesc);
}

void HSD_DObjResolveRefsAll(HSD_DObj *dobj, HSD_DObjDesc *desc)
{
    for (; dobj != NULL && desc != NULL; dobj = dobj->next, desc = desc->next) {
        HSD_DObjResolveRefs(dobj, desc);
    }
}

#pragma push
#pragma force_active on
/* The release retains these unused link diagnostics. */
static char polygon_link_warning[] = "can not find specified pobj in link.\n";
static char polygon_link_assertion[] = "can not find specified pobj in link.";
static char material_assertion[] = "dobj->mobj == mobj";
#pragma pop

void HSD_DObjDisp(HSD_DObj *dobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    HSD_PObj *p;

    HSD_MObjSetCurrent(dobj->mobj);
    if ((rendermode & RENDER_SHADOW) == 0) {
        HSD_MOBJ_METHOD(dobj->mobj)->setup(dobj->mobj, rendermode);
    }
    for (p = dobj->pobj; p != NULL; p = p->next) {
        HSD_POBJ_METHOD(p)->disp(p, vmtx, pmtx, rendermode);
    }
    if ((rendermode & RENDER_SHADOW) == 0) {
        HSD_MOBJ_METHOD(dobj->mobj)->unset(dobj->mobj, rendermode);
    }
    HSD_MObjSetCurrent(NULL);
}

static void DObjRelease(HSD_Class *o)
{
    HSD_DObj *dobj = HSD_DOBJ(o);

    HSD_MObjRemove(dobj->mobj);
    HSD_PObjRemoveAll(dobj->pobj);
    HSD_AObjRemove(dobj->aobj);

    HSD_PARENT_INFO(&hsdDObj)->release(o);
}

static void DObjAmnesia(HSD_ClassInfo *info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdDObj)->forget(info);
}

static void DObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdDObj), HSD_CLASS_INFO(&hsdClass),
                     "sysdolphin_base_library", "hsd_dobj", sizeof(HSD_DObjInfo),
                     sizeof(HSD_DObj));

    HSD_CLASS_INFO(&hsdDObj)->release = DObjRelease;
    HSD_CLASS_INFO(&hsdDObj)->forget = DObjAmnesia;
    HSD_DOBJ_INFO(&hsdDObj)->disp = HSD_DObjDisp;
    HSD_DOBJ_INFO(&hsdDObj)->load = DObjLoad;
}

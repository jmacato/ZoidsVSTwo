/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/mobj.c. */
#include "mobj.h"
#include "tobj.h"
#define NULL 0
#define GX_TEV_ADD 0
#define GX_TB_ZERO 0
#define GX_CS_SCALE_1 0
#define RENDER_DIFFUSE_MAT 1
#define RENDER_DIFFUSE_VTX 2
#define RENDER_ALPHA_MAT (1 << 13)
#define RENDER_ALPHA_VTX (2U << 13)
#define RENDER_DIFFUSE_BITS 3
#define RENDER_ALPHA_BITS (3 << 13)
#define RENDER_DIFFUSE 4
#define RENDER_SPECULAR 8
#define RENDER_TOON 0x1000
#define RENDER_SHADOW 0x4000000
#define HSD_TOBJ_METHOD(o) ((HSD_TObjInfo *)((o)->class_info))
extern HSD_TExpType HSD_TExpGetType(HSD_TExp *);

#define HSD_CLASS_METHOD(o) ((o)->parent.class_info)
#define HSD_MOBJ(o) ((HSD_MObj *)(o))
#define HSD_MOBJ_INFO(o) ((HSD_MObjInfo *)(o))
#define HSD_CLASS_INFO(o) ((HSD_ClassInfo *)(o))
#define HSD_PARENT_INFO(o) ((o)->parent.head.parent)
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("mobj.c", line, #condition);                                      \
    } while (0)
extern HSD_ClassInfo hsdClass;
extern HSD_ClassInfo *default_class;
extern HSD_TObj *tobj_shadows;
extern HSD_TObj *tobj_toon;
extern void HSD_StateInitTev(void);
extern void HSD_SetMaterialColor(GXColor, GXColor, GXColor, f32);
extern void HSD_SetMaterialShininess(f32);
extern void HSD_SetupRenderModeWithCustomPE(u32, HSD_PEDesc *);
extern void hsdFreeMemPiece(void *, u32);
extern void *memset(void *, s32, u32);
void HSD_MObjSetup(HSD_MObj *, u32);
void HSD_MObjUnset(HSD_MObj *, u32);
static void MObjInfoInit(void);
HSD_MObjInfo hsdMObj = {{{MObjInfoInit}}};

#define MOBJ_ANIM 4
#define HSD_MOBJ_METHOD(o) ((HSD_MObjInfo *)((o)->parent.class_info))
extern HSD_MObj *current_mobj;
extern void HSD_AObjRemove(HSD_AObj *);
extern HSD_AObj *HSD_AObjLoadDesc(HSD_AObjDesc *);
extern void HSD_AObjReqAnim(HSD_AObj *, f32);
extern void HSD_AObjInterpretAnim(HSD_AObj *, void *, AnimationUpdate);
extern void *memcpy(void *, const void *, u32);
extern void *hsdAllocMemPiece(u32);
extern void __assert(const char *, u32, const char *);
void HSD_MObjSetCurrent(HSD_MObj *mobj)
{
    current_mobj = mobj;
}

void HSD_MObjRemoveAnimByFlags(HSD_MObj *mobj, u32 flags)
{
    if (mobj == NULL) {
        return;
    }

    if (flags & MOBJ_ANIM) {
        HSD_AObjRemove(mobj->aobj);
        mobj->aobj = NULL;
    }
    if (flags & TOBJ_ANIM) {
        HSD_TObjRemoveAnimAll(mobj->tobj);
    }
}

void HSD_MObjAddAnim(HSD_MObj *mobj, HSD_MatAnim *matanim)
{
    if (mobj == NULL) {
        return;
    }

    if (matanim != NULL) {
        if (mobj->aobj != NULL) {
            HSD_AObjRemove(mobj->aobj);
        }
        mobj->aobj = HSD_AObjLoadDesc(matanim->aobjdesc);
        HSD_TObjAddAnimAll(mobj->tobj, matanim->texanim);
    }
}

void HSD_MObjReqAnimByFlags(HSD_MObj *mobj, f32 startframe, u32 flags)
{
    if (mobj == NULL) {
        return;
    }
    if (flags & MOBJ_ANIM) {
        HSD_AObjReqAnim(mobj->aobj, startframe);
    }
    HSD_TObjReqAnimAllByFlags(mobj->tobj, startframe, flags);
}

static void MObjUpdateFunc(void *obj, s32 type, AnimationValue *val)
{
    HSD_MObj *mobj = obj;

    if (mobj == NULL) {
        return;
    }

    switch (type) {
    case HSD_A_M_AMBIENT_R:
        mobj->mat->ambient.r = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_AMBIENT_G:
        mobj->mat->ambient.g = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_AMBIENT_B:
        mobj->mat->ambient.b = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_DIFFUSE_R:
        mobj->mat->diffuse.r = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_DIFFUSE_G:
        mobj->mat->diffuse.g = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_DIFFUSE_B:
        mobj->mat->diffuse.b = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_ALPHA:
        mobj->mat->alpha = 1.0F - val->scalar;
        break;
    case HSD_A_M_SPECULAR_R:
        mobj->mat->specular.r = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_SPECULAR_G:
        mobj->mat->specular.g = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_SPECULAR_B:
        mobj->mat->specular.b = (u8)(255.0 * val->scalar);
        break;
    case HSD_A_M_PE_REF0:
        if (mobj->pe) {
            mobj->pe->ref0 = (u8)(255.0 * val->scalar);
        }
        break;
    case HSD_A_M_PE_REF1:
        if (mobj->pe) {
            mobj->pe->ref1 = (u8)(255.0 * val->scalar);
        }
        break;
    case HSD_A_M_PE_DSTALPHA:
        if (mobj->pe) {
            mobj->pe->dst_alpha = (u8)(255.0 * val->scalar);
        }
        break;
    }
}

void HSD_MObjAnim(HSD_MObj *mobj)
{
    if (mobj == NULL) {
        return;
    }
    HSD_AObjInterpretAnim(mobj->aobj, mobj, HSD_MOBJ_METHOD(mobj)->update);
    HSD_TObjAnimAll(mobj->tobj);
}

static int MObjLoad(HSD_MObj *mobj, HSD_MObjDesc *desc)
{
    mobj->rendermode = desc->rendermode;
    mobj->tobj = HSD_TObjLoadDesc(desc->texdesc);
    mobj->mat = HSD_MaterialAlloc();
    memcpy(mobj->mat, desc->mat, sizeof(HSD_Material));
    mobj->rendermode |= RENDER_TOON;
    if (desc->pedesc != NULL) {
        mobj->pe = hsdAllocMemPiece(sizeof(HSD_PEDesc));
        memcpy(mobj->pe, desc->pedesc, sizeof(HSD_PEDesc));
    }
    mobj->aobj = NULL;
    return 0;
}

HSD_MObj *HSD_MObjLoadDesc(HSD_MObjDesc *mobjdesc)
{
    if (mobjdesc) {
        HSD_MObj *mobj;
        HSD_ClassInfo *info;

        if (!mobjdesc->class_name ||
            !(info = hsdSearchClassInfo(mobjdesc->class_name))) {
            mobj = HSD_MObjAlloc();
        } else {
            mobj = hsdNew(info);
            if (mobj == NULL) {
                __assert("mobj.c", 349, "mobj");
            }
        }

        HSD_MOBJ_METHOD(mobj)->load(mobj, mobjdesc);
        HSD_MObjCompileTev(mobj);

        return mobj;
    } else {
        return NULL;
    }
}

static HSD_TExp *MObjMakeTExp(HSD_MObj *mobj, HSD_TObj *tobj_top, HSD_TExp **list)
{
    HSD_TExp *diff, *spec, *ext, *alpha;
    s32 valid_list = list != NULL;
    HSD_TExp *exp, *exp_2, *exp_3;
    HSD_TObj *tobj, *tobj_2, *tobj_3, *tobj_4, *toon = NULL;
    u32 done = 0;
    s32 color_selector, alpha_selector;
    static u8 one = 255;

    if (!valid_list)
        __assert("mobj.c", 371, "list");
    *list = NULL;
    for (tobj = tobj_top; tobj != NULL; tobj = tobj->next) {
        if (tobj_coord(tobj) == TEX_COORD_TOON)
            toon = tobj;
    }
    color_selector = mobj->rendermode & RENDER_DIFFUSE_BITS;
    if (color_selector == 0)
        color_selector = RENDER_DIFFUSE_MAT;
    alpha_selector = mobj->rendermode & RENDER_ALPHA_BITS;
    if (alpha_selector == 0)
        alpha_selector = color_selector << 13;
    exp = HSD_TExpTev(list);
    if (mobj->rendermode & RENDER_DIFFUSE) {
        switch (color_selector) {
        case RENDER_DIFFUSE_VTX:
            HSD_TExpColorOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_1, HSD_TEXP_ZERO);
            break;
        default: {
            HSD_TExp *constant =
                HSD_TExpCnst(&mobj->mat->diffuse, HSD_TE_RGB, HSD_TE_U8, list);
            HSD_TExpColorOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, constant);
            break;
        }
        }
        switch (alpha_selector) {
        case RENDER_ALPHA_VTX: {
            HSD_TExp *constant = HSD_TExpCnst(&one, HSD_TE_X, HSD_TE_U8, list);
            HSD_TExpAlphaOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_X, constant);
            break;
        }
        default: {
            HSD_TExp *constant =
                HSD_TExpCnst(&mobj->mat->alpha, HSD_TE_X, HSD_TE_F32, list);
            HSD_TExpAlphaOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_X, constant);
            break;
        }
        }
    } else {
        switch (color_selector) {
        case RENDER_DIFFUSE_MAT: {
            HSD_TExp *constant =
                HSD_TExpCnst(&mobj->mat->diffuse, HSD_TE_RGB, HSD_TE_U8, list);
            HSD_TExpColorOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, constant);
            break;
        }
        case RENDER_DIFFUSE_VTX:
            HSD_TExpOrder(exp, toon, GX_COLOR0A0);
            HSD_TExpColorOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB,
                            toon != NULL ? HSD_TEXP_TEX : HSD_TEXP_RAS);
            break;
        default: {
            HSD_TExp *constant =
                HSD_TExpCnst(&mobj->mat->diffuse, HSD_TE_RGB, HSD_TE_U8, list);
            HSD_TExpOrder(exp, toon, GX_COLOR0A0);
            HSD_TExpColorOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp, HSD_TE_RGB, toon != NULL ? HSD_TEXP_TEX : HSD_TEXP_RAS,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_RGB, constant);
            break;
        }
        }
        switch (alpha_selector) {
        case RENDER_ALPHA_MAT: {
            HSD_TExp *constant =
                HSD_TExpCnst(&mobj->mat->alpha, HSD_TE_X, HSD_TE_F32, list);
            HSD_TExpAlphaOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_X, constant);
            break;
        }
        case RENDER_ALPHA_VTX:
            HSD_TExpOrder(exp, toon, GX_COLOR0A0);
            HSD_TExpAlphaOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, HSD_TEXP_RAS);
            break;
        default: {
            HSD_TExp *constant =
                HSD_TExpCnst(&mobj->mat->alpha, HSD_TE_X, HSD_TE_F32, list);
            HSD_TExpOrder(exp, toon, GX_COLOR0A0);
            HSD_TExpAlphaOp(exp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, HSD_TEXP_RAS,
                            HSD_TE_X, constant, HSD_TE_0, HSD_TEXP_ZERO);
            break;
        }
        }
    }
    diff = exp;
    alpha = exp;
    for (tobj_2 = tobj_top; tobj_2 != NULL; tobj_2 = tobj_2->next) {
        if ((tobj_2->flags & (TEX_LIGHTMAP_DIFFUSE | TEX_LIGHTMAP_AMBIENT)) &&
            tobj_2->id != GX_TEXMAP_NULL) {
            HSD_TOBJ_METHOD(tobj_2)->make_texp(
                tobj_2, TEX_LIGHTMAP_DIFFUSE | TEX_LIGHTMAP_AMBIENT, done, &diff,
                &alpha, list);
        }
    }
    done |= TEX_LIGHTMAP_DIFFUSE | TEX_LIGHTMAP_AMBIENT;
    if (mobj->rendermode & RENDER_DIFFUSE) {
        if (alpha_selector & RENDER_ALPHA_VTX) {
            exp_2 = HSD_TExpTev(list);
            HSD_TExpOrder(exp_2, NULL, GX_COLOR1A1);
            HSD_TExpColorOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp_2, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, diff);
            HSD_TExpAlphaOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp_2, HSD_TE_A, alpha, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A,
                            HSD_TEXP_RAS, HSD_TE_0, HSD_TEXP_ZERO);
            diff = exp_2;
            alpha = exp_2;
        }
        exp_2 = HSD_TExpTev(list);
        if (toon != NULL) {
            HSD_TExpOrder(exp_2, toon, GX_COLOR0A0);
            HSD_TExpColorOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp_2, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, diff,
                            HSD_TE_RGB, HSD_TEXP_TEX, HSD_TE_0, HSD_TEXP_ZERO);
        } else {
            HSD_TExpOrder(exp_2, NULL, GX_COLOR0A0);
            HSD_TExpColorOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpColorIn(exp_2, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, diff,
                            HSD_TE_RGB, HSD_TEXP_RAS, HSD_TE_0, HSD_TEXP_ZERO);
        }
        diff = exp_2;
        if (alpha_selector & RENDER_ALPHA_VTX) {
            HSD_TExpAlphaOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp_2, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, alpha, HSD_TE_A,
                            HSD_TEXP_RAS, HSD_TE_0, HSD_TEXP_ZERO);
        } else {
            HSD_TExpAlphaOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
            HSD_TExpAlphaIn(exp_2, HSD_TE_A, alpha, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A,
                            HSD_TEXP_RAS, HSD_TE_0, HSD_TEXP_ZERO);
        }
        alpha = exp_2;
    }
    if (mobj->rendermode & RENDER_SPECULAR) {
        HSD_TExp *constant =
            HSD_TExpCnst(&mobj->mat->specular, HSD_TE_RGB, HSD_TE_U8, list);
        exp_3 = HSD_TExpTev(list);
        HSD_TExpColorOp(exp_3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        HSD_TExpColorIn(exp_3, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                        HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, constant);
        spec = exp_3;
        for (tobj_3 = tobj_top; tobj_3 != NULL; tobj_3 = tobj_3->next) {
            if ((tobj_3->flags & TEX_LIGHTMAP_SPECULAR) &&
                tobj_3->id != GX_TEXMAP_NULL) {
                HSD_TOBJ_METHOD(tobj_3)->make_texp(tobj_3, TEX_LIGHTMAP_SPECULAR, done,
                                                   &spec, &alpha, list);
            }
        }
        done |= TEX_LIGHTMAP_SPECULAR;
        exp_3 = HSD_TExpTev(list);
        HSD_TExpOrder(exp_3, NULL, GX_COLOR1A1);
        HSD_TExpColorOp(exp_3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        HSD_TExpColorIn(exp_3, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, spec, HSD_TE_RGB,
                        HSD_TEXP_RAS, HSD_TE_0, HSD_TEXP_ZERO);
        spec = exp_3;
        exp_3 = HSD_TExpTev(list);
        HSD_TExpColorOp(exp_3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        HSD_TExpColorIn(exp_3, HSD_TE_RGB, spec, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                        HSD_TEXP_ZERO, HSD_TE_RGB, diff);
        diff = exp_3;
    }
    ext = diff;
    for (tobj_4 = tobj_top; tobj_4 != NULL; tobj_4 = tobj_4->next) {
        if ((tobj_4->flags & TEX_LIGHTMAP_EXT) && tobj_4->id != GX_TEXMAP_NULL) {
            HSD_TOBJ_METHOD(tobj_4)->make_texp(tobj_4, TEX_LIGHTMAP_EXT, done, &ext,
                                               &alpha, list);
        }
    }
    if (ext != alpha || HSD_TExpGetType(ext) != HSD_TE_TEV ||
        HSD_TExpGetType(alpha) != HSD_TE_TEV) {
        exp_2 = HSD_TExpTev(list);
        HSD_TExpColorOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        HSD_TExpColorIn(exp_2, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                        HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, ext);
        HSD_TExpAlphaOp(exp_2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        HSD_TExpAlphaIn(exp_2, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                        HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, alpha);
        return exp_2;
    }
    return ext;
}

void HSD_MObjCompileTev(HSD_MObj *mobj)
{
    HSD_TObj *tobj, **tail;
    HSD_TExp *texp;

    tail = NULL;
    if (mobj != NULL) {
        if (mobj->tevdesc != NULL) {
            HSD_TExpFreeTevDesc(mobj->tevdesc);
            mobj->tevdesc = NULL;
        }
        if (mobj->texp != NULL) {
            HSD_TExpFreeList(mobj->texp, HSD_TE_ALL, 1);
            mobj->texp = NULL;
        }
        tobj = mobj->tobj;
        if (mobj->rendermode & RENDER_SHADOW) {
            if (tobj_shadows != NULL) {
                tail = &tobj;
                while (*tail != NULL) {
                    tail = &(*tail)->next;
                }
                *tail = tobj_shadows;
            }
        }
        if (mobj->rendermode & RENDER_TOON) {
            if (tobj_toon != NULL && tobj_toon->imagedesc != NULL) {
                tobj_toon->next = tobj;
                tobj = tobj_toon;
            }
        }
        HSD_TObjAssignResources(tobj);
        texp = HSD_MOBJ_METHOD(mobj)->make_texp(mobj, tobj, &mobj->texp);
        HSD_TExpCompile(texp, &mobj->tevdesc, &mobj->texp);
        if (tail != NULL) {
            *tail = NULL;
        }
    }
}

#pragma push
#pragma force_active on
/* The release retains this unused class assertion. */
static char default_class_assertion[] = "hsdIsDescendantOf(info, &hsdMObj)";
#pragma pop
void MObjSetupTev(HSD_MObj *mobj, HSD_TObj *tobj, u32 rendermode)
{
    HSD_ASSERT(774, mobj->tevdesc);
    HSD_TExpSetupTev(mobj->tevdesc, mobj->texp);
    HSD_TObjSetupVolatileTev(tobj, rendermode);
}

void HSD_MObjSetup(HSD_MObj *mobj, u32 rendermode)
{
    HSD_TObj *tobj, **tail;

    HSD_StateInitTev();
    rendermode = mobj->rendermode;
    HSD_SetMaterialColor(mobj->mat->ambient, mobj->mat->diffuse, mobj->mat->specular,
                         mobj->mat->alpha);
    if (rendermode & RENDER_SPECULAR) {
        HSD_SetMaterialShininess(mobj->mat->shininess);
    }

    tobj = mobj->tobj;
    tail = NULL;

    if ((rendermode & RENDER_SHADOW) && tobj_shadows != NULL) {
        tail = &tobj;
        while (*tail != NULL) {
            tail = &(*tail)->next;
        }
        *tail = tobj_shadows;
    }
    if ((rendermode & RENDER_TOON) && tobj_toon != NULL &&
        tobj_toon->imagedesc != NULL) {
        tobj_toon->next = tobj;
        tobj = tobj_toon;
    }
    HSD_TObjSetup(tobj);
    HSD_TObjSetupTextureCoordGen(tobj);
    HSD_MOBJ_METHOD(mobj)->setup_tev(mobj, tobj, rendermode);
    HSD_SetupRenderModeWithCustomPE(rendermode, mobj->pe);
    if (tail != NULL) {
        *tail = NULL;
    }
#pragma push
#pragma force_active on
    {
        /* The release retains this unused toon descriptor. */
        static HSD_TObjDesc desc = {NULL,      NULL, 7, 19,   {0, 0, 0}, {0, 0, 0},
                                    {0, 0, 0}, 0,    0, 0,    0,         4,
                                    1.0F,      1,    0, NULL, NULL,      NULL};
    }
#pragma pop
}

#pragma push
#pragma force_active on
/* The release retains these unused toon diagnostics. */
static char toon_warning[] = "cannot allocate tobj for toon.";
static char toon_assertion[] = "tobj_toon";
#pragma pop
void HSD_MObjUnset(HSD_MObj *mobj, u32 rendermode)
{
    HSD_TObjSetup(NULL);
}

void HSD_MObjRemove(HSD_MObj *mobj)
{
    if (mobj != NULL) {
        HSD_CLASS_METHOD(mobj)->release((HSD_Class *)mobj);
        HSD_CLASS_METHOD(mobj)->destroy((HSD_Class *)mobj);
    }
}

HSD_MObj *HSD_MObjAlloc(void)
{
    HSD_MObj *mobj = hsdNew(default_class != NULL ? default_class : &hsdMObj.parent);
    HSD_ASSERT(1074, mobj);
    return mobj;
}

HSD_Material *HSD_MaterialAlloc(void)
{
    HSD_Material *mat = hsdAllocMemPiece(sizeof(HSD_Material));
    HSD_ASSERT(1102, mat);
    memset(mat, 0, sizeof(HSD_Material));
    mat->alpha = 1.0F;
    return mat;
}

void HSD_MObjAddShadowTexture(HSD_TObj *tobj)
{
    HSD_TObj *cur;
    HSD_ASSERT(1149, tobj);
    for (cur = tobj_shadows; cur != NULL; cur = cur->next) {
        if (cur == tobj) {
            return;
        }
    }
    tobj->next = tobj_shadows;
    tobj_shadows = tobj;
}

void HSD_MObjDeleteShadowTexture(HSD_TObj *tobj)
{
    if (tobj != NULL) {
        HSD_TObj **cur = &tobj_shadows;
        while (*cur != NULL) {
            if (*cur == tobj) {
                *cur = tobj->next;
                tobj->next = NULL;
                return;
            }
            cur = &(*cur)->next;
        }
    } else {
        HSD_TObj *next;
        for (next = NULL; tobj_shadows != NULL; tobj_shadows = next) {
            next = tobj_shadows->next;
            tobj_shadows->next = NULL;
        }
    }
}

static void MObjRelease(HSD_Class *o)
{
    HSD_MObj *mobj = HSD_MOBJ(o);

    HSD_AObjRemove(mobj->aobj);
    hsdFreeMemPiece(mobj->mat, sizeof(HSD_Material));
    HSD_TObjRemoveAll(mobj->tobj);

    if (mobj->tevdesc != NULL) {
        HSD_TExpFreeTevDesc(mobj->tevdesc);
    }
    if (mobj->texp != NULL) {
        HSD_TExpFreeList(mobj->texp, HSD_TE_ALL, 1);
    }
    if (mobj->pe != NULL) {
        hsdFreeMemPiece(mobj->pe, sizeof(HSD_PEDesc));
    }
    HSD_PARENT_INFO(&hsdMObj)->release(o);
}

static void MObjAmnesia(HSD_ClassInfo *info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdMObj)) {
        tobj_toon = NULL;
        tobj_shadows = NULL;
    }
    HSD_PARENT_INFO(&hsdMObj)->forget(info);
}

static void MObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdMObj), HSD_CLASS_INFO(&hsdClass),
                     "sysdolphin_base_library", "hsd_mobj", sizeof(HSD_MObjInfo),
                     sizeof(HSD_MObj));

    HSD_CLASS_INFO(&hsdMObj)->release = MObjRelease;
    HSD_CLASS_INFO(&hsdMObj)->forget = MObjAmnesia;
    HSD_MOBJ_INFO(&hsdMObj)->setup = HSD_MObjSetup;
    HSD_MOBJ_INFO(&hsdMObj)->unset = HSD_MObjUnset;
    HSD_MOBJ_INFO(&hsdMObj)->load = MObjLoad;
    HSD_MOBJ_INFO(&hsdMObj)->make_texp = MObjMakeTExp;
    HSD_MOBJ_INFO(&hsdMObj)->setup_tev = MObjSetupTev;
    HSD_MOBJ_INFO(&hsdMObj)->update = MObjUpdateFunc;
}

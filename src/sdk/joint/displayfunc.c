/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/displayfunc.c.
 */
#include "jobj.h"
#include "cobj.h"
#include "lobj.h"
#define NULL 0
#define JOBJ_BILLBOARD_FIELD 0xE00
#define JOBJ_BILLBOARD 0x200
#define JOBJ_VBILLBOARD 0x400
#define JOBJ_HBILLBOARD 0x600
#define JOBJ_RBILLBOARD 0x800
#define JOBJ_PBILLBOARD 0x2000
#define JOBJ_SKELETON 1
#define JOBJ_SKELETON_ROOT 2
#define JOBJ_HIDDEN 0x10
#define JOBJ_PTCL 0x20
#define JOBJ_SPLINE 0x4000
#define JOBJ_SPECULAR 0x10000
#define JOBJ_TRSP_SHIFT 18
#define JOBJ_OPA 0x40000
#define JOBJ_XLU 0x80000
#define JOBJ_TEXEDGE 0x100000
#define JOBJ_PTCL_BANK_MASK 0x3F
#define JOBJ_PTCL_OFFSET_SHIFT 6
#define JOBJ_PTCL_OFFSET_MASK 0xFFFFFF
#define JOBJ_PTCL_ACTIVE 0x7FFFFFFF
#define DOBJ_TRSP_SHIFT 1
#define DOBJ_HIDDEN 1
#define RENDER_SHADOW 0x4000000
#define union_type_dobj(o) ((o)->flags & (JOBJ_PTCL | JOBJ_SPLINE) ? 0 : 1)
#define union_type_ptcl(o) ((o)->flags & JOBJ_PTCL ? 1 : 0)
#define HSD_JOBJ_METHOD(o) ((HSD_JObjInfo *)((o)->class_info))
#define HSD_DOBJ_METHOD(o) ((HSD_DObjInfo *)((o)->parent.class_info))
#define HSD_JObjSetupMatrix setup_matrix
#define HSD_CObjGetViewingMtxPtrDirect(o) ((o)->view_mtx)
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("displayfunc.c", line, #condition);                               \
    } while (0)
#define mtx matrix
#define envelopemtx envelope_matrix
#define MTXConcat PSMTXConcat
#define MTXInverse PSMTXInverse
#define MTXCopy PSMTXCopy
#define MTXScale PSMTXScale
#define MTXRotRad PSMTXRotRad
#define VECMag PSVECMag
#define VECCrossProduct PSVECCrossProduct
#define HSD_TrspMask u32
extern f64 __frsqrte(f64);
extern s32 PSMTXInverse(Mtx, Mtx);
extern void PSMTXRotRad(Mtx, u8, f32);
extern void PSVECCrossProduct(Vec3 *, Vec3 *, Vec3 *);
extern void *memset(void *, s32, u32);
extern void _HSD_ZListClear(void);
static inline f32 sqrtf__Ff(f32 value)
{
    if (value > 0.0F) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        estimate = 0.5 * estimate * (3.0 - estimate * estimate * value);
        result = value * estimate;
        return result;
    }
    return value;
}
static inline void HSD_MtxColVec(Mtx m, u32 column, Vec3 *out)
{
    out->x = m[0][column];
    out->y = m[1][column];
    out->z = m[2][column];
}
static inline void HSD_MtxSetColVec(Mtx m, u32 column, Vec3 *in)
{
    m[0][column] = in->x;
    m[1][column] = in->y;
    m[2][column] = in->z;
}
static inline f32 HSD_MtxColMag(Mtx m, u32 column)
{
    return sqrtf__Ff(m[0][column] * m[0][column] + m[1][column] * m[1][column] +
                     m[2][column] * m[2][column]);
}
typedef struct _HSD_ZList {
    Mtx pmtx;
    MtxPtr vmtx;
    HSD_JObj *jobj;
    u32 rendermode;

    struct _HSD_ZList_sort {
        struct _HSD_ZList *texedge;
        struct _HSD_ZList *xlu;
    } sort;

    struct _HSD_ZList *next;
} HSD_ZList;

static HSD_ObjAllocData zlist_alloc_data;

static void (*sptcl_callback)(s32, s32, s32, HSD_JObj *) = NULL;

static int zsort_listing = 0;
static int zsort_sorting = 0;

static HSD_ZList *zlist_top = NULL;
static HSD_ZList **zlist_bottom = &zlist_top;

static HSD_ZList *zlist_texedge_top = NULL;
static HSD_ZList **zlist_texedge_bottom = &zlist_texedge_top;
static int zlist_texedge_nb = 0;

static HSD_ZList *zlist_xlu_top = NULL;
static HSD_ZList **zlist_xlu_bottom = &zlist_xlu_top;
static int zlist_xlu_nb = 0;

#define ZLIST_NEXT(list, offset) (*(HSD_ZList **)(((u8 *)(list)) + (offset)))
#define ZLIST_LINK_OFFSET(link) ((s32)&((HSD_ZList *)0)->sort.link)

void HSD_ZListInitAllocData(void)
{
    HSD_ObjAllocInit(&zlist_alloc_data, sizeof(HSD_ZList), 4);
}

static inline HSD_ZList *HSD_ZListAlloc(void)
{
    HSD_ZList *list;

    list = HSD_ObjAlloc(&zlist_alloc_data);
    memset(&list->vmtx, 0, sizeof(HSD_ZList) - sizeof(Mtx));

    return list;
}

static inline void HSD_ZListFree(HSD_ZList *ptr)
{
    HSD_ObjFree(&zlist_alloc_data, ptr);
}

static void mkVBillBoardMtx(HSD_JObj *jobj, MtxPtr src, MtxPtr dst)
{
    static Vec3 uz = {0.0F, 0.0F, 1.0F};
    Vec3 pos, ax, ay, az;
    float sx, sz;

    HSD_MtxColVec(src, 3, &pos);
    HSD_MtxColVec(src, 1, &ay);

    sx = HSD_MtxColMag(src, 0);
    sz = HSD_MtxColMag(src, 2);

    if (jobj->flags & JOBJ_PBILLBOARD) {
        VECCrossProduct(&pos, &ay, &ax);
    } else {
        VECCrossProduct(&ay, &uz, &ax);
    }
    VECCrossProduct(&ax, &ay, &az);
    sx /= VECMag(&ax);
    sz /= VECMag(&az);

    dst[0][0] = sx * ax.x;
    dst[1][0] = sx * ax.y;
    dst[2][0] = sx * ax.z;

    HSD_MtxSetColVec(dst, 1, &ay);

    dst[0][2] = sz * az.x;
    dst[1][2] = sz * az.y;
    dst[2][2] = sz * az.z;

    HSD_MtxSetColVec(dst, 3, &pos);
}

static void mkHBillBoardMtx(HSD_JObj *jobj, MtxPtr src, MtxPtr dst)
{
    static Vec3 uy = {0.0F, 1.0F, 0.0F};
    Vec3 pos, ax, ay, az;
    Vec3 up_direction;
    float sy, sz;

    HSD_MtxColVec(src, 3, &pos);
    HSD_MtxColVec(src, 0, &ax);

    sy = HSD_MtxColMag(src, 1);
    sz = HSD_MtxColMag(src, 2);

    if (jobj->flags & JOBJ_PBILLBOARD) {
        up_direction.y = sqrtf__Ff(pos.x * pos.x + pos.z * pos.z);
        up_direction.x = -pos.y / up_direction.y * pos.x;
        up_direction.z = -pos.y / up_direction.y * pos.z;
        VECCrossProduct(&ax, &up_direction, &az);
    } else {
        VECCrossProduct(&ax, &uy, &az);
    }
    VECCrossProduct(&az, &ax, &ay);
    sy /= VECMag(&ay);
    sz /= VECMag(&az);

    HSD_MtxSetColVec(dst, 0, &ax);

    dst[0][1] = sy * ay.x;
    dst[1][1] = sy * ay.y;
    dst[2][1] = sy * ay.z;

    dst[0][2] = sz * az.x;
    dst[1][2] = sz * az.y;
    dst[2][2] = sz * az.z;

    HSD_MtxSetColVec(dst, 3, &pos);
}

static void mkBillBoardMtx(HSD_JObj *jobj, MtxPtr src, MtxPtr dst)
{
    static Vec3 caz = {0.0F, 0.0F, 1.0F};
    Vec3 ax, ay, *az, pos;
    float sx, sy, sz;

    sx = HSD_MtxColMag(src, 0);
    sz = HSD_MtxColMag(src, 2);

    HSD_MtxColVec(src, 1, &ay);
    sy = VECMag(&ay);
    HSD_MtxColVec(src, 3, &pos);

    if (jobj->flags & JOBJ_PBILLBOARD) {
        VECCrossProduct(&pos, &ay, &ax);
        VECCrossProduct(&ax, &pos, &ay);
        az = &pos;
        sz /= -VECMag(az);
    } else {
        VECCrossProduct(&ay, &caz, &ax);
        VECCrossProduct(&caz, &ax, &ay);
        az = &caz;
        sz /= VECMag(&caz);
    }
    sx /= VECMag(&ax);
    sy /= VECMag(&ay);

    dst[0][0] = sx * ax.x;
    dst[1][0] = sx * ax.y;
    dst[2][0] = sx * ax.z;

    dst[0][1] = sy * ay.x;
    dst[1][1] = sy * ay.y;
    dst[2][1] = sy * ay.z;

    dst[0][2] = sz * az->x;
    dst[1][2] = sz * az->y;
    dst[2][2] = sz * az->z;

    HSD_MtxSetColVec(dst, 3, &pos);
}

static inline void mkRBillBoardMtx(HSD_JObj *jobj, MtxPtr src, MtxPtr dst)
{
    Mtx rot, scl;
    f32 sx, sy, sz;
    sx = HSD_MtxColMag(src, 0);
    sy = HSD_MtxColMag(src, 1);
    sz = HSD_MtxColMag(src, 2);
    MTXScale(scl, sx, sy, sz);
    MTXRotRad(rot, 'z', jobj->rotation[2]);
    rot[0][3] = src[0][3];
    rot[1][3] = src[1][3];
    rot[2][3] = src[2][3];
    MTXConcat(rot, scl, dst);
}

void HSD_JObjMakePositionMtx(HSD_JObj *jobj, Mtx vmtx, Mtx pmtx)
{
    Mtx mtx;

    if (jobj->flags & JOBJ_BILLBOARD_FIELD) {
        MTXConcat(vmtx, jobj->mtx, mtx);
        switch (jobj->flags & JOBJ_BILLBOARD_FIELD) {
        case JOBJ_BILLBOARD:
            mkBillBoardMtx(jobj, mtx, pmtx);
            break;
        case JOBJ_VBILLBOARD:
            mkVBillBoardMtx(jobj, mtx, pmtx);
            break;
        case JOBJ_HBILLBOARD:
            mkHBillBoardMtx(jobj, mtx, pmtx);
            break;
        case JOBJ_RBILLBOARD:
            mkRBillBoardMtx(jobj, mtx, pmtx);
            break;
        default:
            HSD_Panic("displayfunc.c", 322, "unkown type of billboard.\n");
        }
    } else {
        MTXConcat(vmtx, jobj->mtx, pmtx);
    }
}

static inline HSD_JObj *HSD_JObjFindSkeleton(HSD_JObj *jobj)
{
    HSD_ASSERT(342, jobj);
    for (; jobj; jobj = jobj->parent) {
        if (jobj->flags & (JOBJ_SKELETON | JOBJ_SKELETON_ROOT)) {
            return jobj;
        }
    }
    return NULL;
}

MtxPtr _HSD_mkEnvelopeModelNodeMtx(HSD_JObj *m, MtxPtr mtx)
{
    if (m->flags & JOBJ_SKELETON_ROOT) {
        return NULL;
    } else {
        HSD_JObj *x = HSD_JObjFindSkeleton(m);
        HSD_ASSERT(422, x);

        if (x == m) {
            MTXInverse(x->envelopemtx, mtx);
        } else if (x->flags & JOBJ_SKELETON_ROOT) {
            HSD_MtxInverseConcat(x->mtx, m->mtx, mtx);
        } else {
            Mtx n;
            MTXConcat(x->mtx, x->envelopemtx, n);
            HSD_MtxInverseConcat(n, m->mtx, mtx);
        }

        return mtx;
    }
}

void HSD_JObjDispSub(HSD_JObj *jobj, MtxPtr vmtx, MtxPtr pmtx, HSD_TrspMask trsp_mask,
                     u32 rendermode)
{
    HSD_DObj *dobj;
    u32 dobj_trsp;

    HSD_JObjSetCurrent(jobj);

    dobj_trsp = trsp_mask << DOBJ_TRSP_SHIFT;

    if (!(rendermode & RENDER_SHADOW)) {
        if (jobj->flags & JOBJ_SPECULAR) {
            HSD_LObjSetupSpecularInit(pmtx);
        }
    }

    HSD_PObjClearMtxMark(NULL, 0);
    for (dobj = jobj->display_object; dobj; dobj = dobj->next) {
        if (dobj->flags & DOBJ_HIDDEN) {
            continue;
        }

        if (dobj->flags & dobj_trsp) {
            HSD_DObjSetCurrent(dobj);
            HSD_DOBJ_METHOD(dobj)->disp(dobj, vmtx, pmtx, rendermode);
        }
    }
    HSD_DObjSetCurrent(NULL);
    HSD_JObjSetCurrent(NULL);
}

void HSD_JObjDispDObj(HSD_JObj *jobj, MtxPtr vmtx, HSD_TrspMask trsp_mask,
                      u32 rendermode)
{
    HSD_CObj *cobj;
    Mtx mtx;
    u32 hidden = jobj->flags & JOBJ_HIDDEN;

    if (hidden == 0) {
        u32 xlu_bits = jobj->flags & (trsp_mask << JOBJ_TRSP_SHIFT);
        if (xlu_bits != 0) {
            HSD_JObjSetupMatrix(jobj);

            if (vmtx == NULL) {
                cobj = HSD_CObjGetCurrent();
                vmtx = HSD_CObjGetViewingMtxPtrDirect(cobj);
            }

            HSD_JOBJ_METHOD(jobj)->make_pmtx(jobj, vmtx, mtx);
            if ((xlu_bits & JOBJ_OPA) != 0) {
                HSD_JOBJ_METHOD(jobj)->disp(jobj, vmtx, mtx, HSD_TRSP_OPA, rendermode);
            }
            if (zsort_listing == 0) {
                if ((xlu_bits & JOBJ_TEXEDGE) != 0) {
                    HSD_JOBJ_METHOD(jobj)->disp(jobj, vmtx, mtx, HSD_TRSP_TEXEDGE,
                                                rendermode);
                }
                if ((xlu_bits & JOBJ_XLU) != 0) {
                    HSD_JOBJ_METHOD(jobj)->disp(jobj, vmtx, mtx, HSD_TRSP_XLU,
                                                rendermode);
                }
            } else {
                if ((xlu_bits & (JOBJ_TEXEDGE | JOBJ_XLU)) != 0) {
                    HSD_ZList *zlist;

                    zlist = HSD_ZListAlloc();
                    MTXCopy(mtx, zlist->pmtx);
                    if (vmtx != NULL) {
                        zlist->vmtx = (MtxPtr)HSD_MtxAlloc();
                        MTXCopy(vmtx, zlist->vmtx);
                    }
                    zlist->jobj = jobj;
                    zlist->rendermode = rendermode;
                    *zlist_bottom = zlist;
                    zlist_bottom = &zlist->next;
                    if ((xlu_bits & JOBJ_TEXEDGE) != 0) {
                        *zlist_texedge_bottom = zlist;
                        zlist_texedge_bottom = &zlist->sort.texedge;
                        zlist_texedge_nb += 1;
                    }
                    if ((xlu_bits & JOBJ_XLU) != 0) {
                        *zlist_xlu_bottom = zlist;
                        zlist_xlu_bottom = &zlist->sort.xlu;
                        zlist_xlu_nb += 1;
                    }
                }
            }
        }
    }
}

static HSD_ZList *zlist_sort(HSD_ZList *list, s32 nb, s32 offset)
{
    HSD_ZList *fore, *hind, **ptr;
    int nb_fore, nb_hind;
    int i;

    if (nb <= 1) {
        if (list != NULL) {
            ZLIST_NEXT(list, offset) = NULL;
        }
        return list;
    }

    nb_fore = nb / 2;
    nb_hind = nb - nb_fore;

    hind = list;
    for (i = 0; i < nb_fore; i++) {
        hind = ZLIST_NEXT(hind, offset);
    }

    fore = zlist_sort(list, nb_fore, offset);
    hind = zlist_sort(hind, nb_hind, offset);

    list = NULL;
    ptr = &list;

    while (fore != NULL && hind != NULL) {
        if (fore->pmtx[2][3] <= hind->pmtx[2][3]) {
            *ptr = fore;
            fore = ZLIST_NEXT(fore, offset);
        } else {
            *ptr = hind;
            hind = ZLIST_NEXT(hind, offset);
        }
        ptr = &ZLIST_NEXT(*ptr, offset);
    }

    if (fore != NULL) {
        *ptr = fore;
    } else if (hind != NULL) {
        *ptr = hind;
    }

    return list;
}

void _HSD_ZListSort(void)
{
    if (zsort_sorting) {
        zlist_texedge_top =
            zlist_sort(zlist_texedge_top, zlist_texedge_nb, ZLIST_LINK_OFFSET(texedge));
        zlist_xlu_top = zlist_sort(zlist_xlu_top, zlist_xlu_nb, ZLIST_LINK_OFFSET(xlu));
    }
}

static inline MtxPtr drawing_view_matrix(HSD_CObj *camera)
{
    return camera->view_mtx;
}
void _HSD_ZListDisp(void)
{
    MtxPtr vmtx;
    HSD_ZList *list;
    s32 has_view;
    HSD_CObj *cobj;

    cobj = HSD_CObjGetCurrent();
    vmtx = drawing_view_matrix(cobj);

    list = zlist_texedge_top;
    while (list != NULL) {
        has_view = list->vmtx != NULL;
        HSD_JOBJ_METHOD(list->jobj)
            ->disp(list->jobj, has_view ? list->vmtx : vmtx, list->pmtx,
                   HSD_TRSP_TEXEDGE, list->rendermode);
        list = list->sort.texedge;
    }

    list = zlist_xlu_top;
    while (list != NULL) {
        has_view = list->vmtx != NULL;
        HSD_JOBJ_METHOD(list->jobj)
            ->disp(list->jobj, has_view ? list->vmtx : vmtx, list->pmtx,
                   HSD_TRSP_XLU, list->rendermode);
        list = list->sort.xlu;
    }

    _HSD_ZListClear();
}

void _HSD_ZListClear(void)
{
    HSD_ZList *list = zlist_top;

    while (list != NULL) {
        HSD_ZList *next = list->next;
        if (list->vmtx) {
            HSD_MtxFree((Mtx *)list->vmtx);
        }
        HSD_ZListFree(list);
        list = next;
    }
    zlist_top = NULL;
    zlist_bottom = &zlist_top;

    zlist_texedge_top = NULL;
    zlist_texedge_bottom = &zlist_texedge_top;
    zlist_texedge_nb = 0;

    zlist_xlu_top = NULL;
    zlist_xlu_bottom = &zlist_xlu_top;
    zlist_xlu_nb = 0;
}

void HSD_SetZSortMode(s32 listing, s32 sorting)
{
    zsort_listing = listing;
    zsort_sorting = sorting;
}

void HSD_JObjDisp(HSD_JObj *jobj, MtxPtr vmtx, HSD_TrspMask trsp_mask, u32 rendermode)
{
    if (jobj != NULL) {
        if (union_type_dobj(jobj)) {
            HSD_JObjDispDObj(jobj, vmtx, trsp_mask, rendermode);
        } else if (union_type_ptcl(jobj) && sptcl_callback != NULL) {
            HSD_SList *sp;
            for (sp = jobj->display_object; sp != NULL; sp = sp->next) {
                if ((((u32)sp->data) & 0x80000000) != 0) {
                    u32 bank = JOBJ_PTCL_BANK_MASK & ((u32)sp->data);
                    u32 offset = (((u32)sp->data) >> JOBJ_PTCL_OFFSET_SHIFT) &
                                 JOBJ_PTCL_OFFSET_MASK;
                    (*sptcl_callback)(0, bank, offset, jobj);
                }
                sp->data = (void *)((u32)sp->data & JOBJ_PTCL_ACTIVE);
            }
        }
    }
}

void HSD_JObjSetSPtclCallback(void (*func)(s32, s32, s32, HSD_JObj *))
{
    sptcl_callback = func;
}



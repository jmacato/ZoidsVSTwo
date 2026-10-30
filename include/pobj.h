#ifndef ZOIDS_POBJ_H
#define ZOIDS_POBJ_H
/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/pobj.h. The
 * shipped ELF keeps animation in HSD_PObj and adds its update method. */
#include "class.h"
#include "matrix.h"
#include "aobj.h"
#include "list.h"
#include "gx_attr.h"
typedef struct HSD_PObj HSD_PObj;
typedef struct HSD_PObjInfo HSD_PObjInfo;
typedef struct HSD_PObjDesc HSD_PObjDesc;
typedef struct HSD_VtxDescList HSD_VtxDescList;
typedef struct HSD_Envelope HSD_Envelope;
typedef struct HSD_EnvelopeDesc HSD_EnvelopeDesc;
typedef struct HSD_ShapeSet HSD_ShapeSet;
typedef struct HSD_ShapeSetDesc HSD_ShapeSetDesc;
typedef struct HSD_ShapeAnim HSD_ShapeAnim;
typedef struct HSD_ShapeAnimJoint HSD_ShapeAnimJoint;
typedef struct HSD_ShapeAnimDObj HSD_ShapeAnimDObj;
typedef HSD_JObjDesc HSD_Joint;
struct HSD_PObj {
    HSD_Class parent;
    HSD_PObj *next;
    HSD_VtxDescList *verts;
    u16 flags;
    u16 n_display;
    u8 *display;
    union HSD_PObjUnion {
        HSD_JObj *jobj;
        HSD_ShapeSet *shape_set;
        HSD_SList *envelope_list;
    } u;
    HSD_AObj *aobj;
};

typedef u8 HSD_DisplayList;

typedef u8 HSD_VertexArray;

struct HSD_PObjDesc {
    char *class_name;
    HSD_PObjDesc *next;
    HSD_VtxDescList *verts;
    u16 flags;
    u16 n_display;
    HSD_DisplayList *display;
    union HSD_PObjDesc_u {
        HSD_Joint *joint;
        HSD_ShapeSetDesc *shape_set;
        HSD_EnvelopeDesc **envelope_p;
    } u;
};

struct HSD_VtxDescList {
    GXAttr attr;
    GXAttrType attr_type;
    GXCompCnt comp_cnt;
    GXCompType comp_type;
    u8 frac;
    u16 stride;
    HSD_VertexArray *vertex;
};

struct HSD_Envelope {
    HSD_Envelope *next;
    HSD_JObj *jobj;
    f32 weight;
};

struct HSD_EnvelopeDesc {
    HSD_Joint *joint;
    f32 weight;
};

struct HSD_ShapeSet {
    u16 flags;
    u16 nb_shape;
    int nb_vertex_index;
    HSD_VtxDescList *vertex_desc;
    u8 **vertex_idx_list;
    s32 nb_normal_index;
    HSD_VtxDescList *normal_desc;
    u8 **normal_idx_list;
    union HSD_ShapeSet_blend {
        f32 *bp;
        f32 bl;
    } blend;
};

struct HSD_ShapeSetDesc {
    u16 flags;
    u16 nb_shape;
    s32 nb_vertex_index;
    HSD_VtxDescList *vertex_desc;
    u8 **vertex_idx_list;
    s32 nb_normal_index;
    HSD_VtxDescList *normal_desc;
    u8 **normal_idx_list;
};

struct HSD_ShapeAnim {
    HSD_ShapeAnim *next;
    HSD_AObjDesc *aobjdesc;
};

struct HSD_ShapeAnimJoint {
    HSD_ShapeAnimJoint *child;
    HSD_ShapeAnimJoint *next;
    HSD_ShapeAnimDObj *shapeanimdobj;
};

struct HSD_PObjInfo {
    HSD_ClassInfo parent;
    void (*disp)(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
    void (*setup_mtx)(HSD_PObj *pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
    s32 (*load)(HSD_PObj *pobj, HSD_PObjDesc *desc);
    AnimationUpdate update;
};

extern void HSD_PObjClearMtxMark(void *, u32);
extern HSD_PObjInfo hsdPObj;
extern HSD_PObj *HSD_PObjAlloc(void);
extern HSD_PObj *HSD_PObjLoadDesc(HSD_PObjDesc *);
extern void HSD_PObjRemoveAll(HSD_PObj *);
extern void HSD_PObjRemoveAnimAllByFlags(HSD_PObj *, u32);
extern void HSD_PObjAddAnimAll(HSD_PObj *, HSD_ShapeAnim *);
extern void HSD_PObjReqAnimAllByFlags(HSD_PObj *, f32, u32);
extern void HSD_PObjAnimAll(HSD_PObj *);
extern void HSD_PObjResolveRefsAll(HSD_PObj *, HSD_PObjDesc *);
extern void HSD_PObjDisp(HSD_PObj *, Mtx, Mtx, u32);
#endif

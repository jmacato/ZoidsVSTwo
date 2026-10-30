#ifndef ZOIDS_TEXP_H
#define ZOIDS_TEXP_H
#include "types.h"
#include "matrix.h"
#include "gx_tev.h"
typedef union HSD_TExp HSD_TExp;
typedef struct HSD_TObj {
    struct HSD_ClassInfo *class_info;
    u16 reference_count;
    u16 individual_reference_count;
    struct HSD_TObj *next;
    GXTexMapID id;
    s32 src;
    u32 mtxid;
    Quaternion rotate;
    Vec3 scale;
    Vec3 translate;
    s32 wrap_s;
    s32 wrap_t;
    u8 repeat_s;
    u8 repeat_t;
    /* Descriptor ID remains separate from the assigned hardware map ID. */
    u16 original_id;
    u32 flags;
    f32 blending;
    s32 magFilt;
    struct HSD_ImageDesc *imagedesc;
    struct HSD_Tlut *tlut;
    struct HSD_TexLODDesc *lod;
    struct HSD_AObj *aobj;
    struct HSD_ImageDesc **imagetbl;
    struct HSD_Tlut **tluttbl;
    u8 tlut_no;
    Mtx mtx;
    GXTexCoordID coord;
    struct HSD_TObjTev *tev;
} HSD_TObj;
#define GX_ENABLE 1
#define GX_DISABLE 0
#define GX_TEV_SUB 1
#define HSD_TEXP_RAS ((HSD_TExp *)-2)
#define HSD_TEXP_TEX ((HSD_TExp *)-1)
#define HSD_TEXP_ZERO ((HSD_TExp *)0)

#define TEVCONF_MODE 1

typedef enum _HSD_TEInput {
    HSD_TE_END = 0,
    HSD_TE_RGB = 1,
    HSD_TE_R = 2,
    HSD_TE_G = 3,
    HSD_TE_B = 4,
    HSD_TE_A = 5,
    HSD_TE_X = 6,
    HSD_TE_0 = 7,
    HSD_TE_1 = 8,
    HSD_TE_1_8 = 9,
    HSD_TE_2_8 = 10,
    HSD_TE_3_8 = 11,
    HSD_TE_4_8 = 12,
    HSD_TE_5_8 = 13,
    HSD_TE_6_8 = 14,
    HSD_TE_7_8 = 15,
    HSD_TE_INPUT_MAX = 16,
    HSD_TE_UNDEF = 0xFF
} HSD_TEInput;

typedef enum _HSD_TEType {
    HSD_TE_U8 = 0,
    HSD_TE_U16 = 1,
    HSD_TE_U32 = 2,
    HSD_TE_F32 = 3,
    HSD_TE_F64 = 4,
    HSD_TE_COMP_TYPE_MAX = 5
} HSD_TEType;

typedef enum _HSD_TExpType {
    HSD_TE_ZERO = 0,
    HSD_TE_TEV = 1,
    HSD_TE_TEX = 2,
    HSD_TE_RAS = 3,
    HSD_TE_CNST = 4,
    HSD_TE_IMM = 5,
    HSD_TE_KONST = 6,
    HSD_TE_ALL = 7,
    HSD_TE_TYPE_MAX = 8
} HSD_TExpType;

typedef struct _HSD_TECommon {
    HSD_TExpType type;
    HSD_TExp *next;
} HSD_TECommon;

typedef struct _HSD_TECnst {
    HSD_TExpType type;
    HSD_TExp *next;
    void *val;
    u8 comp;
    u8 ctype;
    u8 reg;
    u8 idx;
    u8 ref;
    u8 range;
} HSD_TECnst;

typedef struct _HSD_TEArg {
    u8 type;
    u8 sel;
    u8 arg;
    HSD_TExp *exp;
} HSD_TEArg;

typedef struct _HSD_TETev {
    HSD_TExpType type;
    HSD_TExp *next;
    s32 c_ref;
    u8 c_dst;
    u8 c_op;
    u8 c_clamp;
    u8 c_bias;
    u8 c_scale;
    u8 c_range;
    s32 a_ref;
    u8 a_dst;
    u8 a_op;
    u8 a_clamp;
    u8 a_bias;
    u8 a_scale;
    u8 a_range;
    u8 tex_swap;
    u8 ras_swap;
    u8 kcsel;
    u8 kasel;
    HSD_TEArg c_in[4];
    HSD_TEArg a_in[4];
    HSD_TObj *tex;
    u8 chan;
} HSD_TETev;

union HSD_TExp {
    HSD_TExpType type;
    struct _HSD_TECommon comm;
    struct _HSD_TETev tev;
    struct _HSD_TECnst cnst;
};

extern HSD_TExpType HSD_TExpGetType(HSD_TExp *expression);
extern void HSD_TExpRef(HSD_TExp *expression, u8 selector);
extern void HSD_TExpUnref(HSD_TExp *expression, u8 selector);
typedef struct HSD_TExpRes {
    int failed;
    int texmap;
    int cnst_remain;
    struct HSD_TExpRes_reg {
        u8 color;
        u8 alpha;
    } reg[8];
    u8 c_ref[4];
    u8 a_ref[4];
    u8 c_use[4];
    u8 a_use[4];
} HSD_TExpRes;

#define GX_TEV_ADD 0
typedef struct _HSD_TevConf {
    GXTevOp clr_op;
    GXTevColorArg clr_a;
    GXTevColorArg clr_b;
    GXTevColorArg clr_c;
    GXTevColorArg clr_d;
    GXTevScale clr_scale;
    GXTevBias clr_bias;
    u8 clr_clamp;
    GXTevRegID clr_out_reg;
    GXTevOp alpha_op;
    GXTevAlphaArg alpha_a;
    GXTevAlphaArg alpha_b;
    GXTevAlphaArg alpha_c;
    GXTevAlphaArg alpha_d;
    GXTevScale alpha_scale;
    GXTevBias alpha_bias;
    u8 alpha_clamp;
    GXTevRegID alpha_out_reg;
    GXTevClampMode mode;
    GXTevSwapSel ras_swap;
    GXTevSwapSel tex_swap;
    GXTevKColorSel kcsel;
    GXTevKAlphaSel kasel;
} HSD_TevConf;

typedef struct _HSD_TevDesc {
    struct _HSD_TevDesc *next;
    u32 flags;
    u32 stage;
    u32 coord;
    u32 map;
    u32 color;
    union _HSD_TevDesc_u {
        HSD_TevConf tevconf;
        struct _HSD_TevDesc_u_tevop {
            u32 tevmode;
        } tevop;
    } u;
} HSD_TevDesc;

typedef struct _HSD_TExpTevDesc {
    struct _HSD_TevDesc desc;
    HSD_TObj *tobj;
} HSD_TExpTevDesc;

#define GX_TRUE 1
#define GX_FALSE 0
typedef struct HSD_TExpDag HSD_TExpDag;
struct HSD_TExpDag {
    struct _HSD_TETev *tev;
    u8 idx;
    u8 nb_dep;
    u8 nb_ref;
    u8 dist;
    HSD_TExpDag *depend[8];
};

extern s32 HSD_TExpSimplify(HSD_TExp *expression);
extern s32 HSD_TExpSimplify2(HSD_TExp *expression);
extern s32 HSD_TExpMakeDag(HSD_TExp *expression, HSD_TExpDag *list);
extern void HSD_TExpSchedule(s32 count, HSD_TExpDag *list, HSD_TExp **order,
                             HSD_TExpRes *resource);
extern GXTevStageID HSD_Index2TevStage(s32 index);
extern GXTevStageID HSD_StateAssignTev(void);
extern void HSD_SetupTevStage(HSD_TevDesc *description);
extern void GXPixModeSync(void);
extern void GXSetTevKColor(GXTevKColorID id, GXColor color);
extern void GXSetTevColor(GXTevRegID id, GXColor color);
extern void HSD_StateInvalidate(u32 flags);
extern HSD_TExp *HSD_TExpTev(HSD_TExp **list);
extern HSD_TExp *HSD_TExpCnst(void *value, HSD_TEInput component, HSD_TEType type,
                              HSD_TExp **list);
extern HSD_TExp *HSD_TExpFreeList(HSD_TExp *list, HSD_TExpType type, s32 all);
extern void HSD_TExpColorOp(HSD_TExp *expression, GXTevOp operation, GXTevBias bias,
                            GXTevScale scale, u8 clamp);
extern void HSD_TExpAlphaOp(HSD_TExp *expression, GXTevOp operation, GXTevBias bias,
                            GXTevScale scale, u8 clamp);
extern void HSD_TExpColorIn(HSD_TExp *expression, HSD_TEInput select_a, HSD_TExp *a,
                            HSD_TEInput select_b, HSD_TExp *b, HSD_TEInput select_c,
                            HSD_TExp *c, HSD_TEInput select_d, HSD_TExp *d);
extern void HSD_TExpAlphaIn(HSD_TExp *expression, HSD_TEInput select_a, HSD_TExp *a,
                            HSD_TEInput select_b, HSD_TExp *b, HSD_TEInput select_c,
                            HSD_TExp *c, HSD_TEInput select_d, HSD_TExp *d);
extern void HSD_TExpOrder(HSD_TExp *expression, HSD_TObj *texture, GXChannelID channel);
extern void HSD_TExpSetReg(HSD_TExp *constants);
extern void HSD_TExpSetupTev(HSD_TExpTevDesc *description, HSD_TExp *constants);
extern s32 HSD_TExpCompile(HSD_TExp *expression, HSD_TExpTevDesc **description,
                           HSD_TExp **list);
extern void HSD_TExpFreeTevDesc(HSD_TExpTevDesc *description);
#endif

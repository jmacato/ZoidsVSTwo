extern "C" {
/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/state.c. */
#include "state.h"
#include "lobj.h"
#define NULL 0
#define GX_PASSCLR 4
#define HSD_STATE_NONE 0
#define HSD_STATE_PRIMITIVE 1
#define HSD_STATE_VTX_ATTR 2
#define HSD_STATE_COLOR_CHANNEL 4
#define HSD_STATE_TEV_STAGE 8
#define HSD_STATE_TEV_REGISTER 16
#define HSD_STATE_TEX_COORD_GEN 32
#define HSD_STATE_RENDER_MODE 64
#define RENDER_DIFFUSE_BITS 3
#define RENDER_DIFFUSE_MAT0 0
#define RENDER_DIFFUSE_MAT 1
#define RENDER_DIFFUSE_VTX 2
#define RENDER_DIFFUSE 4
#define RENDER_SPECULAR 8
#define RENDER_ALPHA_SHIFT 13
#define RENDER_ALPHA_BITS (3 << RENDER_ALPHA_SHIFT)
#define RENDER_ALPHA_COMPAT (0 << RENDER_ALPHA_SHIFT)
#define RENDER_ALPHA_VTX (2 << RENDER_ALPHA_SHIFT)
#define RENDER_ZMODE_ALWAYS (1 << 27)
#define RENDER_NO_ZUPDATE (1 << 29)
#define RENDER_XLU (1 << 30)
extern void HSD_MulColor(GXColor *, GXColor *, GXColor *);
extern void HSD_DisableChannelLighting(GXChannelID);
extern void GXSetCullMode(s32);
extern void GXSetBlendMode(s32, s32, s32, s32);
extern void GXSetZMode(u8, s32, u8);
extern void GXSetAlphaCompare(s32, u8, s32, s32, u8);
extern void GXSetColorUpdate(u8);
extern void GXSetAlphaUpdate(u8);
extern void GXSetDstAlpha(u8, u8);
extern void GXSetZCompLoc(u8);
extern void GXSetDither(u8);
extern void HSD_ClearVtxDesc(void);
extern void _HSD_StateInvalidateColorChannel(void);
extern void _HSD_StateInvalidateTevStage(void);
extern void _HSD_StateInvalidateTevRegister(void);
extern void _HSD_StateInvalidateTexCoordGen(void);
void HSD_SetupPEMode(u32, HSD_PEDesc *);
void HSD_StateSetBlendMode(s32, s32, s32, s32);
void HSD_StateSetZMode(s32, s32, s32);
void HSD_StateSetAlphaCompare(s32, u8, s32, s32, u8);
void HSD_StateSetColorUpdate(s32);
void HSD_StateSetAlphaUpdate(s32);
void HSD_StateSetDstAlpha(s32, u8);
void HSD_StateSetZCompLoc(s32);
void HSD_StateSetDither(s32);
static u8 state_line_width;
static u8 state_point_size;
static int state_cull_mode;
static int state_blend_type;
static int state_src_factor;
static int state_dst_factor;
static int state_logic_op;
static u8 state_z_enable;
static int state_z_func;
static u8 state_z_update;
static int state_alpha_comp0;
static u8 state_alpha_ref0;
static int state_alpha_op;
static int state_alpha_comp1;
static u8 state_alpha_ref1;
static u8 state_color_update;
static u8 state_alpha_update;
static u8 state_enable_dst_alpha;
static u8 state_dst_alpha;
static u8 state_before_tex;
static u8 state_dither;

static struct matstate_t {
    GXColor ambient;
    GXColor diffuse;
    GXColor specular;
    u8 alpha;
    f32 shininess;
} matstate;

void HSD_SetupChannelMode(u32 flags)
{
    s32 color_source = flags & RENDER_DIFFUSE_BITS;
    s32 alpha_source;
    s32 specular = 0;
    s32 vertex_alpha = 0;
    if (color_source == RENDER_DIFFUSE_MAT0)
        color_source = RENDER_DIFFUSE_MAT;
    alpha_source = flags & RENDER_ALPHA_BITS;
    if (alpha_source == RENDER_ALPHA_COMPAT)
        alpha_source = color_source << RENDER_ALPHA_SHIFT;
    if (flags & RENDER_SPECULAR) {
        static HSD_Chan ch = {
            NULL, GX_COLOR1, 0, {0, 0, 0, 0}, {255, 255, 255, 255}, 1, 0, 0,
            0,    2,         0, NULL};
        int i;
        int count;
        ch.light_mask = HSD_LObjGetLightMaskSpecular();
        HSD_SetupChannel(&ch);
        specular = 1;
        count = HSD_LObjGetNbActive();
        for (i = 0; i < count; i++) {
            HSD_LObj *light = HSD_LObjGetActiveByIndex(i);
            if (light != NULL)
                HSD_LObjSetup(light, light->color, matstate.shininess);
        }
    }
    if (flags & RENDER_DIFFUSE) {
        static HSD_Chan ch = {
            NULL, GX_COLOR0, 0, {0, 0, 0, 0}, {255, 255, 255, 255}, 1, 0, 0,
            0,    2,         1, NULL};
        static HSD_Chan ch_a = {NULL, GX_ALPHA0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 1, 0, 0,
                                0,    2,         1, NULL};
        static HSD_Chan ch_vtx_a = {
            NULL, GX_ALPHA0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0, 1, 0, 2, 1, NULL};
        static GXColor dark_matter = {0, 0, 0, 255};
        HSD_LObj *light = HSD_LObjGetActiveByID(GX_MAX_LIGHT);
        u8 ambient_alpha;
        if (light != NULL && (light->flags & LOBJ_DIFFUSE))
            HSD_MulColor(&matstate.ambient, &light->color, &ch.amb_color);
        else
            ch.amb_color = dark_matter;
        ch.mat_src = (color_source & RENDER_DIFFUSE_VTX) ? 1 : 0;
        ch.light_mask = HSD_LObjGetLightMaskDiffuse();
        HSD_SetupChannel(&ch);
        if (alpha_source & RENDER_ALPHA_VTX) {
            ch_a.chan = GX_ALPHA1;
            vertex_alpha = 1;
            HSD_SetupChannel(&ch_vtx_a);
        } else
            ch_a.chan = GX_ALPHA0;
        ch_a.light_mask = HSD_LObjGetLightMaskAlpha();
        if (light != NULL && (light->flags & LOBJ_ALPHA))
            ambient_alpha = light->color.a;
        else
            ambient_alpha = 0;
        if (ch_a.light_mask != 0) {
            ch_a.enable = 1;
            ch_a.mat_color.a = 255;
            ch_a.amb_color.a = ambient_alpha;
        } else {
            ch_a.enable = 0;
            ch_a.mat_color.a = ambient_alpha;
        }
        HSD_SetupChannel(&ch_a);
    } else {
        static HSD_Chan ch = {
            NULL, GX_COLOR0, 0, {0, 0, 0, 0}, {255, 255, 255, 255}, 0, 0, 1,
            0,    0,         2, NULL};
        static HSD_Chan ch_a = {
            NULL, GX_ALPHA0, 0, {0, 0, 0, 0}, {0, 0, 0, 255}, 0, 0, 1, 0, 0, 2, NULL};
        ch.mat_src = (color_source & RENDER_DIFFUSE_VTX) ? 1 : 0;
        HSD_SetupChannel(&ch);
        ch_a.mat_src = (alpha_source & RENDER_ALPHA_VTX) ? 1 : 0;
        HSD_SetupChannel(&ch_a);
    }
    if (specular != 0) {
        if (vertex_alpha == 0)
            HSD_DisableChannelLighting(GX_ALPHA1);
        HSD_StateSetNumChans(2);
    } else if (vertex_alpha != 0) {
        HSD_DisableChannelLighting(GX_COLOR1);
        HSD_StateSetNumChans(2);
    } else {
        HSD_DisableChannelLighting(GX_COLOR1A1);
        HSD_StateSetNumChans(1);
    }
}

void HSD_SetupPEMode(u32 flags, HSD_PEDesc *pe)
{
    bool depth_update;
    int blend_mode;

    if (pe != NULL) {
        HSD_StateSetColorUpdate(pe->flags & 1);
        HSD_StateSetAlphaUpdate(pe->flags & 2);
        HSD_StateSetDstAlpha(pe->flags & 4, pe->dst_alpha);
        HSD_StateSetBlendMode(pe->type, pe->src_factor, pe->dst_factor, pe->logic_op);
        HSD_StateSetZMode(pe->flags & 0x10, pe->z_comp, pe->flags & 0x20);
        HSD_StateSetZCompLoc(pe->flags & 8);
        HSD_StateSetAlphaCompare(pe->alpha_comp0, pe->ref0, pe->alpha_op,
                                 pe->alpha_comp1, pe->ref1);
        HSD_StateSetDither(pe->flags & 0x40);
        return;
    }
    HSD_StateSetColorUpdate(1);
    HSD_StateSetAlphaUpdate(0);
    HSD_StateSetDstAlpha(0, 0U);
    if (flags & RENDER_XLU) {
        blend_mode = 1;
    } else {
        blend_mode = 0;
    }
    HSD_StateSetBlendMode(blend_mode, 4U, 5U, 0xFU);
    depth_update = (flags & RENDER_NO_ZUPDATE) ? false : true;
    HSD_StateSetZMode(1, (flags & RENDER_ZMODE_ALWAYS) ? 7 : 3, depth_update);
    if (!(flags & RENDER_NO_ZUPDATE) && (flags & RENDER_XLU)) {
        HSD_StateSetZCompLoc(0);
        HSD_StateSetAlphaCompare(4U, 0U, 0U, 4U, 0U);
    } else {
        HSD_StateSetZCompLoc(1);
        HSD_StateSetAlphaCompare(7U, 0U, 0U, 7U, 0U);
    }
    HSD_StateSetDither(0);
}

static inline void setupTevMode_last(void)
{
    if (!HSD_StateGetNumTevStages()) {
        HSD_TevDesc tevdesc;
        tevdesc.flags = 0;
        tevdesc.stage = HSD_StateAssignTev();
        tevdesc.coord = GX_TEXCOORD_NULL;
        tevdesc.map = GX_TEXMAP_NULL;
        tevdesc.color = GX_COLOR0A0;
        tevdesc.u.tevop.tevmode = GX_PASSCLR;
        HSD_SetupTevStage(&tevdesc);
    }
}

void HSD_SetupRenderModeWithCustomPE(u32 rendermode, HSD_PEDesc *pe)
{
    setupTevMode_last();
    HSD_SetupPEMode(rendermode, pe);
    HSD_SetTevRegAll();
    HSD_StateSetNumTevStages();
    HSD_StateSetNumTexGens();
    HSD_SetupChannelMode(rendermode);
}

void HSD_SetMaterialColor(GXColor ambient, GXColor diffuse, GXColor specular, f32 alpha)
{
    matstate.ambient = ambient;
    matstate.diffuse = diffuse;
    matstate.specular = specular;
    matstate.alpha = 255.0 * alpha;
}

void HSD_SetMaterialShininess(f32 shininess)
{
    matstate.shininess = shininess;
}

void HSD_StateSetCullMode(int mode)
{
    if (state_cull_mode != mode) {
        GXSetCullMode(mode);
        state_cull_mode = mode;
    }
}

void HSD_StateSetBlendMode(int type, int src_factor, int dst_factor, int op)
{
    if (state_blend_type != type || state_src_factor != src_factor ||
        state_dst_factor != dst_factor || state_logic_op != op) {
        GXSetBlendMode(type, src_factor, dst_factor, op);
        state_blend_type = type;
        state_src_factor = src_factor;
        state_dst_factor = dst_factor;
        state_logic_op = op;
    }
}

void HSD_StateSetZMode(int enable, int function, int write_depth)
{
    bool enabled = enable ? true : false;
    bool update = write_depth ? true : false;

    if (state_z_enable != (u8)enabled || state_z_func != function ||
        state_z_update != (u8)update) {
        GXSetZMode(enabled, function, update);
        state_z_enable = enabled;
        state_z_func = function;
        state_z_update = update;
    }
}

void HSD_StateSetAlphaCompare(int function_0, u8 reference_0, int operation, int function_1, u8 reference_1)
{
    if (state_alpha_comp0 != function_0 || state_alpha_ref0 != reference_0 ||
        state_alpha_op != operation || state_alpha_comp1 != function_1 ||
        state_alpha_ref1 != reference_1) {
        GXSetAlphaCompare(function_0, reference_0, operation, function_1, reference_1);
        state_alpha_comp0 = function_0;
        state_alpha_ref0 = reference_0;
        state_alpha_op = operation;
        state_alpha_comp1 = function_1;
        state_alpha_ref1 = reference_1;
    }
}

void HSD_StateSetColorUpdate(int enable)
{
    bool enabled = enable ? true : false;
    if (state_color_update != (u8)enabled) {
        GXSetColorUpdate(enabled);
        state_color_update = enabled;
    }
}

void HSD_StateSetAlphaUpdate(int enable)
{
    bool enabled = enable ? true : false;
    if (state_alpha_update != (u8)enabled) {
        GXSetAlphaUpdate(enabled);
        state_alpha_update = enabled;
    }
}

void HSD_StateSetDstAlpha(int enable, u8 alpha)
{
    bool enabled = enable ? true : false;
    if (state_enable_dst_alpha != (u8)enabled || state_dst_alpha != alpha) {
        GXSetDstAlpha(enabled, alpha);
        state_enable_dst_alpha = enabled;
        state_dst_alpha = alpha;
    }
}

void HSD_StateSetZCompLoc(int before_texture)
{
    bool enabled = before_texture ? true : false;
    if (state_before_tex != (u8)enabled) {
        GXSetZCompLoc(enabled);
        state_before_tex = (u8)enabled;
    }
}

void HSD_StateSetDither(int enable)
{
    bool enabled = enable ? true : false;
    if (state_dither != (u8)enabled) {
        GXSetDither(enabled);
        state_dither = enabled;
    }
}

void _HSD_StateInvalidatePrimitive(void)
{
    state_point_size = 0;
    state_cull_mode = -1;
    state_line_width = 255;
}

void _HSD_StateInvalidateVtxAttr(void)
{
    HSD_ClearVtxDesc();
}

void _HSD_StateInvalidateRenderMode(void)
{
    state_blend_type = -1;
    state_src_factor = -1;
    state_dst_factor = -1;
    state_logic_op = -1;
    state_z_enable = -1;
    state_z_func = -1;
    state_z_update = -1;
    state_alpha_comp0 = -1;
    state_alpha_ref0 = 0;
    state_alpha_op = -1;
    state_alpha_comp1 = -1;
    state_alpha_ref1 = 0;
    state_color_update = -1;
    state_alpha_update = -1;
    state_enable_dst_alpha = -1;
    state_dst_alpha = 0;
    state_before_tex = -1;
    state_dither = -1;
}

void HSD_StateInvalidate(u32 mask)
{
    struct invalidate_funcs_t {
        int mask;
        void (*func)(void);
    };
    static struct invalidate_funcs_t invalidate_funcs[] = {
        {HSD_STATE_PRIMITIVE, _HSD_StateInvalidatePrimitive},
        {HSD_STATE_VTX_ATTR, _HSD_StateInvalidateVtxAttr},
        {HSD_STATE_COLOR_CHANNEL, _HSD_StateInvalidateColorChannel},
        {HSD_STATE_TEV_STAGE, _HSD_StateInvalidateTevStage},
        {HSD_STATE_TEV_REGISTER, _HSD_StateInvalidateTevRegister},
        {HSD_STATE_TEX_COORD_GEN, _HSD_StateInvalidateTexCoordGen},
        {HSD_STATE_RENDER_MODE, _HSD_StateInvalidateRenderMode},
        {HSD_STATE_NONE}};

    int i;
    for (i = 0; invalidate_funcs[i].mask != HSD_STATE_NONE; i++) {
        if (mask & invalidate_funcs[i].mask) {
            invalidate_funcs[i].func();
        }
    }
}
}

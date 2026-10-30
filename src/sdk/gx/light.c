/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXLight.c. */
#include "gx_light.h"
#include "gx_state.h"
extern f32 cosf(f32 value);
// GXLightObj private data
struct __GXLightObjInt_struct {
    u32 reserved[3];
    u32 Color;
    f32 a[3];
    f32 k[3];
    f32 lpos[3];
    f32 ldir[3];
};

void GXInitLightAttn(GXLightObj *lt_obj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2)
{
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXInitLightSpot(GXLightObj *lt_obj, f32 cutoff, s32 spot_func)
{
    float a0, a1, a2;
    float r;
    float d;
    float cr;
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    if (cutoff <= 0.0f || cutoff > 90.0f)
        spot_func = GX_SP_OFF;

    r = (3.1415927f * cutoff) / 180.0f;
    cr = cosf(r);
    switch (spot_func) {
    case GX_SP_FLAT:
        a0 = -1000.0f * cr;
        a1 = 1000.0f;
        a2 = 0.0f;
        break;
    case GX_SP_COS:
        a0 = -cr / (1.0f - cr);
        a1 = 1.0f / (1.0f - cr);
        a2 = 0.0f;
        break;
    case GX_SP_COS2:
        a0 = 0.0f;
        a1 = -cr / (1.0f - cr);
        a2 = 1.0f / (1.0f - cr);
        break;
    case GX_SP_SHARP:
        d = (1.0f - cr) * (1.0f - cr);
        a0 = (cr * (cr - 2.0f)) / d;
        a1 = 2.0f / d;
        a2 = -1.0f / d;
        break;
    case GX_SP_RING1:
        d = (1.0f - cr) * (1.0f - cr);
        a0 = (-4.0f * cr) / d;
        a1 = (4.0f * (1.0f + cr)) / d;
        a2 = -4.0f / d;
        break;
    case GX_SP_RING2:
        d = (1.0f - cr) * (1.0f - cr);
        a0 = 1.0f - ((2.0f * cr * cr) / d);
        a1 = (4.0f * cr) / d;
        a2 = -2.0f / d;
        break;
    case GX_SP_OFF:
    default:
        a0 = 1.0f;
        a1 = 0.0f;
        a2 = 0.0f;
        break;
    }
    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
}

void GXInitLightDistAttn(GXLightObj *lt_obj, f32 ref_dist, f32 ref_br, s32 dist_func)
{
    f32 k0, k1, k2;
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    if (ref_dist < 0.0f)
        dist_func = GX_DA_OFF;
    if (ref_br <= 0.0f || ref_br >= 1.0f)
        dist_func = GX_DA_OFF;

    switch (dist_func) {
    case GX_DA_GENTLE:
        k0 = 1.0f;
        k1 = (1.0f - ref_br) / (ref_br * ref_dist);
        k2 = 0.0f;
        break;
    case GX_DA_MEDIUM:
        k0 = 1.0f;
        k1 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist);
        k2 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist * ref_dist);
        break;
    case GX_DA_STEEP:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = (1.0f - ref_br) / (ref_br * ref_dist * ref_dist);
        break;
    case GX_DA_OFF:
    default:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = 0.0f;
        break;
    }

    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXInitLightPos(GXLightObj *lt_obj, f32 x, f32 y, f32 z)
{
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    obj->lpos[0] = x;
    obj->lpos[1] = y;
    obj->lpos[2] = z;
}

void GXInitLightDir(GXLightObj *lt_obj, f32 nx, f32 ny, f32 nz)
{
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    obj->ldir[0] = -nx;
    obj->ldir[1] = -ny;
    obj->ldir[2] = -nz;
}

void GXInitLightColor(GXLightObj *lt_obj, GXColor color)
{
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    obj->Color = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
}

void GXLoadLightObjImm(GXLightObj *lt_obj, GXLightID light)
{
    unsigned long addr;
    unsigned long idx;
    struct __GXLightObjInt_struct *obj;

    obj = (struct __GXLightObjInt_struct *)lt_obj;

    switch (light) {
    case GX_LIGHT0:
        idx = 0;
        break;
    case GX_LIGHT1:
        idx = 1;
        break;
    case GX_LIGHT2:
        idx = 2;
        break;
    case GX_LIGHT3:
        idx = 3;
        break;
    case GX_LIGHT4:
        idx = 4;
        break;
    case GX_LIGHT5:
        idx = 5;
        break;
    case GX_LIGHT6:
        idx = 6;
        break;
    case GX_LIGHT7:
        idx = 7;
        break;
    default:
        idx = 0;

        break;
    }

    addr = idx * 0x10 + 0x600;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(addr | 0xF0000);

    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(obj->Color);
    GX_WRITE_F32(obj->a[0]);
    GX_WRITE_F32(obj->a[1]);
    GX_WRITE_F32(obj->a[2]);
    GX_WRITE_F32(obj->k[0]);
    GX_WRITE_F32(obj->k[1]);
    GX_WRITE_F32(obj->k[2]);
    GX_WRITE_F32(obj->lpos[0]);
    GX_WRITE_F32(obj->lpos[1]);
    GX_WRITE_F32(obj->lpos[2]);
    GX_WRITE_F32(obj->ldir[0]);
    GX_WRITE_F32(obj->ldir[1]);
    GX_WRITE_F32(obj->ldir[2]);

    gx->bpSent = 1;
}

void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color)
{
    u32 reg = 0;
    u32 colIdx;
    u32 alpha;

    switch (chan) {
    case GX_COLOR0:
        alpha = gx->ambColor[0] & 0xFF;
        GX_SET_FIELD(reg, 8, 0, alpha);
        GX_SET_FIELD(reg, 8, 8, amb_color.b);
        GX_SET_FIELD(reg, 8, 16, amb_color.g);
        GX_SET_FIELD(reg, 8, 24, amb_color.r);
        colIdx = 0;
        break;
    case GX_COLOR1:
        alpha = gx->ambColor[1] & 0xFF;
        GX_SET_FIELD(reg, 8, 0, alpha);
        GX_SET_FIELD(reg, 8, 8, amb_color.b);
        GX_SET_FIELD(reg, 8, 16, amb_color.g);
        GX_SET_FIELD(reg, 8, 24, amb_color.r);
        colIdx = 1;
        break;
    case GX_ALPHA0:
        reg = gx->ambColor[0];
        GX_SET_FIELD(reg, 8, 0, amb_color.a);
        colIdx = 0;
        break;
    case GX_ALPHA1:
        reg = gx->ambColor[1];
        GX_SET_FIELD(reg, 8, 0, amb_color.a);
        colIdx = 1;
        break;
    case GX_COLOR0A0:
        GX_SET_FIELD(reg, 8, 0, amb_color.a);
        GX_SET_FIELD(reg, 8, 8, amb_color.b);
        GX_SET_FIELD(reg, 8, 16, amb_color.g);
        GX_SET_FIELD(reg, 8, 24, amb_color.r);
        colIdx = 0;
        break;
    case GX_COLOR1A1:
        GX_SET_FIELD(reg, 8, 0, amb_color.a);
        GX_SET_FIELD(reg, 8, 8, amb_color.b);
        GX_SET_FIELD(reg, 8, 16, amb_color.g);
        GX_SET_FIELD(reg, 8, 24, amb_color.r);
        colIdx = 1;
        break;
    default:

        return;
    }

    GX_WRITE_XF_REG(colIdx + 10, reg);
    gx->bpSent = 1;
    gx->ambColor[colIdx] = reg;
}

void GXSetChanMatColor(GXChannelID chan, GXColor mat_color)
{
    u32 reg = 0;
    u32 alpha;
    u32 colIdx;

    switch (chan) {
    case GX_COLOR0:
        alpha = gx->matColor[0] & 0xFF;
        GX_SET_FIELD(reg, 8, 0, alpha);
        GX_SET_FIELD(reg, 8, 8, mat_color.b);
        GX_SET_FIELD(reg, 8, 16, mat_color.g);
        GX_SET_FIELD(reg, 8, 24, mat_color.r);
        colIdx = 0;
        break;
    case GX_COLOR1:
        alpha = gx->matColor[1] & 0xFF;
        GX_SET_FIELD(reg, 8, 0, alpha);
        GX_SET_FIELD(reg, 8, 8, mat_color.b);
        GX_SET_FIELD(reg, 8, 16, mat_color.g);
        GX_SET_FIELD(reg, 8, 24, mat_color.r);
        colIdx = 1;
        break;
    case GX_ALPHA0:
        reg = gx->matColor[0];
        GX_SET_FIELD(reg, 8, 0, mat_color.a);
        colIdx = 0;
        break;
    case GX_ALPHA1:
        reg = gx->matColor[1];
        GX_SET_FIELD(reg, 8, 0, mat_color.a);
        colIdx = 1;
        break;
    case GX_COLOR0A0:
        GX_SET_FIELD(reg, 8, 0, mat_color.a);
        GX_SET_FIELD(reg, 8, 8, mat_color.b);
        GX_SET_FIELD(reg, 8, 16, mat_color.g);
        GX_SET_FIELD(reg, 8, 24, mat_color.r);
        colIdx = 0;
        break;
    case GX_COLOR1A1:
        GX_SET_FIELD(reg, 8, 0, mat_color.a);
        GX_SET_FIELD(reg, 8, 8, mat_color.b);
        GX_SET_FIELD(reg, 8, 16, mat_color.g);
        GX_SET_FIELD(reg, 8, 24, mat_color.r);
        colIdx = 1;
        break;
    default:

        return;
    }

    GX_WRITE_XF_REG(colIdx + 12, reg);
    gx->bpSent = 1;
    gx->matColor[colIdx] = reg;
}

void GXSetNumChans(u8 nChans)
{
    GX_SET_FIELD(gx->genMode, 3, 4, nChans);
    GX_WRITE_XF_REG(9, nChans);
    gx->dirtyState |= 4;
}

void GXSetChanCtrl(GXChannelID chan, u8 enable, s32 amb_src, s32 mat_src,
                   u32 light_mask, s32 diff_fn, s32 attn_fn)
{
    u32 reg;
    u32 idx;

    if (chan == GX_COLOR0A0)
        idx = 0;
    else if (chan == GX_COLOR1A1)
        idx = 1;
    else
        idx = chan;

    reg = 0;
    GX_SET_FIELD(reg, 1, 1, enable);
    GX_SET_FIELD(reg, 1, 0, mat_src);
    GX_SET_FIELD(reg, 1, 6, amb_src);
    GX_SET_FIELD(reg, 1, 2, (light_mask & GX_LIGHT0) != 0);
    GX_SET_FIELD(reg, 1, 3, (light_mask & GX_LIGHT1) != 0);
    GX_SET_FIELD(reg, 1, 4, (light_mask & GX_LIGHT2) != 0);
    GX_SET_FIELD(reg, 1, 5, (light_mask & GX_LIGHT3) != 0);
    GX_SET_FIELD(reg, 1, 11, (light_mask & GX_LIGHT4) != 0);
    GX_SET_FIELD(reg, 1, 12, (light_mask & GX_LIGHT5) != 0);
    GX_SET_FIELD(reg, 1, 13, (light_mask & GX_LIGHT6) != 0);
    GX_SET_FIELD(reg, 1, 14, (light_mask & GX_LIGHT7) != 0);
    GX_SET_FIELD(reg, 2, 7, (attn_fn == GX_AF_SPEC) ? GX_DF_NONE : diff_fn);
    GX_SET_FIELD(reg, 1, 9, (attn_fn != GX_AF_NONE));
    GX_SET_FIELD(reg, 1, 10, (attn_fn != GX_AF_SPEC));

    GX_WRITE_XF_REG(idx + 14, reg);
    gx->bpSent = 1;
    if (chan == GX_COLOR0A0) {
        GX_WRITE_XF_REG(16, reg);
    } else if (chan == GX_COLOR1A1) {
        GX_WRITE_XF_REG(17, reg);
    }
}

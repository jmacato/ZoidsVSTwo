/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/psdisp.c. */
#include "particle.h"
#include "appsrt.h"
#include "gx_attr.h"
#include "gx_light.h"
#include "gx_geometry.h"
#include "gx_pixel.h"
#include "gx_texture.h"
#include "gx_tev_api.h"
#include "gx_transform.h"
#include "gx_state.h"
#include "lobj.h"
#include "cobj.h"
#include "matrix.h"
#include "util.h"

#define NULL 0

extern f64 __frsqrte(f64);
extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 atan2f(f32, f32);
extern void setVtxDesc(s32 fmt);
extern void calcTornadoLastPos(HSD_Particle *pp, f32 *x, f32 *y, f32 *z);
extern void getColorPrimEnv(HSD_Particle *pp, GXColor *primCol, GXColor *envCol);
extern void getColorMatAmb(HSD_Particle *pp, GXColor *matCol, GXColor *ambCol);
extern void HSD_FogSet(void *fog);
extern void HSD_StateInvalidate(u32 flags);
extern void PSMTXInverse(Mtx src, Mtx inv);
extern void PSMTXRotAxisRad(Mtx m, Vec3 *axis, f32 rad);
extern void GXGetProjectionv(f32 *p);
extern void GXLoadPosMtxImm(f32 mtx[3][4], u32 id);

#define PS_APPSTATUS_ONCE 1
#define PS_APPSTATUS_STILL 2
#define FLT_EPSILON 1.19209290e-07F
#define FLT_MIN 1.17549435e-38F

typedef struct {
    Mtx mtx;
} psdisp_Mtx;

typedef struct {
    void *lut;
    s32 fmt;
    s32 tlut_name;
    u16 n_entries;
} HSD_TlutDesc;

static char str_object_h[] = "object.h";
static char str_ref_count[] = "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF";
static u8 billboard_tex0_u8[] __attribute__((aligned(32))) = {
    0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0,
};
u8 psFrameNum = 123;
static Mtx vmtx;
static Mtx rvmtx;
static f32 prj[7];
static Mtx pvmtx;
static void *particleFog;
static s32 prevPointSize;
static s32 prevLineWidth;
static f32 rx0;
static f32 rx1;
static f32 ry0;
static f32 ry1;
static f32 rz0;
static f32 rz1;
static s32 prevBlendMode;
static s32 prevChanCtrl;
static GXColor prevChanMat;
static GXColor prevChanAmb;
static GXColor prevColorPrim;
static GXColor prevColorEnv;
static GXColor prevColorMat;
static volatile s32 currentMtx;

static inline float sqrtf(float x)
{
    volatile float y;
    if (x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        y = (float)(x * guess);
        return y;
    }
    return x;
}

static inline void GXPosition3f32(f32 x, f32 y, f32 z)
{
    GXWGFifo.f32 = x;
    GXWGFifo.f32 = y;
    GXWGFifo.f32 = z;
}

static inline void GXColor4u8(u8 r, u8 g, u8 b, u8 a)
{
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
}

static inline void GXTexCoord1x8(u8 x)
{
    GXWGFifo.u8 = x;
}

#define GX_FALSE 0
#define GX_TRUE 1
#define GX_PNMTX0 0
#define GX_PNMTX1 3
#define GX_TEXMTX0 30
#define GX_PTIDENTITY 125
#define GX_TLUT0 0

typedef struct {
    f32 x;
    f32 y;
} Vec2;

static inline void getClrTrail(HSD_Particle* pp, GXColor* color)
{
    GXColor env_color;

    switch (pp->kind & (DispLighting | PrimEnv)) {
    case 0:
    case DispLighting:
        getColorPrimEnv(pp, color, &env_color);
        break;
    case PrimEnv:
    case DispLighting | PrimEnv:
        color->r = color->g = color->b = color->a = 0xFF;
        break;
    }
}

static inline void psSetColor(GXColor* color, u8 value)
{
    color->r = value;
    color->g = value;
    color->b = value;
    color->a = value;
}

static inline void psSetupVtxFormat(GXVtxFmt format, int has_color,
                                    int has_texture, GXCompType texture_type)
{
    GXSetVtxAttrFmt(format, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0U);
    if (has_color) {
        GXSetVtxAttrFmt(format, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
    }
    if (has_texture) {
        GXSetVtxAttrFmt(format, GX_VA_TEX0, GX_TEX_ST, texture_type, 0U);
    }
}

static inline void setupChanCtrl(HSD_Particle* pp)
{
    u32 chan_state = pp->kind & (DispLighting | Trail);

    if (chan_state != (u32) prevChanCtrl) {
        prevChanCtrl = chan_state;
        GXSetNumChans(1);
        switch (prevChanCtrl) {
        case Trail:
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
                          GX_DF_NONE, GX_AF_NONE);
            break;
        case DispLighting:
            GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG,
                          HSD_LObjGetLightMaskDiffuse(), GX_DF_NONE,
                          HSD_LObjGetLightMaskAttnFunc() ? GX_AF_SPOT
                                                         : GX_AF_NONE);
            GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
                          GX_DF_NONE, GX_AF_NONE);
            break;
        case DispLighting | Trail:
            GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG,
                          HSD_LObjGetLightMaskDiffuse(), GX_DF_NONE,
                          HSD_LObjGetLightMaskAttnFunc() ? GX_AF_SPOT
                                                         : GX_AF_NONE);
            GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
                          GX_DF_NONE, GX_AF_NONE);
            break;
        default:
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
                          GX_DF_NONE, GX_AF_NONE);
            break;
        }
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
                      GX_DF_NONE, GX_AF_NONE);
    }
}

static inline void setupChanReg(HSD_Particle* pp)
{
    GXColor prim_color;
    GXColor amb_color;
    GXColor mat_color;
    HSD_LObj* lobj;

    if (pp->kind & DispLighting) {
        getColorMatAmb(pp, &mat_color, &amb_color);
        if (pp->kind & PrimEnv) {
            prim_color.r = prim_color.g = prim_color.b = 0xFF;
        } else {
            getColorPrimEnv(pp, &prim_color, &mat_color);
            amb_color.r = (u8) ((amb_color.r * prim_color.r) >> 8);
            amb_color.g = (u8) ((amb_color.g * prim_color.g) >> 8);
            amb_color.b = (u8) ((amb_color.b * prim_color.b) >> 8);
        }
        if (prim_color.r != prevChanMat.r || prim_color.g != prevChanMat.g ||
            prim_color.b != prevChanMat.b)
        {
            prevChanMat = prim_color;
            GXSetChanMatColor(GX_COLOR0, prevChanMat);
        }
        lobj = HSD_LObjGetActiveByID(GX_MAX_LIGHT);
        if (lobj != NULL) {
            HSD_MulColor(&amb_color, &lobj->color, &amb_color);
        } else {
            amb_color.r = amb_color.g = amb_color.b = 0;
        }
        if (amb_color.r != prevChanAmb.r || amb_color.g != prevChanAmb.g ||
            amb_color.b != prevChanAmb.b)
        {
            prevChanAmb = amb_color;
            GXSetChanAmbColor(GX_COLOR0, prevChanAmb);
        }
    }
}

static inline void setupTevReg(HSD_Particle* pp)
{
    GXColor prim_color;
    GXColor env_color;
    GXColor mat_color;
    GXColor amb_color;

    getColorPrimEnv(pp, &prim_color, &env_color);
    if ((pp->kind & PrimEnv) ||
        (!(pp->kind & DispLighting) && !(pp->kind & Trail)))
    {
        if (prevColorPrim.r != prim_color.r ||
            prevColorPrim.g != prim_color.g ||
            prevColorPrim.b != prim_color.b || prevColorPrim.a != prim_color.a)
        {
            prevColorPrim = prim_color;
            GXSetTevColor(GX_TEVREG0, prevColorPrim);
        }
        if (pp->kind & PrimEnv) {
            if (prevColorEnv.r != env_color.r ||
                prevColorEnv.g != env_color.g ||
                prevColorEnv.b != env_color.b || prevColorEnv.a != env_color.a)
            {
                prevColorEnv = env_color;
                GXSetTevColor(GX_TEVREG1, prevColorEnv);
            }
        } else if (prevColorEnv.r != 0 || prevColorEnv.g != 0 ||
                   prevColorEnv.b != 0 || prevColorEnv.a != 0)
        {
            prevColorEnv.r = prevColorEnv.g = prevColorEnv.b = prevColorEnv.a =
                0;
            GXSetTevColor(GX_TEVREG1, prevColorEnv);
        }
    }
    if (pp->kind & DispLighting) {
        getColorMatAmb(pp, &mat_color, &amb_color);
        if (pp->kind & PrimEnv) {
            if (prevColorMat.r != mat_color.r ||
                prevColorMat.g != mat_color.g ||
                prevColorMat.b != mat_color.b || prevColorMat.a != mat_color.a)
            {
                prevColorMat = mat_color;
                GXSetTevColor(GX_TEVREG2, prevColorMat);
            }
        } else {
            mat_color.a = (u8) ((mat_color.a * prim_color.a) >> 8);
            if (prevColorMat.r != mat_color.r ||
                prevColorMat.g != mat_color.g ||
                prevColorMat.b != mat_color.b || prevColorMat.a != mat_color.a)
            {
                prevColorMat = mat_color;
                GXSetTevColor(GX_TEVREG2, prevColorMat);
            }
        }
    }
}

static inline void setBlendMode(int blend_mode)
{
    if (prevBlendMode != blend_mode) {
        prevBlendMode = blend_mode;
        switch (blend_mode) {
        case 0:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                           GX_LO_CLEAR);
            break;
        case 1:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE,
                           GX_LO_CLEAR);
            break;
        default:
            OSReport("Particle:setBlendMode:Unknown mode\n");
            break;
        }
    }
}

static inline void psSetCurrentMtx(s32 idx)
{
    if (currentMtx != idx) {
        currentMtx = idx;
        GXSetCurrentMtx(idx);
    }
}

static inline void psDispSubPoint(HSD_Particle* pp)
{
    u8 w;

    psSetCurrentMtx(GX_PNMTX0);
    w = (pp->size > 42.5) ? 255.0f : 6.0f * pp->size;
    if (prevPointSize != (s32) w) {
        prevPointSize = w;
        GXSetPointSize(w, GX_TO_ONE);
    }
    if (pp->kind & DispTexture) {
        setVtxDesc(0);
        GXBegin(GX_POINTS, GX_VTXFMT0, 1);
    } else {
        setVtxDesc(1);
        GXBegin(GX_POINTS, GX_VTXFMT1, 1);
    }
    GXPosition3f32(pp->pos.x, pp->pos.y, pp->pos.z);
    if (pp->kind & DispTexture) {
        GXTexCoord1x8(1);
    }
}

static inline void psDispSubPointTrail(HSD_Particle* pp)
{
    f32 x;
    f32 y;
    f32 z;
    GXColor color;
    u8 w;

    psSetCurrentMtx(GX_PNMTX0);
    w = (pp->size > 42.5) ? 255.0f : 6.0f * pp->size;
    if (prevLineWidth != (s32) w) {
        prevLineWidth = w;
        GXSetLineWidth(w, GX_TO_ONE);
    }
    if (pp->kind & Tornado) {
        calcTornadoLastPos(pp, &x, &y, &z);
    } else {
        x = pp->pos.x - pp->vel.x;
        y = pp->pos.y - pp->vel.y;
        z = pp->pos.z - pp->vel.z;
    }
    getClrTrail(pp, &color);
    if (pp->kind & DispTexture) {
        setVtxDesc(2);
        GXBegin(GX_LINES, GX_VTXFMT2, 2);
    } else {
        setVtxDesc(3);
        GXBegin(GX_LINES, GX_VTXFMT3, 2);
    }
    GXPosition3f32(x, y, z);
    GXColor4u8(color.r, color.g, color.b, (u8) ((f32) color.a * pp->trail));
    if (pp->kind & DispTexture) {
        GXTexCoord1x8(0);
    }
    GXPosition3f32(pp->pos.x, pp->pos.y, pp->pos.z);
    GXColor4u8(color.r, color.g, color.b, color.a);
    if (pp->kind & DispTexture) {
        GXTexCoord1x8(1);
    }
}

static inline void psMaskAbsF32(f32* value)
{
    *(s32*) value &= 0x7FFFFFFF;
}

static inline int psPolyAbsLtF32(f32 value, f32 limit)
{
    *(s32*) &value &= 0x7FFFFFFF;
    return value < limit;
}

static inline void psDispSubMakePolygon(HSD_Particle* pp, u8* texform, f32 x,
                                        f32 y, f32 z, f32 x0, f32 y0, f32 z0,
                                        f32 x1, f32 y1, f32 z1, GXColor* color,
                                        f32* prev_x, f32* prev_y, f32* prev_z)
{
    Vec2 right;
    f32 right_z;
    Vec2 up;
    f32 up_z;
    u8* it = texform;
    f32 cx;

    right.x = x0;
    right.y = y0;
    right_z = z0;
    up.x = x1;
    up.y = y1;
    up_z = z1;

    psSetCurrentMtx(GX_PNMTX0);
    if (pp->kind & Trail) {
        if (pp->kind & Tornado) {
            calcTornadoLastPos(pp, prev_x, prev_y, prev_z);
        } else {
            *prev_x = x - pp->vel.x;
            *prev_y = y - pp->vel.y;
            *prev_z = z - pp->vel.z;
        }
        getClrTrail(pp, color);
        if (it == NULL) {
            if (pp->kind & DispTexture) {
                setVtxDesc(2);
                GXBegin(GX_QUADS, GX_VTXFMT2, 4);
            } else {
                setVtxDesc(3);
                GXBegin(GX_QUADS, GX_VTXFMT3, 4);
            }
            GXPosition3f32((*prev_x) - right.x, (*prev_y) - right.y,
                           (*prev_z) - right_z);
            {
                u8 a = color->a;
                u8 b = color->b;
                u8 g = color->g;
                u16 r = color->r;
                GXColor4u8(r, g, b, (u8) ((f32) a * pp->trail));
            }
            if (pp->kind & DispTexture) {
                u8 tex_base = (pp->kind >> 16) & 0xC;

                GXWGFifo.u8 = tex_base;
            }
            GXPosition3f32(x - up.x, y - up.y, z - up_z);
            {
                u8 a = color->a;
                u8 b = color->b;
                u8 g = color->g;
                u16 r = color->r;
                GXColor4u8(r, g, b, a);
            }
            if (pp->kind & DispTexture) {
                GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 1;
            }
            GXPosition3f32(x + right.x, y + right.y, z + right_z);
            {
                u8 a = color->a;
                u8 b = color->b;
                u8 g = color->g;
                u16 r = color->r;
                GXColor4u8(r, g, b, a);
            }
            if (pp->kind & DispTexture) {
                GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 2;
            }
            GXPosition3f32((*prev_x) + up.x, (*prev_y) + up.y,
                           (*prev_z) + up_z);
            {
                u8 a = color->a;
                f32 alpha = (f32) a * pp->trail;
                u8 b = color->b;
                u8 g = color->g;
                u16 r = color->r;
                GXColor4u8(r, g, b, (u8) alpha);
            }
            if (pp->kind & DispTexture) {
                GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 3;
            }
        } else {
            f32 trail_alpha = 255.0f * (1.0f - pp->trail);
            f32 up_len = sqrtf(up.x * up.x + up.y * up.y + up_z * up_z);

            if (!psPolyAbsLtF32(up_len, FLT_MIN)) {
                f32 dz = z - (*prev_z);
                f32 dx = x - (*prev_x);
                f32 dy = y - (*prev_y);
                f32 xl = dx * dx;
                f32 yl = dy * dy;
                f32 zl = dz * dz;
                f32 segment_len = sqrtf(zl + (xl + yl));
                f32 ratio = segment_len / up_len;
                u32 primitive_count;

                if (ratio < 1.0f) {
                    ratio = 1.0f;
                }
                primitive_count = *(u32*) it;
                it += sizeof(u32);
                up.x *= ratio;
                up.y *= ratio;
                up_z *= ratio;
                for (; primitive_count != 0; primitive_count--) {
                    GXPrimitive primitive = it[0];
                    u8 count = it[1];
                    s32 i;

                    it += 4;
                    if (pp->kind & DispTexture) {
                        setVtxDesc(5);
                        GXBegin(primitive, GX_VTXFMT5, count);
                    } else {
                        setVtxDesc(3);
                        GXBegin(primitive, GX_VTXFMT3, count);
                    }
                    for (i = count; i > 0; i--) {
                        f32 s = *(f32*) &it[0];
                        f32 sx = 2.0f * (s - 0.5f);
                        f32 t;
                        f32 tx;
                        s32 converted_alpha;
                        s32 alpha;

                        if (pp->kind & TexFlipS) {
                            s = 1.0f - s;
                        }
                        t = *(f32*) &it[4];
                        it += 8;
                        alpha = (s32) (255.0f - t * trail_alpha);
                        converted_alpha = (s32) (255.0f - t * trail_alpha);
                        if (converted_alpha < 0) {
                            alpha = 0;
                        }
                        if (alpha > 0xFF) {
                            alpha = 0xFF;
                        }
                        tx = 2.0f * (t - 0.5f);
                        if (pp->kind & TexFlipT) {
                            t = 1.0f - t;
                        }
                        GXPosition3f32(up.x * tx + (right.x * sx + x),
                                       up.y * tx + (right.y * sx + y),
                                       up_z * tx + (right_z * sx + z));
                        GXColor4u8(color->r, color->g, color->b, alpha);
                        if (pp->kind & DispTexture) {
                            GXWGFifo.f32 = s;
                            GXWGFifo.f32 = t;
                        }
                    }
                }
            }
        }
    } else if (it == NULL) {
        if (pp->kind & DispTexture) {
            setVtxDesc(0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        } else {
            setVtxDesc(1);
            GXBegin(GX_QUADS, GX_VTXFMT1, 4);
        }
        GXPosition3f32((cx = x - right.x), y - right.y, z - right_z);
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = (pp->kind >> 16) & 0xC;
        }
        GXPosition3f32((cx = x - up.x), y - up.y, z - up_z);
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 1;
        }
        GXPosition3f32((cx = x + right.x), y + right.y, z + right_z);
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 2;
        }
        GXPosition3f32(x + up.x, y + up.y, z + up_z);
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 3;
        }
    } else {
        u32 primitive_count = *(u32*) it;
        it += sizeof(u32);
        for (; primitive_count != 0; primitive_count--) {
            GXPrimitive primitive = it[0];
            u8 count = it[1];
            s32 i;

            it += 4;
            if (pp->kind & DispTexture) {
                setVtxDesc(4);
                GXBegin(primitive, GX_VTXFMT4, count);
            } else {
                setVtxDesc(1);
                GXBegin(primitive, GX_VTXFMT1, count);
            }
            for (i = count; i > 0; i--) {
                f32 s = *(f32*) &it[0];
                f32 sx = 2.0f * (s - 0.5f);
                f32 t;
                f32 tx;

                if (pp->kind & TexFlipS) {
                    s = 1.0f - s;
                }
                t = *(f32*) &it[4];
                it += 8;
                tx = 2.0f * (t - 0.5f);
                if (pp->kind & TexFlipT) {
                    t = 1.0f - t;
                }
                GXPosition3f32(x + right.x * sx + up.x * tx,
                               y + right.y * sx + up.y * tx,
                               z + right_z * sx + up_z * tx);
                if (pp->kind & DispTexture) {
                    GXWGFifo.f32 = s;
                    GXWGFifo.f32 = t;
                }
            }
        }
    }
    (void) (up.x + up.y + up_z);
}

static inline int psMaskAbsLtF32(f32 value, f32 limit)
{
    psMaskAbsF32(&value);
    return value < limit;
}

static inline int psMaskAbsGtF32(f32 value, f64 limit)
{
    psMaskAbsF32(&value);
    return value > limit;
}

static inline int psAbsLtF32(f32 value, f32 limit)
{
    *(s32*) &value &= 0x7FFFFFFF;
    return value < limit;
}

static inline int psAbsGtF32(f32 value, f64 limit)
{
    *(s32*) &value &= 0x7FFFFFFF;
    return value > limit;
}

static inline int psDispSubAbsLtF32(f32 value, f32 limit)
{
    return psAbsLtF32(value, limit);
}

static inline int psDispSubAbsGtF32(f32 value, f64 limit)
{
    return psAbsGtF32(value, limit);
}

static inline void psDispSub(HSD_Particle* pp, u8* texform)
{
    f32 right_y;
    f32 right_x;
    f32 right_z;
    f32 up_x;
    f32 up_y;
    f32 up_z;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;

    x = pp->pos.x;
    y = pp->pos.y;
    z = pp->pos.z;
    if (texform != NULL) {
        right_x = rvmtx[0][0] * pp->size;
        up_x = -rvmtx[0][1] * pp->size;
        right_y = rvmtx[1][0] * pp->size;
        up_y = -rvmtx[1][1] * pp->size;
        right_z = rvmtx[2][0] * pp->size;
        up_z = -rvmtx[2][1] * pp->size;
    } else {
        right_x = rx0 * pp->size;
        up_x = rx1 * pp->size;
        right_y = ry0 * pp->size;
        up_y = ry1 * pp->size;
        right_z = rz0 * pp->size;
        up_z = rz1 * pp->size;
    }
    if ((pp->kind & Trail) || (pp->kind & DirVec)) {
        f32 x;
        f32 y;

        if (0.0f == prj[0]) {
            f32 prev_x;
            f32 prev_y;
            f32 prev_z;
            f32 w0inv;
            f32 w1inv;
            f32 pv13;
            f32 pv03;

            if (pp->kind & Tornado) {
                calcTornadoLastPos(pp, &prev_x, &prev_y, &prev_z);
            } else {
                prev_x = pp->pos.x - pp->vel.x;
                prev_y = pp->pos.y - pp->vel.y;
                prev_z = pp->pos.z - pp->vel.z;
            }
            w0inv = -1.0f / (vmtx[2][3] +
                 (vmtx[2][2] * pp->pos.z +
                  (vmtx[2][0] * pp->pos.x + vmtx[2][1] * pp->pos.y)));
            w1inv = -1.0f / (vmtx[2][3] + (vmtx[2][2] * prev_z +
                               (vmtx[2][0] * prev_x + vmtx[2][1] * prev_y)));
            pv03 = pvmtx[0][3];
            pv13 = pvmtx[1][3];
            x = w0inv * (pv03 + (pvmtx[0][2] * pp->pos.z +
                                 (((f32 *)pvmtx)[0] * pp->pos.x +
                                  pvmtx[0][1] * pp->pos.y))) -
                w1inv *
                    (pv03 + (pvmtx[0][2] * prev_z +
                             (pvmtx[0][0] * prev_x + pvmtx[0][1] * prev_y)));
            y = w0inv * (pv13 + (pvmtx[1][2] * pp->pos.z +
                                 (pvmtx[1][0] * pp->pos.x +
                                  pvmtx[1][1] * pp->pos.y))) -
                w1inv *
                    (pv13 + (pvmtx[1][2] * prev_z +
                             (pvmtx[1][0] * prev_x + pvmtx[1][1] * prev_y)));
        } else if (pp->kind & Tornado) {
            f32 prev_x;
            f32 prev_y;
            f32 prev_z;
            f32 dx;
            f32 dy;
            f32 dz;

            calcTornadoLastPos(pp, &prev_x, &prev_y, &prev_z);
            dx = pp->pos.x - prev_x;
            dy = pp->pos.y - prev_y;
            dz = pp->pos.z - prev_z;
            x = pvmtx[0][2] * dz + (pvmtx[0][0] * dx + pvmtx[0][1] * dy);
            y = pvmtx[1][2] * dz + (pvmtx[1][0] * dx + pvmtx[1][1] * dy);
        } else {
            x = pvmtx[0][2] * pp->vel.z +
                (pvmtx[0][0] * pp->vel.x + pvmtx[0][1] * pp->vel.y);
            y = pvmtx[1][2] * pp->vel.z +
                (pvmtx[1][0] * pp->vel.x + pvmtx[1][1] * pp->vel.y);
        }
        if (psDispSubAbsLtF32(y, FLT_MIN)) {
            angle = (x >= 0.0f) ? 1.5707964f : -1.5707964f;
        } else {
            angle = atan2f(x, y);
        }
        if (pp->kind & DirVec) {
            angle += pp->rotate;
        }
    } else {
        angle = pp->rotate;
    }
    if (psDispSubAbsGtF32(angle, 0.01)) {
        Mtx mtx;
        Vec3 axis;

        f32 rx = right_x;
        f32 ry = right_y;
        f32 rz = right_z;
        f32 ux = up_x;
        f32 uz = up_z;
        f32 uy = up_y;
        f32 ax;
        f32 ay;
        f32 az;

        ax = ry * uz - rz * uy;
        {
            f32 axis_y_product = rx * uz;
            ay = rz * ux - axis_y_product;
        }
        az = rx * uy - ry * ux;
        axis.x = ax;
        axis.y = ay;
        axis.z = az;
        PSMTXRotAxisRad(mtx, &axis, angle);
        right_x = mtx[0][2] * rz + (mtx[0][0] * rx + mtx[0][1] * ry);
        right_y = mtx[1][2] * rz + (mtx[1][0] * rx + mtx[1][1] * ry);
        right_z = mtx[2][2] * rz + (mtx[2][0] * rx + mtx[2][1] * ry);
        up_x = mtx[0][2] * uz + (mtx[0][0] * ux + mtx[0][1] * uy);
        up_y = mtx[1][2] * uz + (mtx[1][0] * ux + mtx[1][1] * uy);
        up_z = mtx[2][2] * uz + (mtx[2][0] * ux + mtx[2][1] * uy);
    }
    {
        GXColor color;
        f32 prev_x;
        f32 prev_y;
        f32 prev_z;

        psDispSubMakePolygon(pp, texform, x, y, z, right_x, right_y, right_z,
                             up_x, up_y, up_z, &color, &prev_x, &prev_y,
                             &prev_z);
    }
}

static inline void psScaleAppSRTAxes(HSD_Particle* pp, Mtx mtx)
{
    mtx[0][0] *= pp->size;
    mtx[1][0] *= pp->size;
    mtx[2][0] *= pp->size;
    mtx[0][1] *= pp->size;
    mtx[1][1] *= pp->size;
    mtx[2][1] *= pp->size;
    mtx[0][2] *= pp->size;
    mtx[1][2] *= pp->size;
    mtx[2][2] *= pp->size;
}

static inline void psDispSubAPPSRTPoint(HSD_Particle* pp)
{
    f32 dummy;
    Mtx scratch_mtx;
    Vec3 scratch_scale;
    f32 cur_x;
    f32 cur_y;
    f32 cur_z;
    f32 prev_x;
    f32 prev_y;
    f32 prev_z;
    u8 w;

    (void) &dummy;
    psSetCurrentMtx(GX_PNMTX1);
    if (pp->appsrt != NULL) {
        if (pp->appsrt->frameNum != psFrameNum) {
            f32 scale_x;
            f32 scale_y;

            if (pp->appsrt->status != PS_APPSTATUS_STILL) {
                Vec3* translate = &pp->appsrt->translate;
                Vec3* rotate = (Vec3*) &pp->appsrt->rot;
                Vec3* scale = &pp->appsrt->scale;
                MtxPtr mmtx = pp->appsrt->mmtx;

                HSD_MtxSRT(mmtx, scale, rotate, translate, NULL);
            }
            if (pp->appsrt->status == PS_APPSTATUS_ONCE) {
                pp->appsrt->status = PS_APPSTATUS_STILL;
            }
            PSMTXConcat(vmtx, pp->appsrt->mmtx, pp->appsrt->vmmtx);
            scale_x = pp->appsrt->vmmtx[0][0] * pp->appsrt->vmmtx[0][0] +
                      pp->appsrt->vmmtx[1][0] * pp->appsrt->vmmtx[1][0] +
                      pp->appsrt->vmmtx[2][0] * pp->appsrt->vmmtx[2][0];
            scale_x = sqrtf(scale_x);
            pp->appsrt->view_scale_x = scale_x;
            scale_y = pp->appsrt->vmmtx[0][1] * pp->appsrt->vmmtx[0][1] +
                      pp->appsrt->vmmtx[1][1] * pp->appsrt->vmmtx[1][1] +
                      pp->appsrt->vmmtx[2][1] * pp->appsrt->vmmtx[2][1];
            scale_y = sqrtf(scale_y);
            pp->appsrt->view_scale_y = scale_y;
            if (pp->appsrt->xA2 != 0) {
                PSMTXIdentity(scratch_mtx);
                scratch_mtx[0][3] = pp->appsrt->translate.x;
                scratch_mtx[1][3] = pp->appsrt->translate.y;
                scratch_mtx[2][3] = pp->appsrt->translate.z;
                PSMTXConcat(vmtx, scratch_mtx, scratch_mtx);
                HSD_MtxGetScale(scratch_mtx, &scratch_scale);
                PSMTXScale(pp->appsrt->vmmtx, scratch_scale.x,
                           scratch_scale.y, scratch_scale.z);
                pp->appsrt->vmmtx[0][3] = scratch_mtx[0][3];
                pp->appsrt->vmmtx[1][3] = scratch_mtx[1][3];
                pp->appsrt->vmmtx[2][3] = scratch_mtx[2][3];
            }
        }
        pp->appsrt->frameNum = psFrameNum;
    }
    cur_x = pp->appsrt->vmmtx[0][3] +
            (pp->appsrt->vmmtx[0][2] * pp->pos.z +
             (pp->appsrt->vmmtx[0][0] * pp->pos.x + pp->appsrt->vmmtx[0][1] * pp->pos.y));
    cur_y = pp->appsrt->vmmtx[1][3] +
            (pp->appsrt->vmmtx[1][2] * pp->pos.z +
             (pp->appsrt->vmmtx[1][0] * pp->pos.x + pp->appsrt->vmmtx[1][1] * pp->pos.y));
    cur_z = pp->appsrt->vmmtx[2][3] +
            (pp->appsrt->vmmtx[2][2] * pp->pos.z +
             (pp->appsrt->vmmtx[2][0] * pp->pos.x + pp->appsrt->vmmtx[2][1] * pp->pos.y));
    (void) (pp->pos.x, pp->pos.y);
    (void) (pp->pos.z, pp->size);
    if (pp->kind & Tornado) {
        f32 x;
        f32 y;
        f32 z;

        calcTornadoLastPos(pp, &x, &y, &z);
        prev_x =
            pp->appsrt->vmmtx[0][3] + (pp->appsrt->vmmtx[0][2] * z +
                               (pp->appsrt->vmmtx[0][0] * x + pp->appsrt->vmmtx[0][1] * y));
        prev_y =
            pp->appsrt->vmmtx[1][3] + (pp->appsrt->vmmtx[1][2] * z +
                               (pp->appsrt->vmmtx[1][0] * x + pp->appsrt->vmmtx[1][1] * y));
        prev_z =
            pp->appsrt->vmmtx[2][3] + (pp->appsrt->vmmtx[2][2] * z +
                               (pp->appsrt->vmmtx[2][0] * x + pp->appsrt->vmmtx[2][1] * y));
    } else {
        prev_x =
            pp->appsrt->vmmtx[0][3] + (pp->appsrt->vmmtx[0][2] * (pp->pos.z - pp->vel.z) +
                               (pp->appsrt->vmmtx[0][0] * (pp->pos.x - pp->vel.x) +
                                pp->appsrt->vmmtx[0][1] * (pp->pos.y - pp->vel.y)));
        prev_y =
            pp->appsrt->vmmtx[1][3] + (pp->appsrt->vmmtx[1][2] * (pp->pos.z - pp->vel.z) +
                               (pp->appsrt->vmmtx[1][0] * (pp->pos.x - pp->vel.x) +
                                pp->appsrt->vmmtx[1][1] * (pp->pos.y - pp->vel.y)));
        prev_z =
            pp->appsrt->vmmtx[2][3] + (pp->appsrt->vmmtx[2][2] * (pp->pos.z - pp->vel.z) +
                               (pp->appsrt->vmmtx[2][0] * (pp->pos.x - pp->vel.x) +
                                pp->appsrt->vmmtx[2][1] * (pp->pos.y - pp->vel.y)));
    }

    w = (pp->size > 42.5) ? 255.0f : 6.0f * pp->size;
    if (pp->kind & Trail) {
        GXColor draw_color;

        if (prevLineWidth != (s32) w) {
            prevLineWidth = w;
            GXSetLineWidth(w, GX_TO_ONE);
        }
        getClrTrail(pp, &draw_color);
        if (pp->kind & DispTexture) {
            setVtxDesc(2);
            GXBegin(GX_LINES, GX_VTXFMT2, 2);
        } else {
            setVtxDesc(3);
            GXBegin(GX_LINES, GX_VTXFMT3, 2);
        }
        GXPosition3f32(prev_x, prev_y, prev_z);
        GXColor4u8(draw_color.r, draw_color.g, draw_color.b,
                   (u8) ((f32) draw_color.a * pp->trail));
        if (pp->kind & DispTexture) {
            GXTexCoord1x8(0);
        }
        GXPosition3f32(cur_x, cur_y, cur_z);
        GXColor4u8(draw_color.r, draw_color.g, draw_color.b, draw_color.a);
        if (pp->kind & DispTexture) {
            GXTexCoord1x8(1);
        }
    } else {
        if (prevPointSize != (s32) w) {
            prevPointSize = w;
            GXSetPointSize(w, GX_TO_ONE);
        }
        if (pp->kind & DispTexture) {
            setVtxDesc(0);
            GXBegin(GX_POINTS, GX_VTXFMT0, 1);
        } else {
            setVtxDesc(1);
            GXBegin(GX_POINTS, GX_VTXFMT1, 1);
        }
        GXPosition3f32(cur_x, cur_y, cur_z);
        if (pp->kind & DispTexture) {
            GXTexCoord1x8(1);
        }
    }
}

static inline void GXPosition2f32(f32 x, f32 y)
{
    GXWGFifo.f32 = x;
    GXWGFifo.f32 = y;
}

static inline void psDispSubAppSRT(HSD_Particle* pp, u8* texform)
{
    Mtx draw_mtx;
    f32 w0;
    f32 w1;
    f32 pad;
    Vec3 scratch_scale;
    f32 x_extent;
    f32 y_extent;
    f32 ax;
    f32 ay;
    f32 bx;
    f32 by;
    f32 angle;
    u8* it = texform;
    f32 pm12;
    f32 pm11;
    f32 pm10;
    f32 pm13;
    f32 pm02;
    f32 pm03;
    f32 om10;
    f32 om11;
    f32 om12;
    f32 om02;
    f32 om01;
    f32 om00;
    f32 pm01;
    f32 pm00;
    f32 prev_pos_x;
    f32 prev_pos_y;
    f32 prev_pos_z;
    f32 w0inv;
    f32 w1inv;
    f32 cur_x;
    f32 cur_y;
    f32 cur_z;

    if (pp->appsrt->frameNum != psFrameNum) {
        f32 scale_x;
        f32 scale_y;

        if (pp->appsrt->status != PS_APPSTATUS_STILL) {
            Vec3* translate = &pp->appsrt->translate;
            Vec3* rotate = (Vec3*) &pp->appsrt->rot;
            Vec3* scale = &pp->appsrt->scale;
            MtxPtr mmtx = pp->appsrt->mmtx;

            HSD_MtxSRT(mmtx, scale, rotate, translate, NULL);
        }
        if (pp->appsrt->status == PS_APPSTATUS_ONCE) {
            pp->appsrt->status = PS_APPSTATUS_STILL;
        }
        PSMTXConcat(vmtx, pp->appsrt->mmtx, pp->appsrt->vmmtx);
        scale_x = pp->appsrt->vmmtx[0][0] * pp->appsrt->vmmtx[0][0] +
                  pp->appsrt->vmmtx[1][0] * pp->appsrt->vmmtx[1][0] +
                  pp->appsrt->vmmtx[2][0] * pp->appsrt->vmmtx[2][0];
        scale_x = sqrtf(scale_x);
        pp->appsrt->view_scale_x = scale_x;
        scale_y = pp->appsrt->vmmtx[0][1] * pp->appsrt->vmmtx[0][1] +
                  pp->appsrt->vmmtx[1][1] * pp->appsrt->vmmtx[1][1] +
                  pp->appsrt->vmmtx[2][1] * pp->appsrt->vmmtx[2][1];
        scale_y = sqrtf(scale_y);
        pp->appsrt->view_scale_y = scale_y;
        if (pp->appsrt->xA2 != 0) {
            PSMTXIdentity(draw_mtx);
            draw_mtx[0][3] = pp->appsrt->translate.x;
            draw_mtx[1][3] = pp->appsrt->translate.y;
            draw_mtx[2][3] = pp->appsrt->translate.z;
            PSMTXConcat(vmtx, draw_mtx, draw_mtx);
            HSD_MtxGetScale(draw_mtx, &scratch_scale);
            PSMTXScale(pp->appsrt->vmmtx, scratch_scale.x,
                       scratch_scale.y, scratch_scale.z);
            pp->appsrt->vmmtx[0][3] = draw_mtx[0][3];
            pp->appsrt->vmmtx[1][3] = draw_mtx[1][3];
            pp->appsrt->vmmtx[2][3] = draw_mtx[2][3];
        }
        pp->appsrt->frameNum = psFrameNum;
    }
    {
        f32 pos_x;
        f32 pos_y;

        PSMTXCopy(pp->appsrt->vmmtx, draw_mtx);
        cur_x = draw_mtx[0][3] + (draw_mtx[0][2] * pp->pos.z +
                                  (draw_mtx[0][0] * (pos_x = pp->pos.x) +
                                   draw_mtx[0][1] * (pos_y = pp->pos.y)));
        cur_y = draw_mtx[1][3] +
                (draw_mtx[1][2] * pp->pos.z +
                 (draw_mtx[1][0] * pos_x + draw_mtx[1][1] * pos_y));
        cur_z = draw_mtx[2][3] +
                (draw_mtx[2][2] * pp->pos.z +
                 (draw_mtx[2][0] * pos_x + draw_mtx[2][1] * pos_y));
        if (pp->kind & Tornado) {
            f32 x;
            f32 y;
            f32 z;

            calcTornadoLastPos(pp, &x, &y, &z);
            prev_pos_x =
                draw_mtx[0][3] + (draw_mtx[0][2] * z +
                                  (draw_mtx[0][0] * x + draw_mtx[0][1] * y));
            prev_pos_y =
                draw_mtx[1][3] + (draw_mtx[1][2] * z +
                                  (draw_mtx[1][0] * x + draw_mtx[1][1] * y));
            prev_pos_z =
                draw_mtx[2][3] + (draw_mtx[2][2] * z +
                                  (draw_mtx[2][0] * x + draw_mtx[2][1] * y));
        } else {
            f32 dz = pp->pos.z - pp->vel.z;
            f32 dx = pp->pos.x - pp->vel.x;
            f32 dy = pp->pos.y - pp->vel.y;

            prev_pos_x =
                draw_mtx[0][3] + (draw_mtx[0][2] * dz +
                                  (draw_mtx[0][0] * dx + draw_mtx[0][1] * dy));
            prev_pos_y =
                draw_mtx[1][3] + (draw_mtx[1][2] * dz +
                                  (draw_mtx[1][0] * dx + draw_mtx[1][1] * dy));
            prev_pos_z =
                draw_mtx[2][3] + (draw_mtx[2][2] * dz +
                                  (draw_mtx[2][0] * dx + draw_mtx[2][1] * dy));
        }
        psScaleAppSRTAxes(pp, draw_mtx);
        (void) &pad;
    }
    x_extent = pp->appsrt->view_scale_x * pp->size;
    y_extent = pp->appsrt->view_scale_y * pp->size;
    if (it == NULL) {
        ay = y_extent;
        by = -ay;
        ax = bx = x_extent;
    } else {
        ay = 0.0f;
        ax = x_extent;
        by = -y_extent;
        bx = ay;
    }
    if ((pp->kind & Trail) || (pp->kind & DirVec)) {
        f32 vf1;
        f32 vf2;
        f32 m00 = pp->appsrt->vmmtx[0][0];
        f32 m01 = pp->appsrt->vmmtx[0][1];
        f32 m02 = pp->appsrt->vmmtx[0][2];
        f32 m03 = pp->appsrt->vmmtx[0][3];
        f32 m10 = pp->appsrt->vmmtx[1][0];
        f32 m11 = pp->appsrt->vmmtx[1][1];
        f32 m12 = pp->appsrt->vmmtx[1][2];
        f32 m13 = pp->appsrt->vmmtx[1][3];
        f32 m20 = pp->appsrt->vmmtx[2][0];
        f32 m21 = pp->appsrt->vmmtx[2][1];
        f32 m22 = pp->appsrt->vmmtx[2][2];
        f32 m23 = pp->appsrt->vmmtx[2][3];
        if (0.0f == prj[0]) {
            f32 prev_x;
            f32 prev_y;
            f32 prev_z;

            if (pp->kind & Tornado) {
                calcTornadoLastPos(pp, &prev_x, &prev_y, &prev_z);
            } else {
                prev_x = pp->pos.x - pp->vel.x;
                prev_y = pp->pos.y - pp->vel.y;
                prev_z = pp->pos.z - pp->vel.z;
            }
            w0 = pp->appsrt->vmmtx[2][3] +
                 (pp->appsrt->vmmtx[2][2] * pp->pos.z +
                  (pp->appsrt->vmmtx[2][0] * pp->pos.x + pp->appsrt->vmmtx[2][1] * pp->pos.y));
            pm00 = prj[1] * pp->appsrt->vmmtx[0][0] + prj[2] * pp->appsrt->vmmtx[2][0];
            pm01 = prj[1] * pp->appsrt->vmmtx[0][1] + prj[2] * pp->appsrt->vmmtx[2][1];
            pm02 = prj[1] * pp->appsrt->vmmtx[0][2] + prj[2] * pp->appsrt->vmmtx[2][2];
            pm03 = prj[1] * pp->appsrt->vmmtx[0][3] + prj[2] * pp->appsrt->vmmtx[2][3];
            pm10 = prj[3] * pp->appsrt->vmmtx[1][0] + prj[4] * pp->appsrt->vmmtx[2][0];
            pm11 = prj[3] * pp->appsrt->vmmtx[1][1] + prj[4] * pp->appsrt->vmmtx[2][1];
            pm12 = prj[3] * pp->appsrt->vmmtx[1][2] + prj[4] * pp->appsrt->vmmtx[2][2];
            pm13 = prj[3] * pp->appsrt->vmmtx[1][3] + prj[4] * pp->appsrt->vmmtx[2][3];
            w0inv = -1.0f / w0;
            w1 = pp->appsrt->vmmtx[2][3] +
                 (pp->appsrt->vmmtx[2][2] * prev_z +
                  (pp->appsrt->vmmtx[2][0] * prev_x + pp->appsrt->vmmtx[2][1] * prev_y));
            w1inv = -1.0f / w1;
            vf1 = w0inv * (pm03 + (pm02 * pp->pos.z +
                                  (pm00 * pp->pos.x + pm01 * pp->pos.y))) -
                  w1inv *
                      (pm03 + (pm02 * prev_z + (pm00 * prev_x + pm01 * prev_y)));
            vf2 =
                w0inv * (pm13 + (pm12 * pp->pos.z +
                                (pm10 * pp->pos.x + pm11 * pp->pos.y))) -
                w1inv * (pm13 + (pm12 * prev_z + (pm10 * prev_x + pm11 * prev_y)));
        } else {
            om00 = prj[1] * pp->appsrt->vmmtx[0][0] + prj[2];
            om01 = prj[1] * pp->appsrt->vmmtx[0][1] + prj[2];
            om02 = prj[1] * pp->appsrt->vmmtx[0][2] + prj[2];
            om10 = prj[3] * pp->appsrt->vmmtx[1][0] + prj[4];
            om11 = prj[3] * pp->appsrt->vmmtx[1][1] + prj[4];
            om12 = prj[3] * pp->appsrt->vmmtx[1][2] + prj[4];
            if (pp->kind & Tornado) {
                f32 tx;
                f32 ty;
                f32 tz;
                calcTornadoLastPos(pp, &tx, &ty, &tz);
                {
                    f32 dx;
                    f32 dy;
                    f32 dz;

                    dy = pp->pos.y - ty;
                    dx = pp->pos.x - tx;
                    dz = pp->pos.z - tz;
                    vf1 = om02 * dz + (om00 * dx + om01 * dy);
                    vf2 = om12 * dz + (om10 * dx + om11 * dy);
                }
            } else {
                f32 vz;
                f32 vx;
                f32 vy;

                vy = pp->vel.y;
                vx = pp->vel.x;
                vz = pp->vel.z;
                vf1 = om02 * vz + (om00 * vx + om01 * vy);
                vf2 = om12 * vz + (om10 * vx + om11 * vy);
            }
        }
        if (psMaskAbsLtF32(vf2, FLT_MIN)) {
            angle = (-vf1 >= 0.0f) ? 1.5707964f : -1.5707964f;
        } else {
            angle = atan2f(-vf1, vf2);
        }
        if (pp->kind & DirVec) {
            angle += pp->rotate;
        }
    } else {
        angle = pp->rotate;
    }
    if (psMaskAbsGtF32(angle, 0.01)) {
        f32 c = cosf(angle);
        f32 s = sinf(angle);
        f32 old_x = ax;
        ax = c * ax - s * ay;
        ay = s * old_x + c * ay;
        old_x = bx;
        bx = c * bx - s * by;
        by = s * old_x + c * by;
    }
    psSetCurrentMtx(GX_PNMTX1);
    if (pp->kind & Trail) {
        f32 xl;
        f32 yl;
        f32 zl;
        GXColor draw_color;

        getClrTrail(pp, &draw_color);
        if (it == NULL) {
            if (pp->kind & DispTexture) {
                setVtxDesc(2);
                GXBegin(GX_QUADS, GX_VTXFMT2, 4U);
            } else {
                setVtxDesc(3);
                GXBegin(GX_QUADS, GX_VTXFMT3, 4U);
            }
            GXPosition3f32(-ax + prev_pos_x, -ay + prev_pos_y, prev_pos_z);
            GXColor4u8(draw_color.r, draw_color.g, draw_color.b,
                       (u8) ((f32) draw_color.a * pp->trail));
            if (pp->kind & DispTexture) {
                GXTexCoord1x8((pp->kind >> 16) & 0xC);
            }
            GXPosition3f32(-bx + cur_x, -by + cur_y, cur_z);
            GXColor4u8(draw_color.r, draw_color.g, draw_color.b, draw_color.a);
            if (pp->kind & DispTexture) {
                GXTexCoord1x8(((pp->kind >> 16) & 0xC) + 1);
            }
            GXPosition3f32(ax + cur_x, ay + cur_y, cur_z);
            GXColor4u8(draw_color.r, draw_color.g, draw_color.b, draw_color.a);
            if (pp->kind & DispTexture) {
                GXTexCoord1x8(((pp->kind >> 16) & 0xC) + 2);
            }
            GXPosition3f32(bx + prev_pos_x, by + prev_pos_y, prev_pos_z);
            GXColor4u8(draw_color.r, draw_color.g, draw_color.b,
                       (u8) ((f32) draw_color.a * pp->trail));
            if (pp->kind & DispTexture) {
                GXTexCoord1x8(((pp->kind >> 16) & 0xC) + 3);
            }
        } else {
            f32 trail_alpha = 255.0f * (1.0f - pp->trail);
            f32 axis_len = sqrtf(bx * bx + by * by);

            if (!psMaskAbsLtF32(axis_len, FLT_MIN)) {
                f32 dx = cur_x - prev_pos_x;
                f32 dy = cur_y - prev_pos_y;
                f32 dz = cur_z - prev_pos_z;
                u32 primitive_count;

                xl = dx * dx;
                yl = dy * dy;
                zl = dz * dz;
                axis_len = sqrtf(zl + (xl + yl)) / axis_len;
                if (axis_len < 1.0f) {
                    axis_len = 1.0f;
                }
                primitive_count = *(u32*) it;

                bx *= axis_len;
                by *= axis_len;
                it += sizeof(u32);
                for (; primitive_count != 0; primitive_count--) {
                    GXPrimitive primitive = it[0];
                    u8 count = it[1];
                    s32 i;

                    it += 4;
                    if (pp->kind & DispTexture) {
                        setVtxDesc(5);
                        GXBegin(primitive, GX_VTXFMT5, count);
                    } else {
                        setVtxDesc(3);
                        GXBegin(primitive, GX_VTXFMT3, count);
                    }
                    for (i = count; i > 0; i--) {
                        f32 s = *(f32*) &it[0];
                        f32 sx = 2.0f * (s - 0.5f);
                        f32 t;
                        f32 tx;
                        s32 converted_alpha;
                        s32 alpha;

                        if (pp->kind & TexFlipS) {
                            s = 1.0f - s;
                        }
                        t = *(f32*) &it[4];
                        it += 8;
                        alpha = (s32) (255.0f - t * trail_alpha);
                        converted_alpha = (s32) (255.0f - t * trail_alpha);
                        if (converted_alpha < 0) {
                            alpha = 0;
                        }
                        if (alpha > 0xFF) {
                            alpha = 0xFF;
                        }
                        tx = 2.0f * (t - 0.5f);
                        if (pp->kind & TexFlipT) {
                            t = 1.0f - t;
                        }
                        GXPosition3f32(cur_x + ax * sx + bx * tx,
                                       cur_y + ay * sx + by * tx, cur_z);
                        GXColor4u8(draw_color.r, draw_color.g, draw_color.b,
                                   (u8) alpha);
                        if (pp->kind & DispTexture) {
                            GXWGFifo.f32 = s;
                            GXWGFifo.f32 = t;
                        }
                    }
                }
            }
        }
    } else if (it == NULL) {
        if (pp->kind & DispTexture) {
            setVtxDesc(0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4U);
        } else {
            setVtxDesc(1);
            GXBegin(GX_QUADS, GX_VTXFMT1, 4U);
        }
        GXWGFifo.f32 = -ax + cur_x;
        GXWGFifo.f32 = -ay + cur_y;
        GXWGFifo.f32 = cur_z;
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = (pp->kind >> 16) & 0xC;
        }
        GXWGFifo.f32 = -bx + cur_x;
        GXWGFifo.f32 = -by + cur_y;
        GXWGFifo.f32 = cur_z;
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 1;
        }
        GXWGFifo.f32 = ax + cur_x;
        GXWGFifo.f32 = ay + cur_y;
        GXWGFifo.f32 = cur_z;
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 2;
        }
        GXWGFifo.f32 = bx + cur_x;
        GXWGFifo.f32 = by + cur_y;
        GXWGFifo.f32 = cur_z;
        if (pp->kind & DispTexture) {
            GXWGFifo.u8 = ((pp->kind >> 16) & 0xC) + 3;
        }
    } else {
        u32 primitive_count = *(u32*) it;

        it += sizeof(u32);
        for (; primitive_count != 0; primitive_count--) {
            GXPrimitive prim = it[0];
            u8 count = it[1];
            s32 i;
            it += 4;
            if (pp->kind & DispTexture) {
                setVtxDesc(4);
                GXBegin(prim, GX_VTXFMT4, count);
            } else {
                setVtxDesc(1);
                GXBegin(prim, GX_VTXFMT1, count);
            }
            for (i = count; i > 0; i--) {
                f32 s = *(f32*) &it[0];
                f32 sx = 2.0f * (s - 0.5f);
                f32 t;
                f32 tx;

                if (pp->kind & TexFlipS) {
                    s = 1.0f - s;
                }
                t = *(f32*) &it[4];
                it += 8;
                tx = 2.0f * (t - 0.5f);
                if (pp->kind & TexFlipT) {
                    t = 1.0f - t;
                }
                GXPosition2f32(cur_x + ax * sx + bx * tx, cur_y + ay * sx + by * tx);
 GXWGFifo.f32 = cur_z;
                if (pp->kind & DispTexture) {
                    GXWGFifo.f32 = s;
                    GXWGFifo.f32 = t;
                }
            }
        }
    }
}

static inline void psUpdateBillboardAxes(const Mtx inv_view)
{
    f32 right_x;
    f32 up_x;
    f32 right_y;
    f32 up_y;
    f32 right_z;
    f32 up_z;

    right_x = inv_view[0][0];
    up_x = inv_view[0][1];
    rx0 = right_x + up_x;
    rx1 = right_x - up_x;
    right_y = inv_view[1][0];
    up_y = inv_view[1][1];
    ry0 = right_y + up_y;
    ry1 = right_y - up_y;
    right_z = inv_view[2][0];
    up_z = inv_view[2][1];
    rz0 = right_z + up_z;
    rz1 = right_z - up_z;
}

#if 1
#pragma push
#pragma inline_depth(3)
#endif
void psDispParticles(u32 target_link, u32 sw)
{
    s32 linkNo;
    u8* prevTex;
    u32 prevTexEdge;
    u32 prevMirror;
    u8 prevACmpParam1;
    u8 prevACmpParam2;
    s32 needs_setup;
    u16* prevLut;
    HSD_TlutDesc tlutdesc;
    void *lut;
    GXTexObj texObj;
    HSD_Particle* sorted_particles;
    HSD_Particle* non_edge_particles;
    s32 alpha_compare_mode;
    s32 prev_tex_interp_near;
    u32 prev_kind;
    HSD_Particle* pp;

    alpha_compare_mode = 0;
    prev_tex_interp_near = 0;
    prevACmpParam1 = 0;
    prevACmpParam2 = 0xFF;
    needs_setup = 1;
    if (sw == 0) {
        if (psFrameNum < 0xFF) {
            psFrameNum += 1;
            return;
        }
        psFrameNum = 1;
        return;
    }
    for (linkNo = 0; linkNo < 16; linkNo++) {
        if (target_link & (1 << linkNo)) {
            particleSort(linkNo, &sorted_particles, &non_edge_particles);
            switch (sw) {
            case 1:
                pp = sorted_particles;
                break;
            case 2:
                pp = non_edge_particles;
                break;
            default:
                continue;
            }
            while (pp != NULL) {
                HSD_PSTexGroup* tex_group = NULL;
                HSD_PSFormGroup* form_group = NULL;
                u8* form = NULL;
                void* image;
                u32 blend_mode;
                u8 alpha0;
                u8 alpha1;
                GXTexWrapMode wrap_s;
                f32 scale_s;
                f32 scale_t;
                GXTexFmt fmt;
                u8** tex_table;
                u32 width;
                u32 height;
                GXTexWrapMode wrap_t;

                if ((sw == 1) && !(pp->kind & TexEdge)) {
                    break;
                }
                if (!(pp->size < FLT_EPSILON)) {
                    if (needs_setup != 0) {
                        prevLut = NULL;
                        prevPointSize = -1;
                        prevTex = NULL;
                        prevLineWidth = -1;
                        prevChanCtrl = -1;
                        psSetupTevInvalidState();
                        prevMirror = (u32) -1;
                        prev_kind &= ~DispFog;
                        prevTexEdge = (u32) -1;
                        HSD_FogSet(NULL);
                        prevChanMat.r = prevChanMat.g = prevChanMat.b = 0xFF;
                        prevChanAmb.r = prevChanAmb.g = prevChanAmb.b = 0xFF;
                        prevChanMat.a = prevChanAmb.a = 0xFF;
                        GXSetChanMatColor(GX_COLOR0A0, prevChanMat);
                        GXSetChanAmbColor(GX_COLOR0A0, prevChanAmb);
                        psSetupTevInvalidState();
                        psSetupTevCommon();
                        psSetColor(&prevColorPrim, 0xFF);
                        psSetColor(&prevColorEnv, 0);
                        psSetColor(&prevColorMat, 0xFF);
                        GXSetTevColor(GX_TEVREG0, prevColorPrim);
                        GXSetTevColor(GX_TEVREG1, prevColorEnv);
                        GXSetTevColor(GX_TEVREG2, prevColorMat);
                        prevBlendMode = -1;
                        GXSetZCompLoc(GX_FALSE);
                        HSD_CObjGetViewingMtx(HSD_CObjGetCurrent(), vmtx);
                        PSMTXInverse(vmtx, rvmtx);
GXGetProjectionv(prj);
if (0.0f == prj[0]) {
pvmtx[0][0] = prj[1] * vmtx[0][0] + prj[2] * vmtx[2][0];
pvmtx[0][1] = prj[1] * vmtx[0][1] + prj[2] * vmtx[2][1];
pvmtx[0][2] = prj[1] * vmtx[0][2] + prj[2] * vmtx[2][2];
pvmtx[0][3] = prj[1] * vmtx[0][3] + prj[2] * vmtx[2][3];
pvmtx[1][0] = prj[3] * vmtx[1][0] + prj[4] * vmtx[2][0];
pvmtx[1][1] = prj[3] * vmtx[1][1] + prj[4] * vmtx[2][1];
pvmtx[1][2] = prj[3] * vmtx[1][2] + prj[4] * vmtx[2][2];
pvmtx[1][3] = prj[3] * vmtx[1][3] + prj[4] * vmtx[2][3];
} else {
pvmtx[0][0] = prj[1] * vmtx[0][0] + prj[2];
pvmtx[0][1] = prj[1] * vmtx[0][1] + prj[2];
pvmtx[0][2] = prj[1] * vmtx[0][2] + prj[2];
pvmtx[0][3] = prj[1] * vmtx[0][3] + prj[2];
pvmtx[1][0] = prj[3] * vmtx[1][0] + prj[4];
pvmtx[1][1] = prj[3] * vmtx[1][1] + prj[4];
pvmtx[1][2] = prj[3] * vmtx[1][2] + prj[4];
pvmtx[1][3] = prj[3] * vmtx[1][3] + prj[4];
}
psUpdateBillboardAxes(*(const Mtx*) rvmtx);
                        GXLoadPosMtxImm(vmtx, GX_PNMTX0);
                        {
                            psdisp_Mtx billboard_mtx = {{{1.0F, 0.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 1.0F, 0.0F}}};
                            f32 pad;
                            GXLoadPosMtxImm(billboard_mtx.mtx, GX_PNMTX1);
                        }
                        currentMtx = GX_PNMTX1;
                        psSetCurrentMtx(GX_PNMTX0);
                        GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
                        GXSetCullMode(GX_CULL_BACK);
                        GXSetArray(GX_VA_TEX0, billboard_tex0_u8, 2);
                        psSetupVtxFormat(GX_VTXFMT0, 0, 1, GX_RGB565);
                        psSetupVtxFormat(GX_VTXFMT1, 0, 0, GX_RGB565);
                        psSetupVtxFormat(GX_VTXFMT2, 1, 1, GX_RGB565);
                        psSetupVtxFormat(GX_VTXFMT3, 1, 0, GX_RGB565);
                        psSetupVtxFormat(GX_VTXFMT4, 0, 1, GX_RGBA6);
                        psSetupVtxFormat(GX_VTXFMT5, 1, 1, GX_RGBA6);
                        needs_setup = 0;
                    }

                    blend_mode = (pp->kind >> 0x16U) & 3;
                    setBlendMode(blend_mode);

                    if (pp->aCmpCount != 0) {
                        s32 scale = (65536 * pp->aCmpRemain) / pp->aCmpCount;
                        alpha0 = ((pp->aCmpParam1Target << 16) +
                                  scale * (pp->aCmpParam1 -
                                           pp->aCmpParam1Target)) >>
                                 16;
                        alpha1 = ((pp->aCmpParam2Target << 16) +
                                  scale * (pp->aCmpParam2 -
                                           pp->aCmpParam2Target)) >>
                                 16;
                    } else {
                        alpha0 = pp->aCmpParam1;
                        alpha1 = pp->aCmpParam2;
                    }
                    if ((alpha_compare_mode != pp->aCmpMode) ||
                        (prevACmpParam1 != alpha0) ||
                        (prevACmpParam2 != alpha1))
                    {
                        prevACmpParam1 = alpha0;
                        alpha_compare_mode = pp->aCmpMode;
                        prevACmpParam2 = alpha1;
                        GXSetAlphaCompare(
                            (alpha_compare_mode >> 3) & 7, prevACmpParam1,
                            (alpha_compare_mode >> 6) & 3,
                            alpha_compare_mode & 7, prevACmpParam2);
                    }

                    psSetupTev(pp);
                    setupChanCtrl(pp);
                    setupChanReg(pp);
                    setupTevReg(pp);
                    if ((pp->kind & TexEdge) != prevTexEdge) {
                        prevTexEdge = pp->kind & TexEdge;
                        if ((s32) prevTexEdge != 0) {
                            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                        } else {
                            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
                        }
                    }
                    if (((pp->kind ^ prev_kind) & DispFog) != 0) {
                        if (pp->kind & DispFog) {
                            HSD_FogSet(particleFog);
                        } else {
                            HSD_FogSet(NULL);
                        }
                    }

                    if (psFormGroupArray[pp->bank] != NULL &&
                        (form_group = psFormGroupArray[pp->bank][pp->texGroup]) !=
                            NULL
#if 1
                        && form_group->formTable != NULL
#endif
                    )
                    {
                        form = form_group->formTable[pp->poseNum];
                    } else {
                        form = NULL;
                    }

                    if (pp->kind & DispTexture) {
                        GXTlutObj gx_tlut_obj;
                        f32 pad1[2];
                        if (pp->kind & MirrorS) {
                            scale_s = 2.0f;
                            wrap_s = GX_MIRROR;
                        } else {
                            scale_s = 1.0f;
                            wrap_s = GX_CLAMP;
                        }
                        if (pp->kind & MirrorT) {
                            scale_t = 2.0f;
                            wrap_t = GX_MIRROR;
                        } else {
                            scale_t = 1.0f;
                            wrap_t = GX_CLAMP;
                        }
                        if ((pp->kind & (MirrorS | MirrorT)) != prevMirror) {
                            Mtx temp_mtx;

                            prevMirror = pp->kind & (MirrorS | MirrorT);
                            prevTex = NULL;
                            PSMTXScale(temp_mtx, scale_s, scale_t, 1.0f);
                            if (pp->kind & MirrorT) {
                                temp_mtx[1][3] = 1.0f;
                            }
                            GXLoadTexMtxImm(temp_mtx, GX_TEXMTX0, GX_MTX2x4);
                            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4,
                                              GX_TG_TEX0, GX_TEXMTX0, GX_FALSE,
                                              GX_PTIDENTITY);
                        }
                        tex_group = psTexGroupArray[pp->bank][pp->texGroup];
                        if (tex_group != NULL) {
                            fmt = tex_group->fmt;
                            tex_table = tex_group->texTable;
                            width = tex_group->width;
                            height = tex_group->height;
                        } else {
                            fmt = 0;
                            height = 0;
                            width = 0;
                            tex_table = NULL;
                        }
                        if (tex_table != NULL) {
                            image = tex_table[pp->poseNum];
                        } else {
                            image = NULL;
                        }
                        if ((fmt == GX_TF_C4) || (fmt == GX_TF_C8)) {
                            if (tex_table != NULL) {
                                void** palettes =
                                    (void**) &tex_table[tex_group->num];
                                if (palettes != NULL) {
                                    if (pp->palNum != 0xFF) {
                                        lut = palettes[pp->palNum];
                                    } else if (!(pp->kind & ComTLUT)) {
                                        lut = palettes[pp->poseNum];
                                    } else {
                                        lut = palettes[0];
                                    }
                                    if (lut != prevLut) {
                                        tlutdesc.fmt = tex_group->tlutfmt;
                                        tlutdesc.tlut_name = GX_TLUT0;
                                        tlutdesc.n_entries =
                                            (fmt == GX_TF_C4) ? 16 : 256;
                                        GXInitTlutObj(
                                            &gx_tlut_obj, lut,
                                            tlutdesc.fmt, tlutdesc.n_entries);
                                        GXLoadTlut(&gx_tlut_obj,
                                                   tlutdesc.tlut_name);
                                        (void) &tlutdesc;
                                    }
                                    prevTex = NULL;
                                }
                            }
                        }
                        if ((prevTex != image) && (image != NULL)) {
                            prevTex = image;
                            switch (fmt) {
                            case GX_TF_C4:
                            case GX_TF_C8:
                                GXInitTexObjCI(&texObj, image, width, height,
                                               fmt, wrap_s, wrap_t, GX_FALSE,
                                               GX_TLUT0);
                                break;
                            case GX_TF_I4:
                            case GX_TF_I8:
                            case GX_TF_IA4:
                            case GX_TF_IA8:
                            case GX_TF_RGB565:
                            case GX_TF_RGB5A3:
                            case GX_TF_RGBA8:
                            case GX_TF_CMPR:
                                GXInitTexObj(&texObj, image, width, height,
                                             fmt, wrap_s, wrap_t, GX_FALSE);
                                break;
                            default:
                                __assert("psdisp.c", 1986, "0");
                                break;
                            }
                            prev_tex_interp_near = pp->kind & TexInterpNear;
                            GXInitTexObjLOD(
                                &texObj,
                                (prev_tex_interp_near != 0) ? GX_NEAR
                                                            : GX_LINEAR,
                                (pp->kind & TexInterpNear) ? GX_NEAR
                                                           : GX_LINEAR,
                                0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                                GX_ANISO_1);
                            GXLoadTexObj(&texObj, GX_TEXMAP0);
                        }
                        if ((u32) prev_tex_interp_near !=
                            (pp->kind & TexInterpNear))
                        {
                            prev_tex_interp_near = pp->kind & TexInterpNear;
                            GXInitTexObjLOD(
                                &texObj,
                                (prev_tex_interp_near != 0) ? GX_NEAR
                                                            : GX_LINEAR,
                                (s32) (pp->kind & TexInterpNear) != 0
                                    ? GX_NEAR
                                    : GX_LINEAR,
                                0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                                GX_ANISO_1);
                            GXLoadTexObj(&texObj, GX_TEXMAP0);
                        }
                    }

                    if (pp->kind & DispPoint) {
                        if (pp->appsrt != NULL) {
                            psDispSubAPPSRTPoint(pp);
                        } else {
                            if (pp->kind & Trail) {
                                psDispSubPointTrail(pp);
                            } else {
                                psDispSubPoint(pp);
                            }
                        }
                    } else if (pp->appsrt != NULL) {
                        psDispSubAppSRT(pp, form);
                    } else {
                        psDispSub(pp, form);
                    }
                }

                prev_kind = pp->kind;
                pp = pp->next;
            }
        }
    }
    if (needs_setup == 0) {
        HSD_StateInvalidate(-1);
    }
}
#if 1
#pragma pop
#endif

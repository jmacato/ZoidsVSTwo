/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/psdisp.c. */
#include "particle.h"
extern f32 sinf(f32 value);
extern f32 cosf(f32 value);
extern f32 tanf(f32 value);
#define GX_VA_POS 9
#define GX_VA_CLR0 11
#define GX_VA_TEX0 13
#define GX_DIRECT 1
#define GX_INDEX8 2
void setVtxDesc(s32 fmt)
{
    GXClearVtxDesc();
    switch (fmt) {
    case 0:
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        return;
    case 1:
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        return;
    case 2:
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        return;
    case 3:
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        return;
    case 4:
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        return;
    case 5:
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        return;
    }
}

void calcTornadoLastPos(HSD_Particle *pp, f32 *x, f32 *y, f32 *z)
{
    f32 radius;
    f32 px, py, pz;
    f32 sina, sinb, cosa, cosb;
    f32 vx0, vz0;
    HSD_Generator *gp;

    gp = pp->gen;

    if (gp == 0) {
        *x = pp->pos.x;
        *y = pp->pos.y;
        *z = pp->pos.z;
        return;
    }

    sina = sinf(pp->grav);
    sinb = sinf(pp->fric);
    cosa = cosf(pp->grav);
    cosb = cosf(pp->fric);

    vz0 = pp->vel.z - gp->aux.tornado.vel;
    vx0 = pp->vel.x - gp->grav;

    radius = (gp->radius < 0.0F ? -gp->radius : gp->radius);
    radius += vz0 * tanf((gp->angle < 0.0F ? -gp->angle : gp->angle));
    radius *= pp->vel.y;
    px = radius * cosf(vx0);
    py = radius * sinf(vx0);
    pz = vz0;

    *x = px * cosb + pz * sinb + gp->pos.x;
    *y = -px * sina * sinb + py * cosa + pz * sina * cosb + gp->pos.y;
    *z = -px * cosa * sinb - py * sina + pz * cosa * cosb + gp->pos.z;
}

void getColorPrimEnv(HSD_Particle *pp, GXColor *primCol, GXColor *envCol)
{
    if (pp->primColCount) {
        int scale = 65536 * pp->primColRemain / pp->primColCount;
        primCol->r = ((pp->primColTarget.r << 16) +
                      (pp->primCol.r - pp->primColTarget.r) * scale) >>
                     16;
        primCol->g = ((pp->primColTarget.g << 16) +
                      (pp->primCol.g - pp->primColTarget.g) * scale) >>
                     16;
        primCol->b = ((pp->primColTarget.b << 16) +
                      (pp->primCol.b - pp->primColTarget.b) * scale) >>
                     16;
        primCol->a = ((pp->primColTarget.a << 16) +
                      (pp->primCol.a - pp->primColTarget.a) * scale) >>
                     16;
    } else {
        *primCol = pp->primCol;
    }
    if (pp->envColCount) {
        int scale = 65536 * pp->envColRemain / pp->envColCount;
        envCol->r = ((pp->envColTarget.r << 16) +
                     (pp->envCol.r - pp->envColTarget.r) * scale) >>
                    16;
        envCol->g = ((pp->envColTarget.g << 16) +
                     (pp->envCol.g - pp->envColTarget.g) * scale) >>
                    16;
        envCol->b = ((pp->envColTarget.b << 16) +
                     (pp->envCol.b - pp->envColTarget.b) * scale) >>
                    16;
        envCol->a = ((pp->envColTarget.a << 16) +
                     (pp->envCol.a - pp->envColTarget.a) * scale) >>
                    16;
    } else {
        *envCol = pp->envCol;
    }
}

void getColorMatAmb(HSD_Particle *pp, GXColor *matCol, GXColor *ambCol)
{
    if (pp->matColCount) {
        int scale = 65536 * pp->matColRemain / pp->matColCount;
        u8 rgb =
            ((pp->matRGBTarget << 16) + (pp->matRGB - pp->matRGBTarget) * scale) >> 16;
        matCol->r = matCol->g = matCol->b = rgb;
        matCol->a =
            ((pp->matATarget << 16) + (pp->matA - pp->matATarget) * scale) >> 16;
    } else {
        matCol->r = matCol->g = matCol->b = pp->matRGB;
        matCol->a = pp->matA;
    }
    if (pp->ambColCount) {
        int scale = 65536 * pp->ambColRemain / pp->ambColCount;
        u8 rgb =
            ((pp->ambRGBTarget << 16) + (pp->ambRGB - pp->ambRGBTarget) * scale) >> 16;
        ambCol->r = ambCol->g = ambCol->b = rgb;
        ambCol->a =
            ((pp->ambATarget << 16) + (pp->ambA - pp->ambATarget) * scale) >> 16;
    } else {
        ambCol->r = ambCol->g = ambCol->b = pp->ambRGB;
        ambCol->a = pp->ambA;
    }
}

#include "particle.h"
#include "appsrt.h"
#include "jobj.h"
#include "random.h"

#define NULL 0


extern f64 __frsqrte(f64);
extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 tanf(f32);
extern void HSD_MtxSRT(Mtx m, Vec3 *scale, Vec3 *rot, Vec3 *trans, Vec3 *scale2);
extern void HSD_JObjSetupMatrixSub(HSD_JObj *jobj);
extern void genPosUpdate(HSD_Generator *gen);
extern void modifyDir(HSD_Particle *pp, f32 angle);
extern s32 applyForceJObj(HSD_Particle *pp, HSD_JObj *jobj, f32 force, f32 range);
extern int (**psCallback)(HSD_Particle *pp);

static u32 BYTEF32;

#define fval (*(f32 *)&BYTEF32)

static inline void psReadFloat(u8 **stream)
{
    u8 *p = *stream;
    ((u8 *)&BYTEF32)[0] = *p++;
    ((u8 *)&BYTEF32)[1] = *p++;
    ((u8 *)&BYTEF32)[2] = *p++;
    ((u8 *)&BYTEF32)[3] = *p++;
    *stream = p;
}

static inline void ref_INC(void *o)
{
    ((HSD_JObj *)o)->reference_count++;
    if (((HSD_JObj *)o)->reference_count == (u16)-1) {
        __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}

static inline void HSD_JObjRef(HSD_JObj *jobj)
{
    if (jobj != NULL) {
        ref_INC(jobj);
    }
}

static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    s32 result;
    if (!jobj)
        __assert("jobj.h", 605, "jobj");
    result = 0;
    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40))
        result = 1;
    return result;
}

__attribute__((weak)) inline void HSD_JObjSetupMatrix(HSD_JObj *jobj)
{
    if (!jobj) {
        return;
    }
    if (HSD_JObjMtxIsDirty(jobj)) {
        HSD_JObjSetupMatrixSub(jobj);
    }
}

#define HSD_JObjSetMtxDirty(jobj)                                                  \
    do {                                                                           \
        if ((jobj) != NULL && !HSD_JObjMtxIsDirty(jobj))                           \
            HSD_JObjSetMtxDirtySub(jobj);                                          \
    } while (0)

static inline void HSD_JObjAddTranslationX(HSD_JObj *jobj, f32 x)
{
    if (jobj == NULL)
        __assert("jobj.h", 1109, "jobj");
    jobj->translation.x += x;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjAddTranslationY(HSD_JObj *jobj, f32 y)
{
    if (jobj == NULL)
        __assert("jobj.h", 1120, "jobj");
    jobj->translation.y += y;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void HSD_JObjAddTranslationZ(HSD_JObj *jobj, f32 z)
{
    if (jobj == NULL)
        __assert("jobj.h", 1131, "jobj");
    jobj->translation.z += z;
    if (!(jobj->flags & 0x02000000))
        HSD_JObjSetMtxDirty(jobj);
}

static inline void psEnableTexture(HSD_Particle *pp)
{
    HSD_PSTexGroup *tg = psTexGroupArray[pp->bank][pp->texGroup];
    if (tg != NULL && tg->texTable != NULL && tg->texTable[pp->poseNum] != NULL) {
        pp->kind |= DispTexture;
    }
}

#define COLOR_STEP(cur, tgt, count, remain)                                         \
    if ((count) != 0) {                                                            \
        s32 step = ((s32)(remain) << 16) / (s32)(count);                          \
        (cur).r = (u8)((((s32)(tgt).r << 16) + step * ((s32)(cur).r - (s32)(tgt).r)) >> 16); \
        (cur).g = (u8)((((s32)(tgt).g << 16) + step * ((s32)(cur).g - (s32)(tgt).g)) >> 16); \
        (cur).b = (u8)((((s32)(tgt).b << 16) + step * ((s32)(cur).b - (s32)(tgt).b)) >> 16); \
        (cur).a = (u8)((((s32)(tgt).a << 16) + step * ((s32)(cur).a - (s32)(tgt).a)) >> 16); \
    }

#define STEP2(c0, c1, t0, t1, count, remain)                                       \
    if ((count) != 0) {                                                            \
        s32 step = ((s32)(remain) << 16) / (s32)(count);                          \
        c0 = (u8)((((s32)t0 << 16) + step * ((s32)c0 - (s32)t0)) >> 16);           \
        c1 = (u8)((((s32)t1 << 16) + step * ((s32)c1 - (s32)t1)) >> 16);           \
    }

#define READ_COUNT(field)                                                          \
    {                                                                              \
        u8 *p = pc;                                                                \
        field = *p++;                                                              \
        {                                                                          \
            u16 cnt = field;                                                       \
            if (cnt & 0x80) {                                                      \
                cnt = ((cnt & 0x7F) << 8) + *p++;                                  \
                field = cnt;                                                       \
            }                                                                      \
        }                                                                          \
        pc = p;                                                                    \
    }

#define CLAMP_COL(dst, base, add)                                                  \
    {                                                                              \
        val = (f32)(base) + (add);                                               \
        if (val < 0.0F) {                                                            \
            val = 0.0F;                                                              \
        }                                                                          \
        if (val > 255.0F) {                                                          \
            val = 255.0F;                                                            \
        }                                                                          \
        dst = (u8)(s32)val;                                                          \
    }

#define RAND_COL(dst)                                                              \
    {                                                                              \
        f32 r = HSD_Randf();                                                       \
        s8 delta = (s8)*pc++;                                                      \
        r = (f32)(delta << 1) * r;                                                 \
        CLAMP_COL(dst, dst, r);                                                    \
    }

static inline void psGenPos(HSD_Generator *gen, HSD_Particle *pp)
{
    gen->pos.x = pp->pos.x;
    gen->pos.y = pp->pos.y;
    gen->pos.z = pp->pos.z;
    if (gen->appsrt != pp->appsrt) {
        if (pp->appsrt == NULL) {
            if (gen->appsrt != NULL) {
                genPosUpdate(gen);
                gen->pos.x -= gen->appsrt->translate.x;
                gen->pos.y -= gen->appsrt->translate.y;
                gen->pos.z -= gen->appsrt->translate.z;
            }
        } else {
            genPosUpdate(pp->appsrt->gp);
            if (gen->appsrt != NULL) {
                genPosUpdate(gen);
                gen->pos.x += pp->appsrt->translate.x - gen->appsrt->translate.x;
                gen->pos.y += pp->appsrt->translate.y - gen->appsrt->translate.y;
                gen->pos.z += pp->appsrt->translate.z - gen->appsrt->translate.z;
            } else {
                gen->pos.x += pp->appsrt->translate.x;
                gen->pos.y += pp->appsrt->translate.y;
                gen->pos.z += pp->appsrt->translate.z;
            }
        }
    }
}

static inline void psGenJObj(HSD_Generator *gen, HSD_Particle *pp)
{
    gen->idnum = pp->idnum;
    if (pp->gen != NULL) {
        HSD_JObj *jobj = pp->gen->jobj;
        if (gen != NULL) {
            gen->jobj = jobj;
            if (jobj != NULL) {
                HSD_JObjRef(jobj);
            }
        }
    }
    if (pp->appsrt != NULL) {
        psAttachGeneratorAppSRT(gen, pp->appsrt);
    }
}

static inline void psChildCommon(HSD_Particle *child, HSD_Particle *pp)
{
    child->idnum = pp->idnum;
    child->gen = pp->gen;
    if (pp->gen != NULL) {
        pp->gen->numChild++;
    }
}

HSD_Particle *psInterpretParticle0(HSD_Particle *pp, HSD_Particle *prev)
{
    u8 *pc;
    u16 operand;
    u8 opcode;
    u8 cls;
    HSD_Particle *child;
    HSD_Generator *gchild;
    HSD_Particle *next;
    f32 val;
    u8 pad_top[80];
    volatile f32 sqrt_res;
    u8 pad_mid[144];
    volatile f32 vel_res;
    volatile f32 dist_res;
    u8 pad_bot[256];

    if (pp->kind & 0x800) {
        return pp->next;
    }

    if (pp->sizeCount != 0) {
        pp->size += (pp->sizeTarget - pp->size) / (f32)pp->sizeCount;
        pp->sizeCount--;
    }

    if (pp->primColCount != 0) {
        pp->primColRemain--;
        if (pp->primColRemain == 0) {
            pp->primColCount = 0;
            pp->primCol.r = pp->primColTarget.r;
            pp->primCol.g = pp->primColTarget.g;
            pp->primCol.b = pp->primColTarget.b;
            pp->primCol.a = pp->primColTarget.a;
        }
    }

    if (pp->envColCount != 0) {
        pp->envColRemain--;
        if (pp->envColRemain == 0) {
            pp->envColCount = 0;
            pp->envCol.r = pp->envColTarget.r;
            pp->envCol.g = pp->envColTarget.g;
            pp->envCol.b = pp->envColTarget.b;
            pp->envCol.a = pp->envColTarget.a;
        }
    }

    if (pp->matColCount != 0) {
        pp->matColRemain--;
        if (pp->matColRemain == 0) {
            pp->matColCount = 0;
            pp->matRGB = pp->matRGBTarget;
            pp->matA = pp->matATarget;
        }
    }

    if (pp->ambColCount != 0) {
        pp->ambColRemain--;
        if (pp->ambColRemain == 0) {
            pp->ambColCount = 0;
            pp->ambRGB = pp->ambRGBTarget;
            pp->ambA = pp->ambATarget;
        }
    }

    if (pp->aCmpCount != 0) {
        pp->aCmpRemain--;
        if (pp->aCmpRemain == 0) {
            pp->aCmpCount = 0;
            pp->aCmpParam1 = pp->aCmpParam1Target;
            pp->aCmpParam2 = pp->aCmpParam2Target;
        }
    }

    if (pp->rotateCount != 0) {
        pp->rotate += (pp->rotateTarget - pp->rotate) / (f32)pp->rotateCount;
        pp->rotateCount--;
    }

    if (pp->cmdWait != 0 && --pp->cmdWait == 0) {
        operand = 0;
        pc = pp->cmdList + pp->cmdPtr;

        for (;;) {
            opcode = *pc++;

            if (opcode < 0x80) {
                operand = opcode & 0x1F;
                if (opcode & 0x20) {
                    operand = (operand << 8) + *pc++;
                }
                switch (opcode & 0xC0) {
                case 0x00:
                    break;
                case 0x40:
                    pp->poseNum = *pc++;
                    psEnableTexture(pp);
                    break;
                }
            } else {
                operand = 0;
                cls = opcode & 0xF8;
                if (cls > 0x98) {
                    cls = opcode & 0xF0;
                    if (cls != 0xC0 && cls != 0xD0) {
                        cls = opcode;
                    }
                }

                switch (cls) {
                case 0x80:
                    if (opcode & 1) {
                        psReadFloat(&pc);
                        pp->pos.x = fval;
                    }
                    if (opcode & 2) {
                        psReadFloat(&pc);
                        pp->pos.y = fval;
                    }
                    if (opcode & 4) {
                        psReadFloat(&pc);
                        pp->pos.z = fval;
                    }
                    break;

                case 0x88:
                    if (opcode & 1) {
                        psReadFloat(&pc);
                        pp->pos.x += fval;
                    }
                    if (opcode & 2) {
                        psReadFloat(&pc);
                        pp->pos.y += fval;
                    }
                    if (opcode & 4) {
                        psReadFloat(&pc);
                        pp->pos.z += fval;
                    }
                    break;

                case 0x90:
                    if (opcode & 1) {
                        psReadFloat(&pc);
                        pp->vel.x = fval;
                    }
                    if (opcode & 2) {
                        psReadFloat(&pc);
                        pp->vel.y = fval;
                    }
                    if (opcode & 4) {
                        psReadFloat(&pc);
                        pp->vel.z = fval;
                    }
                    break;

                case 0x98:
                    if (opcode & 1) {
                        psReadFloat(&pc);
                        pp->vel.x += fval;
                    }
                    if (opcode & 2) {
                        psReadFloat(&pc);
                        pp->vel.y += fval;
                    }
                    if (opcode & 4) {
                        psReadFloat(&pc);
                        pp->vel.z += fval;
                    }
                    break;

                case 0xA0:
                    READ_COUNT(pp->sizeCount);
                    psReadFloat(&pc);
                    pp->sizeTarget = fval;
                    if (pp->sizeCount == 0) {
                        pp->size = pp->sizeTarget;
                    }
                    break;

                case 0xA1:
                    pp->kind &= ~DispTexture;
                    break;

                case 0xA2:
                    psReadFloat(&pc);
                    pp->grav = fval;
                    if (pp->grav == 0.0F) {
                        pp->kind &= ~ApplyGrav;
                    } else {
                        pp->kind |= ApplyGrav;
                    }
                    break;

                case 0xA3:
                    psReadFloat(&pc);
                    pp->fric = fval;
                    if (pp->fric == 1.0F) {
                        pp->kind &= ~ApplyFric;
                    } else {
                        pp->kind |= ApplyFric;
                    }
                    break;

                case 0xA4: {
                    int linkNo = pp->linkNo;
                    int idx = *pc++ << 8;
                    idx += *pc++;
                    child = psGenerateParticleID0(&pp->next, linkNo, pp->bank, idx, 0);
                    if (child != NULL) {
                        psChildCommon(child, pp);
                        psAttachParticleAppSRT(child, pp->appsrt);
                        child->pos.x = pp->pos.x;
                        child->pos.y = pp->pos.y;
                        child->pos.z = pp->pos.z;
                        psInterpretParticle0(child, pp);
                    }
                    break;
                }

                case 0xF1: {
                    u8 bank = pp->bank;
                    int idx = pc[0] << 8;
                    idx += pc[1];
                    pc += 2;
                    if (ptclref[bank] != NULL) {
                        idx = ptclref[bank][idx];
                    }
                    child = psGenerateParticleID0(&pp->next, pp->linkNo, bank, idx, 0);
                    if (child != NULL) {
                        psChildCommon(child, pp);
                        psAttachParticleAppSRT(child, pp->appsrt);
                        child->pos.x = pp->pos.x;
                        child->pos.y = pp->pos.y;
                        child->pos.z = pp->pos.z;
                        psInterpretParticle0(child, pp);
                    }
                    break;
                }

                case 0xA5: {
                    int idx = pc[0] << 8;
                    idx += pc[1];
                    pc += 2;
                    gchild = psCreateGeneratorID(pp->linkNo, pp->bank, idx);
                    if (gchild != NULL) {
                        psGenJObj(gchild, pp);
                        psGenPos(gchild, pp);
                    }
                    break;
                }

                case 0xEF: {
                    int idx;
                    int flags;
                    idx = pc[0] << 8;
                    idx += pc[1];
                    pc += 2;
                    flags = *pc++;
                    gchild = psCreateGeneratorID(pp->linkNo, pp->bank, idx);
                    if (gchild != NULL) {
                        psGenJObj(gchild, pp);
                        gchild->kind &= 0xF1FFFFFF;
                        gchild->kind |= (flags << 25) & 0x0E000000;
                        psGenPos(gchild, pp);
                    }
                    break;
                }

                case 0xF0: {
                    int idx;
                    int flags;
                    u8 bank = pp->bank;
                    idx = pc[0] << 8;
                    idx += pc[1];
                    pc += 2;
                    flags = *pc++;
                    if (ptclref[bank] != NULL) {
                        idx = ptclref[bank][idx];
                    }
                    gchild = psCreateGeneratorID(pp->linkNo, bank, idx);
                    if (gchild != NULL) {
                        psGenJObj(gchild, pp);
                        gchild->kind &= 0xF1FFFFFF;
                        gchild->kind |= (flags << 25) & 0x0E000000;
                        psGenPos(gchild, pp);
                    }
                    break;
                }

                case 0xA6: {
                    int base, range;
                    base = *pc++ << 8;
                    base += *pc++;
                    range = *pc++ << 8;
                    range += *pc++;
                    pp->life = base + (s32)((f32)range * HSD_Randf());
                    break;
                }

                case 0xA7: {
                    int threshold = *pc++;
                    if (threshold >= (s32)(100.0F * HSD_Randf())) {
                        pp->life = 1;
                        goto exit_loop;
                    }
                    break;
                }

                case 0xA8:
                    psReadFloat(&pc);
                    pp->pos.x += 2.0F * fval * HSD_Randf() - fval;
                    psReadFloat(&pc);
                    pp->pos.y += 2.0F * fval * HSD_Randf() - fval;
                    psReadFloat(&pc);
                    pp->pos.z += 2.0F * fval * HSD_Randf() - fval;
                    break;

                case 0xA9:
                    psReadFloat(&pc);
                    modifyDir(pp, fval);
                    break;

                case 0xAA: {
                    int range, idx;
                    u8 bank;
                    idx = *pc++ << 8;
                    idx += *pc++;
                    range = *pc++ << 8;
                    range += *pc++;
                    idx += (s32)((f32)range * HSD_Randf());
                    bank = pp->bank;
                    if (ptclref[bank] != NULL) {
                        idx = ptclref[bank][idx];
                    }
                    child = psGenerateParticleID0(&pp->next, pp->linkNo, bank, idx, 0);
                    if (child != NULL) {
                        child->pos.x = pp->pos.x;
                        child->pos.y = pp->pos.y;
                        child->pos.z = pp->pos.z;
                        psChildCommon(child, pp);
                        if (pp->appsrt != NULL) {
                            psAttachParticleAppSRT(child, pp->appsrt);
                        }
                        psInterpretParticle0(child, pp);
                    }
                    break;
                }

                case 0xAB: {
                    f32 scale;
                    psReadFloat(&pc);
                    scale = fval;
                    pp->vel.x *= scale;
                    pp->vel.y *= scale;
                    pp->vel.z *= scale;
                    break;
                }

                case 0xAC: {
                    f32 range;
                    READ_COUNT(pp->sizeCount);
                    psReadFloat(&pc);
                    pp->sizeTarget = fval;
                    psReadFloat(&pc);
                    range = fval;
                    pp->sizeTarget = range * HSD_Randf() + pp->sizeTarget;
                    if (pp->sizeCount == 0) {
                        pp->size = pp->sizeTarget;
                    }
                    break;
                }

                case 0xAD:
                    pp->kind |= PrimEnv;
                    break;

                case 0xAE:
                    pp->kind &= ~(MirrorS | MirrorT);
                    break;

                case 0xAF:
                    pp->kind &= ~MirrorT;
                    pp->kind |= MirrorS;
                    break;

                case 0xB0:
                    pp->kind &= ~MirrorS;
                    pp->kind |= MirrorT;
                    break;

                case 0xB1:
                    pp->kind |= MirrorS | MirrorT;
                    break;

                case 0xB2: {
                    HSD_psAppSRT *srt = pp->appsrt;
                    if (srt != NULL && srt->xA2 == 0) {
                        f32 x, y, z;
                        genPosUpdate(srt->gp);
                        {
                            Vec3 *translate;
                            Vec3 *rot;
                            Vec3 *scale;
                            MtxPtr mtx;
                            srt = pp->appsrt;
                            translate = &srt->translate;
                            rot = (Vec3 *)&srt->rot;
                            scale = &srt->scale;
                            mtx = srt->mmtx;
                            HSD_MtxSRT(mtx, scale, rot, translate, NULL);
                        }
                        srt = pp->appsrt;
                        x = srt->mmtx[0][3] + (srt->mmtx[0][0] * pp->pos.x + srt->mmtx[0][1] * pp->pos.y + srt->mmtx[0][2] * pp->pos.z);
                        y = srt->mmtx[1][3] + (srt->mmtx[1][0] * pp->pos.x + srt->mmtx[1][1] * pp->pos.y + srt->mmtx[1][2] * pp->pos.z);
                        z = srt->mmtx[2][3] + (srt->mmtx[2][0] * pp->pos.x + srt->mmtx[2][1] * pp->pos.y + srt->mmtx[2][2] * pp->pos.z);
                        pp->pos.x = x;
                        pp->pos.y = y;
                        pp->pos.z = z;
                        psRemoveParticleAppSRT(pp);
                    }
                    break;
                }

                case 0xB3:
                    STEP2(pp->aCmpParam1, pp->aCmpParam2, pp->aCmpParam1Target,
                          pp->aCmpParam2Target, pp->aCmpCount, pp->aCmpRemain);
                    READ_COUNT(pp->aCmpCount);
                    pp->aCmpMode = *pc++;
                    pp->aCmpParam1Target = pc[0];
                    pp->aCmpParam2Target = pc[1];
                    pc += 2;
                    if (pp->aCmpCount == 0) {
                        pp->aCmpParam1 = pp->aCmpParam1Target;
                        pp->aCmpParam2 = pp->aCmpParam2Target;
                        pp->aCmpRemain = 0;
                        pp->aCmpCount = 0;
                    } else {
                        pp->aCmpRemain = pp->aCmpCount;
                    }
                    break;

                case 0xB4:
                    pp->kind |= TexInterpNear;
                    break;

                case 0xB5:
                    pp->kind &= ~TexInterpNear;
                    break;

                case 0xB6:
                    READ_COUNT(pp->rotateCount);
                    psReadFloat(&pc);
                    pp->rotateTarget += fval;
                    if (pp->rotateCount == 0) {
                        pp->rotate = pp->rotateTarget;
                    }
                    break;

                case 0xB7: {
                    HSD_JObj *jobj = psPointJObj[*pc++ + pp->pJObjOfs];
                    MtxPtr matrix;
                    f32 dz, dy, dx, vel_mag_sq, dist_sq;
                    if (jobj == NULL) {
                        break;
                    }
                    HSD_JObjSetupMatrix(jobj);
                    matrix = jobj->matrix;
                    val = pp->vel.x * pp->vel.x + pp->vel.y * pp->vel.y;
                    vel_mag_sq = pp->vel.z * pp->vel.z;
                    vel_mag_sq += val;
                    dx = matrix[0][3] - pp->pos.x;
                    dy = matrix[1][3];
                    dy -= pp->pos.y;
                    dz = matrix[2][3] - pp->pos.z;
                    if (vel_mag_sq > 0.0F) {
                        f64 guess = __frsqrte((f64)vel_mag_sq);
                        guess = 0.5 * guess * (3.0 - guess * guess * vel_mag_sq);
                        guess = 0.5 * guess * (3.0 - guess * guess * vel_mag_sq);
                        guess = 0.5 * guess * (3.0 - guess * guess * vel_mag_sq);
                        vel_res = (f32)(vel_mag_sq * guess);
                        vel_mag_sq = vel_res;
                    }
                    val = dx * dx + dy * dy;
                    if ((dist_sq = dz * dz + val) == 0.0F) {
                        break;
                    }
                    if (dist_sq > 0.0F) {
                        f64 guess = __frsqrte((f64)dist_sq);
                        guess = 0.5 * guess * (3.0 - guess * guess * dist_sq);
                        guess = 0.5 * guess * (3.0 - guess * guess * dist_sq);
                        guess = 0.5 * guess * (3.0 - guess * guess * dist_sq);
                        dist_res = (f32)(dist_sq * guess);
                        dist_sq = dist_res;
                    }
                    {
                        f32 scale = vel_mag_sq / dist_sq;
                        pp->vel.x = dx * scale;
                        pp->vel.y = dy * scale;
                        pp->vel.z = dz * scale;
                    }
                    break;
                }

                case 0xB8: {
                    int idx = *pc++;
                    f32 force, range;
                    idx += pp->pJObjOfs;
                    psReadFloat(&pc);
                    force = fval;
                    psReadFloat(&pc);
                    range = fval;
                    if (applyForceJObj(pp, psPointJObj[idx], force, range) != 0) {
                        pp->life = 1;
                        goto exit_loop;
                    }
                    break;
                }

                case 0xB9: {
                    int idx = *pc++ << 8;
                    idx += *pc++;
                    child = psGenerateParticleID0(&pp->next, pp->linkNo, pp->bank, idx, 0);
                    if (child != NULL) {
                        child->pos.x = pp->pos.x;
                        child->pos.y = pp->pos.y;
                        child->pos.z = pp->pos.z;
                        child->vel.x = pp->vel.x;
                        child->vel.y = pp->vel.y;
                        child->vel.z = pp->vel.z;
                        psChildCommon(child, pp);
                        if (pp->appsrt != NULL) {
                            psAttachParticleAppSRT(child, pp->appsrt);
                        }
                        psInterpretParticle0(child, pp);
                    }
                    break;
                }

                case 0xF2: {
                    u8 bank = pp->bank;
                    int idx = pc[0] << 8;
                    idx += pc[1];
                    pc += 2;
                    if (ptclref[bank] != NULL) {
                        idx = ptclref[bank][idx];
                    }
                    child = psGenerateParticleID0(&pp->next, pp->linkNo, bank, idx, 0);
                    if (child != NULL) {
                        child->pos.x = pp->pos.x;
                        child->pos.y = pp->pos.y;
                        child->pos.z = pp->pos.z;
                        child->vel.x = pp->vel.x;
                        child->vel.y = pp->vel.y;
                        child->vel.z = pp->vel.z;
                        psChildCommon(child, pp);
                        if (pp->appsrt != NULL) {
                            psAttachParticleAppSRT(child, pp->appsrt);
                        }
                        psInterpretParticle0(child, pp);
                    }
                    break;
                }

                case 0xBA:
                    COLOR_STEP(pp->primCol, pp->primColTarget, pp->primColCount, pp->primColRemain);
                    RAND_COL(pp->primColTarget.r);
                    RAND_COL(pp->primColTarget.g);
                    RAND_COL(pp->primColTarget.b);
                    RAND_COL(pp->primColTarget.a);
                    if (pp->primColCount == 0) {
                        pp->primCol = pp->primColTarget;
                    } else {
                        pp->primColRemain = pp->primColCount;
                    }
                    break;

                case 0xBB:
                    COLOR_STEP(pp->envCol, pp->envColTarget, pp->envColCount, pp->envColRemain);
                    RAND_COL(pp->envColTarget.r);
                    RAND_COL(pp->envColTarget.g);
                    RAND_COL(pp->envColTarget.b);
                    RAND_COL(pp->envColTarget.a);
                    if (pp->envColCount == 0) {
                        pp->envCol = pp->envColTarget;
                    } else {
                        pp->envColRemain = pp->envColCount;
                    }
                    break;

                case 0xBC: {
                    f32 range;
                    pp->poseNum = *pc++;
                    range = *pc++;
                    pp->poseNum = (u8)(s32)(range * HSD_Randf() + (f32)pp->poseNum);
                    psEnableTexture(pp);
                    break;
                }

                case 0xBD: {
                    f32 base_speed, random_range;
                    f32 mag;
                    psReadFloat(&pc);
                    base_speed = fval;
                    psReadFloat(&pc);
                    random_range = fval;
                    base_speed = random_range * HSD_Randf() + base_speed;
                    mag = pp->vel.z * pp->vel.z + (pp->vel.x * pp->vel.x + pp->vel.y * pp->vel.y);
                    if (mag > 0.0F) {
                        f64 guess = __frsqrte((f64)mag);
                        guess = 0.5 * guess * (3.0 - guess * guess * mag);
                        guess = 0.5 * guess * (3.0 - guess * guess * mag);
                        guess = 0.5 * guess * (3.0 - guess * guess * mag);
                        sqrt_res = (f32)(mag * guess);
                        mag = sqrt_res;
                    }
                    if (mag > 1e-10F) {
                        base_speed /= mag;
                        pp->vel.x *= base_speed;
                        pp->vel.y *= base_speed;
                        pp->vel.z *= base_speed;
                    }
                    break;
                }

                case 0xBE:
                    psReadFloat(&pc);
                    pp->vel.x *= fval;
                    psReadFloat(&pc);
                    pp->vel.y *= fval;
                    psReadFloat(&pc);
                    pp->vel.z *= fval;
                    break;

                case 0xBF: {
                    u8 idx = *pc++;
                    pp->kind |= (((idx + pp->pJObjOfs) << 12) & 0x7000) | 0x8000;
                }
                    break;

                case 0xC0:
                    COLOR_STEP(pp->primCol, pp->primColTarget, pp->primColCount, pp->primColRemain);
                    READ_COUNT(pp->primColCount);
                    pp->primColTarget = pp->primCol;
                    if (opcode & 1) {
                        pp->primColTarget.r = *pc++;
                    }
                    if (opcode & 2) {
                        pp->primColTarget.g = *pc++;
                    }
                    if (opcode & 4) {
                        pp->primColTarget.b = *pc++;
                    }
                    if (opcode & 8) {
                        pp->primColTarget.a = *pc++;
                    }
                    if (pp->primColCount == 0) {
                        pp->primCol = pp->primColTarget;
                        pp->primColRemain = 0;
                    } else {
                        pp->primColRemain = pp->primColCount;
                    }
                    break;

                case 0xD0:
                    COLOR_STEP(pp->envCol, pp->envColTarget, pp->envColCount, pp->envColRemain);
                    READ_COUNT(pp->envColCount);
                    pp->envColTarget = pp->envCol;
                    if (opcode & 1) {
                        pp->envColTarget.r = *pc++;
                    }
                    if (opcode & 2) {
                        pp->envColTarget.g = *pc++;
                    }
                    if (opcode & 4) {
                        pp->envColTarget.b = *pc++;
                    }
                    if (opcode & 8) {
                        pp->envColTarget.a = *pc++;
                    }
                    if (pp->envColCount == 0) {
                        pp->envCol = pp->envColTarget;
                        pp->envColRemain = 0;
                    } else {
                        pp->envColRemain = pp->envColCount;
                    }
                    break;

                case 0xE0: {
                    f32 r;
                    s8 delta;
                    COLOR_STEP(pp->primCol, pp->primColTarget, pp->primColCount, pp->primColRemain);
                    COLOR_STEP(pp->envCol, pp->envColTarget, pp->envColCount, pp->envColRemain);
                    r = HSD_Randf();
                    delta = (s8)*pc++;
                    r = (f32)(delta << 1) * r;
                    CLAMP_COL(pp->primColTarget.r, pp->primColTarget.r, r);
                    CLAMP_COL(pp->envColTarget.r, pp->envColTarget.r, r);
                    r = HSD_Randf();
                    delta = (s8)*pc++;
                    r = (f32)(delta << 1) * r;
                    CLAMP_COL(pp->primColTarget.g, pp->primColTarget.g, r);
                    CLAMP_COL(pp->envColTarget.g, pp->envColTarget.g, r);
                    r = HSD_Randf();
                    delta = (s8)*pc++;
                    r = (f32)(delta << 1) * r;
                    CLAMP_COL(pp->primColTarget.b, pp->primColTarget.b, r);
                    CLAMP_COL(pp->envColTarget.b, pp->envColTarget.b, r);
                    r = HSD_Randf();
                    delta = (s8)*pc++;
                    r = (f32)(delta << 1) * r;
                    CLAMP_COL(pp->primColTarget.a, pp->primColTarget.a, r);
                    CLAMP_COL(pp->envColTarget.a, pp->envColTarget.a, r);
                    if (pp->primColCount == 0) {
                        pp->primCol = pp->primColTarget;
                    }
                    pp->primColRemain = pp->primColCount;
                    if (pp->envColCount == 0) {
                        pp->envCol = pp->envColTarget;
                    }
                    pp->envColRemain = pp->envColCount;
                    break;
                }

                case 0xE9: {
                    int flags;
                    int steps;
                    f32 r, d;
                    COLOR_STEP(pp->primCol, pp->primColTarget, pp->primColCount, pp->primColRemain);
                    COLOR_STEP(pp->envCol, pp->envColTarget, pp->envColCount, pp->envColRemain);
                    flags = *pc++;
                    steps = *pc++;
                    if (steps != 0) {
                        r = (f32)(s32)((f32)(steps + 1) * HSD_Randf()) / (f32)steps;
                    } else {
                        r = HSD_Randf();
                    }
                    if (flags & 1) {
                        d = r * (f32)((s8)*pc++ << 1);
                        if (flags & 0x10) {
                            CLAMP_COL(pp->primColTarget.r, pp->primColTarget.r, d);
                        }
                        if (flags & 0x20) {
                            CLAMP_COL(pp->envColTarget.r, pp->envColTarget.r, d);
                        }
                    }
                    if (flags & 2) {
                        d = r * (f32)((s8)*pc++ << 1);
                        if (flags & 0x10) {
                            CLAMP_COL(pp->primColTarget.g, pp->primColTarget.g, d);
                        }
                        if (flags & 0x20) {
                            CLAMP_COL(pp->envColTarget.g, pp->envColTarget.g, d);
                        }
                    }
                    if (flags & 4) {
                        d = r * (f32)((s8)*pc++ << 1);
                        if (flags & 0x10) {
                            CLAMP_COL(pp->primColTarget.b, pp->primColTarget.b, d);
                        }
                        if (flags & 0x20) {
                            CLAMP_COL(pp->envColTarget.b, pp->envColTarget.b, d);
                        }
                    }
                    if (flags & 8) {
                        f32 a_rand;
                        s8 delta;
                        a_rand = HSD_Randf();
                        delta = *(s8 *)pc++;
                        a_rand = (f32)(s32)((f32)(steps + 1) * a_rand);
                        d = ((f32)(delta << 1) * a_rand) / (f32)steps;
                        if (flags & 0x10) {
                            CLAMP_COL(pp->primColTarget.a, pp->primColTarget.a, d);
                        }
                        if (flags & 0x20) {
                            CLAMP_COL(pp->envColTarget.a, pp->envColTarget.a, d);
                        }
                    }
                    if (pp->primColCount == 0) {
                        pp->primCol = pp->primColTarget;
                    }
                    pp->primColRemain = pp->primColCount;
                    if (pp->envColCount == 0) {
                        pp->envCol = pp->envColTarget;
                    }
                    pp->envColRemain = pp->envColCount;
                    break;
                }

                case 0xE1: {
                    int idx = *pc++;
                    if (idx == 0) {
                        pp->callback = NULL;
                    } else {
                        pp->callback = psCallback[idx - 1];
                    }
                    break;
                }

                case 0xEC: {
                    int idx = *pc++;
                    f32 v;
                    psReadFloat(&pc);
                    v = fval;
                    if (pp->gen->userfunc != NULL && pp->gen->userfunc->setUserData != NULL) {
                        pp->gen->userfunc->setUserData(pp, idx, v);
                    } else if (pp->userdata != NULL) {
                        pp->userdata[idx] = v;
                    }
                    break;
                }

                case 0xE2:
                    pp->kind |= TexEdge;
                    break;

                case 0xE3:
                    pp->palNum = *pc++;
                    break;

                case 0xE4:
                    switch (*pc++ & 3) {
                    case 0:
                        pp->kind &= ~TexFlipS;
                        break;
                    case 1:
                        pp->kind |= TexFlipS;
                        break;
                    case 2:
                        pp->kind ^= TexFlipS;
                        break;
                    case 3:
                        if (HSD_Randf() < 0.5F) {
                            pp->kind &= ~TexFlipS;
                        } else {
                            pp->kind |= TexFlipS;
                        }
                        break;
                    }
                    break;

                case 0xE5:
                    switch (*pc++ & 3) {
                    case 0:
                        pp->kind &= ~TexFlipT;
                        break;
                    case 1:
                        pp->kind |= TexFlipT;
                        break;
                    case 2:
                        pp->kind ^= TexFlipT;
                        break;
                    case 3:
                        if (HSD_Randf() < 0.5F) {
                            pp->kind &= ~TexFlipT;
                        } else {
                            pp->kind |= TexFlipT;
                        }
                        break;
                    }
                    break;

                case 0xE6:
                    pp->kind |= DirVec;
                    break;

                case 0xE7:
                    pp->kind &= ~DirVec;
                    break;

                case 0xE8: {
                    f32 trail;
                    psReadFloat(&pc);
                    trail = fval;
                    if (trail < 0.0F) {
                        pp->kind &= ~Trail;
                    } else {
                        pp->kind |= Trail;
                        pp->trail = trail;
                    }
                    break;
                }

                case 0xEA:
                    STEP2(pp->matRGB, pp->matA, pp->matRGBTarget, pp->matATarget,
                          pp->matColCount, pp->matColRemain);
                    {
                        u8 flags;
                        READ_COUNT(pp->matColCount);
                        flags = *pc++;
                        pp->matRGBTarget = pp->matRGB;
                        if (flags & 1) {
                            pp->matRGBTarget = *pc++;
                        }
                        if (flags & 8) {
                            pp->matATarget = *pc++;
                        }
                    }
                    if (pp->matColCount == 0) {
                        pp->matRGB = pp->matRGBTarget;
                        pp->matColRemain = 0;
                    } else {
                        pp->matColRemain = pp->matColCount;
                    }
                    break;

                case 0xEB:
                    STEP2(pp->ambRGB, pp->ambA, pp->ambRGBTarget, pp->ambATarget,
                          pp->ambColCount, pp->ambColRemain);
                    {
                        u8 flags;
                        READ_COUNT(pp->ambColCount);
                        flags = *pc++;
                        pp->ambRGBTarget = pp->ambRGB;
                        if (flags & 1) {
                            pp->ambRGBTarget = *pc++;
                        }
                        if (flags & 8) {
                            pp->ambATarget = *pc++;
                        }
                    }
                    if (pp->ambColCount == 0) {
                        pp->ambRGB = pp->ambRGBTarget;
                        pp->ambColRemain = 0;
                    } else {
                        pp->ambColRemain = pp->ambColCount;
                    }
                    break;

                case 0xED: {
                    f32 base_val, range;
                    int steps;
                    psReadFloat(&pc);
                    base_val = fval;
                    psReadFloat(&pc);
                    range = fval;
                    steps = *pc++;
                    if (steps != 0) {
                        base_val += range * (f32)(s32)((f32)(steps + 1) * HSD_Randf()) / (f32)steps;
                    } else {
                        base_val = range * HSD_Randf() + base_val;
                    }
                    pp->rotateTarget += base_val;
                    pp->rotate += base_val;
                    break;
                }

                case 0xFA:
                    pp->loopCount = *pc++;
                    pp->cmdLoopPtr = (u16)(pc - pp->cmdList);
                    break;

                case 0xFB:
                    if (--pp->loopCount != 0) {
                        pc = pp->cmdList + pp->cmdLoopPtr;
                    }
                    break;

                case 0xFC:
                    pp->cmdMarkPtr = (u16)(pc - pp->cmdList);
                    break;

                case 0xFD:
                    pc = pp->cmdList + pp->cmdMarkPtr;
                    break;

                case 0xFE:
                case 0xFF:
                    pp->life = 1;
                    goto exit_loop;

                default:
                    break;
                }
            }

            if (operand != 0) {
                break;
            }
        }

    exit_loop:
        pp->cmdPtr = (u16)(pc - pp->cmdList);
        pp->cmdWait = operand;
    }

    if (--pp->life == 0) {
    delete_particle:
        if (pp->gen != NULL && pp->gen->userfunc != NULL && pp->gen->userfunc->hookDelete != NULL) {
            pp->gen->userfunc->hookDelete(pp);
        }
        if (pp->gen != NULL) {
            pp->gen->numChild--;
        }
        next = pp->next;
        if (pp->appsrt != NULL && psRemoveParticleAppSRT(pp) == 0 && prev == NULL &&
            _psListGetFirst(pp->linkNo) != next)
        {
            next = _psListGetFirst(pp->linkNo);
        }
        psDeletePntJObjwithParticle(pp);
        _psListDelete(pp, prev);
        return next;
    }

    if (pp->kind & Tornado) {
        HSD_Generator *gp = pp->gen;
        f32 sinA, sinB, cosA, cosB;
        f32 d, nd, e, vz;
        f32 R;

        sinA = sinf(pp->grav);
        sinB = sinf(pp->fric);
        cosA = cosf(pp->grav);
        cosB = cosf(pp->fric);

        pp->vel.z += gp->aux.tornado.vel;

        R = gp->radius < 0.0F ? -gp->radius : gp->radius;
        {
            f32 ang;
            if ((ang = gp->angle) < 0.0F) {
                ang = -ang;
            }
            R += pp->vel.z * tanf(ang);
            R *= pp->vel.y;
        }
        pp->vel.x += gp->grav;

        d = R * cosf(pp->vel.x);
        e = R * sinf(pp->vel.x);
        nd = -d;
        vz = pp->vel.z;

        pp->pos.x = gp->pos.x + (d * cosB + vz * sinB);
        pp->pos.y = gp->pos.y + (cosB * (vz * sinA) + (sinB * (nd * sinA) + e * cosA));
        pp->pos.z = gp->pos.z + (cosB * (vz * cosA) + (sinB * (nd * cosA) - e * sinA));
    } else {
        if (pp->kind & ApplyGrav) {
            pp->vel.y -= pp->grav;
        }
        if (pp->kind & ApplyFric) {
            pp->vel.x *= pp->fric;
            pp->vel.y *= pp->fric;
            pp->vel.z *= pp->fric;
        }
        pp->pos.x += pp->vel.x;
        pp->pos.y += pp->vel.y;
        pp->pos.z += pp->vel.z;
    }

    if (pp->kind & 0x8000) {
        s32 jobj_idx = (pp->kind & 0x7000) >> 12;
        HSD_JObj *jobj;

        if (psPointJObj[jobj_idx] == NULL) {
            HSD_JObj *new_jobj = HSD_JObjAlloc();
            if (new_jobj != NULL) {
                psSetPointJObj(jobj_idx + 1, new_jobj);
                HSD_JObjUnref(new_jobj);
            }
        }

        if ((jobj = psPointJObj[jobj_idx]) != NULL) {
            HSD_JObjSetupMatrix(jobj);
            jobj = psPointJObj[jobj_idx];
            HSD_JObjAddTranslationX(jobj, pp->pos.x - jobj->matrix[0][3]);
            jobj = psPointJObj[jobj_idx];
            HSD_JObjAddTranslationY(jobj, pp->pos.y - jobj->matrix[1][3]);
            jobj = psPointJObj[jobj_idx];
            HSD_JObjAddTranslationZ(jobj, pp->pos.z - jobj->matrix[2][3]);
        }
    }

    if (pp->callback != NULL) {
        if (pp->callback(pp) == -1) {
            goto delete_particle;
        }
    }

    return pp->next;
}

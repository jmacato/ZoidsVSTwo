/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/generator.c. */
#include "particle.h"
#include "appsrt.h"
#include "list.h"
#include "matrix.h"
#include "joint_matrix.h"
#include "class.h"
extern void *HSD_Alloc(u32 size);
extern void HSD_Free(void *memory);
extern void *memset(void *memory, s32 value, u32 size);
extern void HSD_JObjUnref(HSD_JObj *joint);
extern void psKillGeneratorChild(HSD_Generator *generator);
struct HSD_CObj {
    HSD_ClassInfo *class_info;
    u16 reference_count;
    u16 individual_reference_count;
};
extern u16 numActiveGenerators;
extern u16 numPeakGenerators;
static u16 psIDNum = 0x100;
static void (*psExtExecGenFunc)(HSD_Generator *generator, Mtx matrix);
static void (*psExtCreateGenFunc)(HSD_Generator *generator);
static struct HSD_CObj *psCamera;
static HSD_SList *standbyGenerator;
/* MWCC places these public BSS objects in reverse declaration order. */
HSD_Generator *freeGenerator;
HSD_Generator *activeGenerator;
HSD_Generator *lastGP;
HSD_Generator *psKillGeneratorSub(HSD_Generator *generator, HSD_Generator *previous);
void psKillGenerator(HSD_Generator *generator);
void psKillAllGenerator(void);
u16 psGetNewIDNum(void)
{
    psIDNum++;
    if (psIDNum < 0x100)
        psIDNum = 0x100;
    return psIDNum;
}
void psInitGenerator(int count)
{
    int i;
    HSD_Generator *generator;
    activeGenerator = 0;
    freeGenerator = 0;
    for (i = count - 1; i >= 0; i--) {
        generator = HSD_Alloc(sizeof(HSD_Generator));
        /* Keep the SDK allocation and clearing order. */
        memset(generator, 0, sizeof(HSD_Generator));
        if (generator == 0)
            return;
        generator->next = freeGenerator;
        freeGenerator = generator;
    }
    numActiveGenerators = 0;
    numPeakGenerators = 0;
    standbyGenerator = 0;
    psCamera = 0;
    psExtExecGenFunc = 0;
    psExtCreateGenFunc = 0;
    lastGP = 0;
}
void psRemoveGenerator(void)
{
    HSD_Generator *generator;
    HSD_Generator *next;
    psKillAllParticle();
    psKillAllGenerator();
    generator = freeGenerator;
    while (generator != 0) {
        next = generator->next;
        HSD_Free(generator);
        generator = next;
    }
    freeGenerator = 0;
    psSetBillboardCamera(0);
}
HSD_Generator *psKillGeneratorSub(HSD_Generator *generator, HSD_Generator *previous)
{
    if (generator->type & 0x80)
        psKillGeneratorChild(generator);
    if (generator->numChild != 0) {
        generator->random = 0.0F;
        generator->genLife = 1;
        return generator;
    }
    if ((generator->type & PS_GEN_JOBJ) && generator->jobj != 0 &&
        (generator->type & PS_GEN_APPSRT)) {
        HSD_psAppSRT *transform = generator->appsrt;
        if (transform != 0 && transform->gp == generator && transform->usedCount != 1) {
            generator->random = 0.0F;
            generator->genLife = 1;
            return generator;
        }
    }
    if (previous == 0)
        activeGenerator = generator->next;
    else
        previous->next = generator->next;
    if (generator->appsrt != 0)
        psRemoveGeneratorAppSRT(generator);
    generator->next = freeGenerator;
    freeGenerator = generator;
    numActiveGenerators--;
    if (generator->jobj != 0) {
        HSD_JObjUnref(generator->jobj);
        generator->jobj = 0;
    }
    return previous;
}
void psKillGenerator(HSD_Generator *generator)
{
    HSD_Generator *current = activeGenerator;
    lastGP = 0;
    while (current != 0) {
        if (current == generator) {
            lastGP = psKillGeneratorSub(generator, lastGP);
            if (lastGP != 0) {
                while (lastGP->next != 0)
                    lastGP = lastGP->next;
            } else if (activeGenerator != 0) {
                lastGP = activeGenerator;
                while (lastGP->next != 0)
                    lastGP = lastGP->next;
            }
            return;
        }
        lastGP = current;
        current = current->next;
    }
}
void psKillAllGenerator(void)
{
    HSD_Generator *generator = activeGenerator;
    HSD_Generator *next;
    while (generator != 0) {
        next = generator->next;
        psKillGenerator(generator);
        generator = next;
    }
    while (standbyGenerator != 0)
        standbyGenerator = HSD_SListRemove(standbyGenerator);
}
void psKillJObjAllGenerator(HSD_JObj *joint)
{
    HSD_Generator *generator;
    HSD_Generator *next;
    if (joint != 0) {
        generator = activeGenerator;
        while (generator != 0) {
            next = generator->next;
            if (generator->jobj == joint)
                psKillGenerator(generator);
            generator = next;
        }
        if (joint->flags & 0x1000)
            return;
        joint = joint == 0 ? 0 : joint->child;
        while (joint != 0) {
            psKillJObjAllGenerator(joint);
            joint = joint == 0 ? 0 : joint->next;
        }
    }
}

static void fixGeneratorJObj(HSD_Generator *gen)
{
    HSD_JObj *jobj;
    Mtx normalized;
    Vec3 col;

    if (!(gen->type & PS_GEN_JOBJ)) {
        return;
    }

    if ((jobj = gen->jobj) == 0) {
        return;
    }

    setup_matrix(jobj);

    if ((gen->type & PS_GEN_APPSRT) && gen->appsrt != 0) {
        if (gen->appsrt->gp == gen) {
            gen->appsrt->translate.x = jobj->matrix[0][3];
            gen->appsrt->translate.y = jobj->matrix[1][3];
            gen->appsrt->translate.z = jobj->matrix[2][3];
        }
    } else {
        gen->pos.x = jobj->matrix[0][3];
        gen->pos.y = jobj->matrix[1][3];
        gen->pos.z = jobj->matrix[2][3];
    }
    col.x = jobj->matrix[0][0];
    col.y = jobj->matrix[1][0];
    col.z = jobj->matrix[2][0];
    PSVECNormalize(&col, &col);
    normalized[0][0] = col.x;
    normalized[1][0] = col.y;
    normalized[2][0] = col.z;

    col.x = jobj->matrix[0][1];
    col.y = jobj->matrix[1][1];
    col.z = jobj->matrix[2][1];
    PSVECNormalize(&col, &col);
    normalized[0][1] = col.x;
    normalized[1][1] = col.y;
    normalized[2][1] = col.z;

    col.x = jobj->matrix[0][2];
    col.y = jobj->matrix[1][2];
    col.z = jobj->matrix[2][2];
    PSVECNormalize(&col, &col);

    {
        f32 cx = col.x;
        f32 cy = col.y;
        f32 cz = col.z;
        f32 vx = gen->vel.x;
        f32 vy = gen->vel.y;
        f32 vz = gen->vel.z;
        gen->vel.x = normalized[0][0] * vx + normalized[0][1] * vy + cx * vz;
        gen->vel.y = normalized[1][0] * vx + normalized[1][1] * vy + cy * vz;
        gen->vel.z = normalized[2][0] * vx + normalized[2][1] * vy + cz * vz;
    }

    if ((gen->type & PS_GEN_TYPE_MASK) == PS_GEN_LINE) {
        Vec3 end;
        end.x = gen->aux.line.x2;
        end.y = gen->aux.line.y2;
        end.z = gen->aux.line.z2;
        gen->aux.line.x2 =
            jobj->matrix[0][0] * end.x + jobj->matrix[0][1] * end.y + jobj->matrix[0][2] * end.z;
        gen->aux.line.y2 =
            jobj->matrix[1][0] * end.x + jobj->matrix[1][1] * end.y + jobj->matrix[1][2] * end.z;
        gen->aux.line.z2 =
            jobj->matrix[2][0] * end.x + jobj->matrix[2][1] * end.y + jobj->matrix[2][2] * end.z;
    }
}

HSD_Generator *psCreateGenerator(void)
{
    HSD_Generator *gen;

    if (freeGenerator == 0) {
        freeGenerator = HSD_Alloc(sizeof(HSD_Generator));
        memset(freeGenerator, 0, sizeof(HSD_Generator));
    }
    gen = freeGenerator;
    if (gen == 0) {
        return 0;
    }

    numActiveGenerators++;
    if (numActiveGenerators > numPeakGenerators) {
        numPeakGenerators = numActiveGenerators;
    }

    freeGenerator = gen->next;
    if (lastGP == 0 || lastGP->next == 0) {
        if (activeGenerator == 0) {
            gen->next = 0;
            activeGenerator = gen;
        } else {
            gen->next = activeGenerator->next;
            activeGenerator->next = gen;
        }
    } else {
        gen->next = lastGP->next->next;
        lastGP->next->next = gen;
    }

    psIDNum++;
    if (psIDNum < 0x100) {
        psIDNum = 0x100;
    }
    gen->idnum = psIDNum;
    gen->appsrt = 0;
    gen->userfunc = 0;
    return gen;
}

extern f32 HSD_Randf(void);
extern f32 atan2f(f32 y, f32 x);
extern f64 __frsqrte(f64 value);
static inline f32 generator_sqrt(f32 value)
{
    if (value > 0.0F) {
        f64 estimate = __frsqrte(value);
        volatile f32 result;
        estimate = 0.5 * estimate * -((value * (estimate * estimate)) - 3.0);
        estimate = 0.5 * estimate * -((value * (estimate * estimate)) - 3.0);
        estimate = 0.5 * estimate * -((value * (estimate * estimate)) - 3.0);
        result = value * estimate;
        return result;
    }
    return value;
}
static inline f32 absolute_value(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

HSD_Generator *psCreateGeneratorID(s32 linkNo, s32 bank, s32 idx)
{
    HSD_Generator *gen;

    if (bank >= 0x40) {
        return 0;
    }
    if (linkNo >= 8) {
        return 0;
    }
    if (idx >= psNumCmdList[bank]) {
        return 0;
    }

    if (psCmdListArray[bank][idx] == 0) {
        return 0;
    }

    gen = psCreateGenerator();
    if (gen != 0) {
        gen->type = psCmdListArray[bank][idx]->type;
        gen->bank = bank;
        gen->linkNo = linkNo;
        gen->kind = psCmdListArray[bank][idx]->kind;
        gen->texGroup = psCmdListArray[bank][idx]->texGroup;
        gen->life = psCmdListArray[bank][idx]->life;
        gen->genLife = psCmdListArray[bank][idx]->genLife;
        gen->pos.z = 0.0F;
        gen->pos.y = 0.0F;
        gen->pos.x = 0.0F;
        gen->vel.x = psCmdListArray[bank][idx]->vx;
        gen->vel.y = psCmdListArray[bank][idx]->vy;
        gen->vel.z = psCmdListArray[bank][idx]->vz;
        gen->grav = psCmdListArray[bank][idx]->grav;
        gen->fric = psCmdListArray[bank][idx]->fric;
        gen->size = psCmdListArray[bank][idx]->size;
        gen->cmdList = psCmdListArray[bank][idx]->cmdList;
        gen->radius = psCmdListArray[bank][idx]->radius;
        gen->angle = psCmdListArray[bank][idx]->angle;
        gen->random = psCmdListArray[bank][idx]->random;

        if (gen->kind & 0x100) {
            if (gen->random < 0.0F) {
                gen->count = (1.0F + gen->random > 1.1920929e-7F) ? 1.0F : 0.0F;
            } else {
                gen->count = 0.9999999F;
            }
        } else if (gen->random < 0.0F) {
            gen->count = 0.0F;
        } else {
            gen->count = HSD_Randf();
        }

        {
            HSD_PSTexGroup *tg = psTexGroupArray[bank][gen->texGroup];
            if (tg != 0 && tg->palflag != 0) {
                gen->kind |= ComTLUT;
            }
        }

        gen->jobj = 0;
        gen->numChild = 0;

        switch (gen->type & PS_GEN_TYPE_MASK) {
        case PS_GEN_DISC:
        case PS_GEN_DISC_3:
        case PS_GEN_DISC_4: {
            HSD_PSCmdList *c = psCmdListArray[bank][idx];
            f32 p1 = c->param1;
            if (p1 == 0.0F && c->param2 == 0.0F) {
                gen->aux.disc.minAngle = 0.0F;
                gen->aux.disc.maxAngle = 6.2831855F;
            } else {
                gen->aux.disc.minAngle = p1;
                gen->aux.disc.maxAngle = psCmdListArray[bank][idx]->param2;
            }
            break;
        }
        case PS_GEN_LINE:
            gen->aux.line.x2 = psCmdListArray[bank][idx]->param1;
            gen->aux.line.y2 = psCmdListArray[bank][idx]->param2;
            gen->aux.line.z2 = psCmdListArray[bank][idx]->param3;
            break;
        case PS_GEN_TORNADO:
            break;
        case PS_GEN_CONE:
        case PS_GEN_CYLINDER: {
            HSD_PSCmdList *c = psCmdListArray[bank][idx];
            f32 p1 = c->param1;
            if (p1 == 0.0F && c->param2 == 0.0F) {
                gen->aux.cone.minAngle = 0.0F;
                gen->aux.cone.maxAngle = 6.2831855F;
            } else {
                gen->aux.cone.minAngle = p1;
                gen->aux.cone.maxAngle = psCmdListArray[bank][idx]->param2;
            }
            gen->aux.cone.height = psCmdListArray[bank][idx]->param3;
            break;
        }
        case PS_GEN_RECT: {
            gen->aux.rect.xx = gen->aux.rect.x = psCmdListArray[bank][idx]->param1;
            gen->aux.rect.yy = gen->aux.rect.y = psCmdListArray[bank][idx]->param2;
            gen->aux.rect.zz = gen->aux.rect.z = psCmdListArray[bank][idx]->param3;
            gen->aux.rect.zy = 0.0F;
            gen->aux.rect.zx = 0.0F;
            gen->aux.rect.yz = 0.0F;
            gen->aux.rect.yx = 0.0F;
            gen->aux.rect.xz = 0.0F;
            gen->aux.rect.xy = 0.0F;
            gen->aux.rect.flag = 0;
            if (psCmdListArray[bank][idx]->param1 < 0.0F) {
                gen->aux.rect.flag |= 1;
            }
            if (psCmdListArray[bank][idx]->param2 < 0.0F) {
                gen->aux.rect.flag |= 2;
            }
            if (psCmdListArray[bank][idx]->param3 < 0.0F) {
                gen->aux.rect.flag |= 4;
            }
            break;
        }
        case PS_GEN_SPHERE: {
            f32 mag;

            gen->aux.sphere.speed =
                generator_sqrt(gen->vel.z * gen->vel.z +
                               (gen->vel.x * gen->vel.x + gen->vel.y * gen->vel.y));

            mag = generator_sqrt(gen->vel.x * gen->vel.x + gen->vel.z * gen->vel.z);

            if (mag < 1.1754944e-38F) {
                if (gen->vel.y >= 0.0F) {
                    gen->aux.sphere.latMid = 1.5707964F;
                } else {
                    gen->aux.sphere.latMid = -1.5707964F;
                }
            } else {
                gen->aux.sphere.latMid = atan2f(gen->vel.y, mag);
            }

            if (absolute_value(gen->vel.x) < 1.1754944e-38F) {
                if (gen->vel.z >= 0.0F) {
                    gen->aux.sphere.lonMid = 1.5707964F;
                } else {
                    gen->aux.sphere.lonMid = -1.5707964F;
                }
            } else {
                gen->aux.sphere.lonMid = atan2f(gen->vel.z, gen->vel.x);
            }
            gen->aux.sphere.latRange = psCmdListArray[bank][idx]->param1;
            if (gen->aux.sphere.latRange < 0.0F) {
                gen->aux.sphere.latRange = -gen->aux.sphere.latRange;
                gen->aux.sphere.speed = -gen->aux.sphere.speed;
            }
            gen->aux.sphere.lonRange = psCmdListArray[bank][idx]->param2;
            break;
        }
        default:
            if (psExtCreateGenFunc != 0) {
                psExtCreateGenFunc(gen);
            }
            break;
        }

        if (gen->kind & 0x20000) {
            gen->type |= PS_GEN_APPSRT;
            psAddGeneratorAppSRT(gen, 0);
            if (gen->appsrt != 0) {
                gen->appsrt->xA2 = 1;
                gen->appsrt->gp = gen;
            }
        }
    }
    return gen;
}

extern f32 generateParticle(HSD_Generator *generator);
static inline void RefJoint(HSD_JObj *joint)
{
    if (joint != 0) {
        joint->reference_count++;
        if (joint->reference_count == 0xFFFF)
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}

void psExecGenerator(u32 mask)
{
    HSD_Generator *gp;
    HSD_Generator *gen;

    while (standbyGenerator != 0) {
        gp = standbyGenerator->data;
        fixGeneratorJObj(gp);
        standbyGenerator = HSD_SListRemove(standbyGenerator);
        if (gp->jobj != 0) {
            HSD_JObjUnref(gp->jobj);
            gp->jobj = 0;
        }
    }

    gen = activeGenerator;
    lastGP = 0;

    while (gen != 0) {
        if (mask & (1 << (gen->linkNo + 16))) {
            lastGP = gen;
            gen = gen->next;
            continue;
        }
        if (gen->kind & 0x800) {
            lastGP = gen;
            gen = gen->next;
            continue;
        }
        if (gen->type & PS_GEN_JOBJ) {
            HSD_JObj *jobj;
            if ((jobj = gen->jobj) != 0) {
                setup_matrix(jobj);
                if ((gen->type & PS_GEN_APPSRT) && gen->appsrt != 0) {
                    if (gen->appsrt->gp == gen) {
                        gen->appsrt->translate.x = gen->jobj->matrix[0][3];
                        gen->appsrt->translate.y = gen->jobj->matrix[1][3];
                        gen->appsrt->translate.z = gen->jobj->matrix[2][3];
                    }
                } else {
                    gen->pos.x = gen->jobj->matrix[0][3];
                    gen->pos.y = gen->jobj->matrix[1][3];
                    gen->pos.z = gen->jobj->matrix[2][3];
                }
            }
        }
        if (gen->random < 0.0F) {
            gen->count -= gen->random;
        } else {
            gen->count += gen->random * HSD_Randf();
        }
        if (gen->count >= 1.0F) {
            gen->count = generateParticle(gen);
        }
        if (gen->genLife != 0) {
            u16 life = gen->genLife - 1;
            gen->genLife = life;
            if (life == 0) {
                lastGP = psKillGeneratorSub(gen, lastGP);
                if (lastGP != 0) {
                    gen = lastGP->next;
                } else {
                    gen = activeGenerator;
                }
                continue;
            }
        }
        lastGP = gen;
        gen = gen->next;
    }
}

HSD_Generator *psCreateGeneratorIDJObjD(s32 linkNo, s32 bank, s32 gfx_id,
                                        HSD_JObj *jobj)
{
    HSD_Generator *gen;

    gen = psCreateGeneratorID(linkNo, bank, gfx_id);
    if (gen == 0) {
        return 0;
    }
    if (gen != 0) {
        gen->jobj = jobj;
        if (jobj != 0) {
            RefJoint(jobj);
        }
    }
    gen->type |= PS_GEN_JOBJ;
    return gen;
}

HSD_Generator *psCreateGeneratorIDJObjS(s32 linkNo, s32 bank, s32 gfx_id,
                                        HSD_JObj *jobj)
{
    HSD_Generator *gen;

    gen = psCreateGeneratorID(linkNo, bank, gfx_id);
    if (gen == 0) {
        return 0;
    }
    if (gen != 0) {
        gen->jobj = jobj;
        if (jobj != 0) {
            RefJoint(jobj);
        }
    }
    gen->type |= PS_GEN_JOBJ;
    standbyGenerator = HSD_SListAppend(standbyGenerator, gen);
    return gen;
}
static inline int CameraUnrefCount(u16 *count)
{
    int result;
    if ((result = (*count == (u16)-1)))
        return result;
    return (*count)-- == 0;
}
static inline void CameraUnref(struct HSD_CObj *camera)
{
    /* Keep the counter address separate from the store for MWCC. */
    if (camera != 0 && CameraUnrefCount((u16 *)((u32)camera + 4))) {
        if (camera != 0) {
            camera->class_info->release((HSD_Class *)camera);
            camera->class_info->destroy((HSD_Class *)camera);
        }
    }
}
static inline void CameraRef(struct HSD_CObj *camera)
{
    if (camera != 0) {
        camera->reference_count++;
        if (camera->reference_count == 0xFFFF)
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}
void psSetBillboardCamera(struct HSD_CObj *camera)
{
    struct HSD_CObj *previous;
    if ((previous = psCamera) != 0)
        CameraUnref(previous);
    if (camera != 0)
        CameraRef(camera);
    psCamera = camera;
}

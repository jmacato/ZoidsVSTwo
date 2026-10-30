/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/particle.c. */
#include "particle.h"
extern void HSD_JObjUnref(HSD_JObj *joint);
extern void __assert(const char *file, u32 line, const char *condition);
static inline void RefJoint(HSD_JObj *joint)
{
    if (joint != 0) {
        joint->reference_count++;
        if (joint->reference_count == 0xFFFF)
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
    }
}
void psSetPointJObj(int index, HSD_JObj *joint)
{
    if (index < 0 || index > 8)
        return;
    if (index != 0) {
        HSD_JObj **slot = psPointJObj;
        HSD_JObj *previous;
        slot += index;
        previous = *--slot;
        if (previous != joint) {
            if (previous != 0) {
                /* Preserve the original SDK's release of the incoming joint. */
                HSD_JObjUnref(joint);
            }
            *slot = joint;
            RefJoint(joint);
        }
    } else {
        int i;
        for (i = 0; i < 8; i++) {
            if (psPointJObj[i] == joint) {
                HSD_JObjUnref(psPointJObj[i]);
                psPointJObj[i] = 0;
            }
        }
    }
}
void psSetPointJObjNodup(HSD_JObj *joint, int index)
{
    int i;
    if (index < 0 || index > 8)
        return;
    for (i = 0; i < 8; i++) {
        if (psPointJObj[i] == joint) {
            HSD_JObjUnref(psPointJObj[i]);
            psPointJObj[i] = 0;
        }
    }
    if (index != 0) {
        psPointJObj[index - 1] = joint;
        RefJoint(joint);
    }
}
void psClearPointJObj(void)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (psPointJObj[i] != 0) {
            HSD_JObjUnref(psPointJObj[i]);
            psPointJObj[i] = 0;
        }
    }
}
void psDeletePntJObjwithParticle(HSD_Particle *particle)
{
    if (particle->kind & 0x8000) {
        int i = (particle->kind >> 12) & 7;
        if (psPointJObj[i] != 0) {
            HSD_JObjUnref(psPointJObj[i]);
            psPointJObj[i] = 0;
        }
    }
}

extern int psRemoveParticleAppSRT(HSD_Particle *particle);

static inline HSD_Particle *getnext(HSD_Particle *p)
{
    return p->next;
}

void psKillGeneratorChild(HSD_Generator *gp)
{
    HSD_Particle *prev;
    HSD_Particle *pp;
    u16 genid;
    int link;

    prev = 0;
    link = gp->linkNo;
    genid = gp->idnum;
    pp = _psListGetFirst(link);
    while (pp != 0) {
        HSD_Particle *next = getnext(pp);
        if (pp->idnum == genid && pp->gen != 0 && pp->gen == gp) {
            if (pp->gen != 0)
                pp->gen->numChild--;
            if (pp->appsrt != 0)
                psRemoveParticleAppSRT(pp);
            psDeletePntJObjwithParticle(pp);
            _psListDelete(pp, prev);
            if (pp->gen != 0 && pp->gen->userfunc != 0 && pp->gen->userfunc->hookDelete != 0)
                pp->gen->userfunc->hookDelete(pp);
        } else {
            prev = pp;
        }
        pp = next;
    }
}

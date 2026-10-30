/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/psappsrt.c. */
#include "appsrt.h"
extern void *HSD_Alloc(u32 size);
extern void HSD_Free(void *memory);
extern void *memset(void *memory, s32 value, u32 size);
extern u16 numActivePSAppSRT;
extern u16 numPeakPSAppSRT;
static HSD_psAppSRT *freeAppSRT;
static u16 psSizeofAppSRT;
int psInitAppSRT(int count, int size)
{
    int i = 0;
    HSD_psAppSRT *transform;
    numActivePSAppSRT = 0;
    numPeakPSAppSRT = 0;
    psSizeofAppSRT = size;
    freeAppSRT = 0;
    for (; i < count; i++) {
        transform = HSD_Alloc(size);
        if (transform == 0)
            return i;
        memset(transform, 0, size);
        transform->next = freeAppSRT;
        freeAppSRT = transform;
    }
    return i;
}
void psRemoveAppSRT(void)
{
    HSD_psAppSRT *transform = freeAppSRT;
    HSD_psAppSRT *next;
    while (transform != 0) {
        next = transform->next;
        HSD_Free(transform);
        transform = next;
    }
    freeAppSRT = 0;
}
HSD_psAppSRT *psAllocAppSRT(int status, u16 id)
{
    HSD_psAppSRT *transform;
    if (freeAppSRT == 0) {
        freeAppSRT = HSD_Alloc(psSizeofAppSRT);
        if (freeAppSRT != 0)
            memset(freeAppSRT, 0, psSizeofAppSRT);
    }
    transform = freeAppSRT;
    if (transform != 0) {
        freeAppSRT = transform->next;
        transform->next = 0;
        transform->usedCount = 1;
        transform->frameNum = 0;
        transform->status = status;
        transform->translate.z = 0.0F;
        transform->translate.y = 0.0F;
        transform->translate.x = 0.0F;
        transform->rot.z = 0.0F;
        transform->rot.y = 0.0F;
        transform->rot.x = 0.0F;
        transform->scale.z = 1.0F;
        transform->scale.y = 1.0F;
        transform->scale.x = 1.0F;
        transform->freefunc = 0;
        transform->xA2 = 0;
        transform->gp = 0;
        transform->idnum = id;
        numActivePSAppSRT++;
        if (numActivePSAppSRT > numPeakPSAppSRT)
            numPeakPSAppSRT = numActivePSAppSRT;
    }
    return transform;
}
HSD_psAppSRT *psAddGeneratorAppSRT(HSD_Generator *generator, int status)
{
    return generator->appsrt = psAllocAppSRT(status, generator->idnum);
}
int psAttachParticleAppSRT(HSD_Particle *particle, HSD_psAppSRT *transform)
{
    if (transform == 0 || particle == 0 || particle->appsrt != 0)
        return -1;
    particle->appsrt = transform;
    return ++transform->usedCount;
}
int psAttachGeneratorAppSRT(HSD_Generator *generator, HSD_psAppSRT *transform)
{
    if (transform == 0 || generator == 0 || generator->appsrt != 0)
        return -1;
    generator->appsrt = transform;
    return ++transform->usedCount;
}
int psRemoveParticleAppSRT(HSD_Particle *particle)
{
    HSD_psAppSRT *transform;
    int count;
    transform = particle->appsrt;
    if (transform == 0)
        return -1;
    count = --transform->usedCount;
    if (count == 0) {
        transform = particle->appsrt;
        if (transform->freefunc != 0)
            transform->freefunc(transform);
        transform->next = freeAppSRT;
        freeAppSRT = transform;
        numActivePSAppSRT--;
    }
    particle->appsrt = 0;
    return count;
}
int psRemoveGeneratorAppSRT(HSD_Generator *generator)
{
    HSD_psAppSRT *transform;
    int count;
    transform = generator->appsrt;
    if (transform == 0)
        return -1;
    {
        HSD_Generator **owner = &transform->gp;
        if (*owner == generator)
            *owner = 0;
    }
    transform = generator->appsrt;
    count = --transform->usedCount;
    if (count == 0) {
        transform = generator->appsrt;
        if (transform->freefunc != 0)
            transform->freefunc(transform);
        transform->next = freeAppSRT;
        freeAppSRT = transform;
        numActivePSAppSRT--;
    }
    generator->appsrt = 0;
    return count;
}

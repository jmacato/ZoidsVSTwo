#include "particle.h"
extern void *HSD_Alloc(u32 size);
extern void HSD_Free(void *memory);
extern void *memset(void *memory, s32 value, u32 size);
extern void __assert(const char *file, u32 line, const char *condition);
extern u16 numActiveParticles;
extern u16 numPeakParticles;
static HSD_Particle *activeParticle[16];
static int sortDirty[16];
static HSD_Particle *activeParticleXLU[16];
static HSD_Particle *freeParticle;
int _psLinkInit(int count)
{
    HSD_Particle *particle;
    int i;
    for (i = 0; i < 16; i++) {
        activeParticle[i] = 0;
        sortDirty[i] = 0;
        activeParticleXLU[i] = 0;
    }
    freeParticle = 0;
    for (i = count - 1; i >= 0; i--) {
        particle = HSD_Alloc(sizeof(HSD_Particle));
        /* Keep the SDK allocation and clearing order. */
        memset(particle, 0, sizeof(HSD_Particle));
        if (particle == 0)
            return -1;
        particle->next = freeParticle;
        freeParticle = particle;
    }
    return i;
}
void _psListClear(void)
{
    HSD_Particle *particle;
    HSD_Particle *next;
    int i;
    particle = freeParticle;
    while (particle != 0) {
        next = particle->next;
        HSD_Free(particle);
        particle = next;
    }
    freeParticle = 0;
    for (i = 0; i < 16; i++) {
        particle = activeParticle[i];
        while (particle != 0) {
            next = particle->next;
            HSD_Free(particle);
            particle = next;
        }
        activeParticle[i] = 0;
        sortDirty[i] = 0;
        activeParticleXLU[i] = 0;
    }
}
HSD_Particle *_psListNew(HSD_Particle **head, int link)
{
    HSD_Particle *particle;
    if (freeParticle == 0) {
        freeParticle = HSD_Alloc(sizeof(HSD_Particle));
        memset(freeParticle, 0, sizeof(HSD_Particle));
    }
    particle = freeParticle;
    if (particle == 0)
        return 0;
    numActiveParticles++;
    if (numActiveParticles > numPeakParticles)
        numPeakParticles = numActiveParticles;
    freeParticle = particle->next;
    if (head == 0) {
        HSD_Particle **slot = &activeParticle[link];
        particle->next = *slot;
        *slot = particle;
    } else {
        particle->next = *head;
        *head = particle;
    }
    sortDirty[link] = 1;
    return particle;
}
void _psListDelete(HSD_Particle *particle, HSD_Particle *previous)
{
    sortDirty[particle->linkNo] = 1;
    if (previous == 0) {
        if (activeParticle[particle->linkNo] != particle)
            __assert("pslist.c", 136, "activeParticle[pp->linkNo] == pp");
        activeParticle[particle->linkNo] = particle->next;
    } else {
        if (previous->next != particle)
            __assert("pslist.c", 139, "parent->next == pp");
        previous->next = particle->next;
    }
    particle->next = freeParticle;
    freeParticle = particle;
    numActiveParticles--;
}
HSD_Particle *_psListGetFirst(int link)
{
    if ((link >= 0 && link < 16) == 0)
        __assert("pslist.c", 152, "linkNo >= 0 && linkNo < PS_NUM_LINK");
    return activeParticle[link];
}
HSD_Particle *particleSort(int link, HSD_Particle **opaque, HSD_Particle **translucent)
{
    struct ParticleBucket {
        HSD_Particle *first;
        HSD_Particle *last;
    } buckets[16];
    s32 already_sorted = sortDirty[link] == 0;
    HSD_Particle *particle = activeParticle[link];
    HSD_Particle *next;
    HSD_Particle *opaque_tail;
    HSD_Particle *opaque_head;
    HSD_Particle *translucent_head;
    HSD_Particle *translucent_tail;
    HSD_Particle *head;
    int blend;
    int key;
    int i;
    if (already_sorted) {
        if (particle == 0) {
            *opaque = 0;
            *translucent = 0;
            return 0;
        }
        if (particle->kind & TexEdge)
            *opaque = particle;
        else
            *opaque = 0;
        *translucent = activeParticleXLU[link];
        return particle;
    }
    blend = 0;
    sortDirty[link] = 0;
    if (particle == 0) {
        activeParticleXLU[link] = 0;
        *opaque = 0;
        *translucent = 0;
        return 0;
    }
    memset(buckets, 0, sizeof(buckets));
    blend = particle->kind & TexEdge ? 0 : 1;
    key = ((particle->kind >> 25) & 7) + (blend << 3);
    buckets[key].first = particle;
    next = particle->next;
    while (next != 0) {
        if ((particle->kind ^ next->kind) & 0x0E000008) {
            /* Join runs with the same render flags without reversing them. */
            buckets[key].last = particle;
            blend = next->kind & TexEdge ? 0 : 1;
            key = ((next->kind >> 25) & 7) + (blend << 3);
            if (buckets[key].first == 0)
                buckets[key].first = next;
            else
                buckets[key].last->next = next;
        }
        particle = next;
        next = next->next;
    }
    buckets[key].last = particle;
    opaque_tail = 0;
    opaque_head = 0;
    translucent_tail = 0;
    translucent_head = 0;
    for (i = 0; i < 8; i++) {
        if (buckets[i].first != 0) {
            if (opaque_head == 0)
                opaque_head = buckets[i].first;
            else
                opaque_tail->next = buckets[i].first;
            opaque_tail = buckets[i].last;
        }
    }
    for (i = 8; i < 16; i++) {
        if (buckets[i].first != 0) {
            if (translucent_head == 0)
                translucent_head = buckets[i].first;
            else
                translucent_tail->next = buckets[i].first;
            translucent_tail = buckets[i].last;
        }
    }
    head = 0;
    if (opaque_tail != 0) {
        head = opaque_head;
        opaque_tail->next = translucent_head;
    }
    if (translucent_tail != 0) {
        if (head == 0)
            head = translucent_head;
        translucent_tail->next = 0;
    }
    activeParticle[link] = head;
    activeParticleXLU[link] = translucent_head;
    *opaque = opaque_head;
    *translucent = translucent_head;
    return head;
}

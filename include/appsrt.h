/* Types from
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/psstructs.h. */
#ifndef ZOIDS_APPSRT_H
#define ZOIDS_APPSRT_H
#include "particle.h"
#include "matrix.h"
struct HSD_psAppSRT {
    HSD_psAppSRT *next;
    HSD_Generator *gp;
    Vec3 translate;
    Quaternion rot;
    Vec3 scale;
    u8 status;
    u8 frameNum;
    u16 usedCount;
    Mtx mmtx;
    Mtx vmmtx;
    f32 view_scale_x;
    f32 view_scale_y;
    void (*freefunc)(HSD_psAppSRT *transform);
    u16 idnum;
    u8 xA2;
};
extern int psInitAppSRT(int count, int size);
extern void psRemoveAppSRT(void);
extern HSD_psAppSRT *psAllocAppSRT(int status, u16 id);
extern HSD_psAppSRT *psAddGeneratorAppSRT(HSD_Generator *generator, int status);
extern int psAttachParticleAppSRT(HSD_Particle *particle, HSD_psAppSRT *transform);
extern int psAttachGeneratorAppSRT(HSD_Generator *generator, HSD_psAppSRT *transform);
extern int psRemoveParticleAppSRT(HSD_Particle *particle);
extern int psRemoveGeneratorAppSRT(HSD_Generator *generator);
#endif

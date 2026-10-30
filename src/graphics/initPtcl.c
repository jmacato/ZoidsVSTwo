#include "types.h"

extern void psInitParticle(s32 count);
extern void psInitGenerator(s32 count);
extern s32 psInitAppSRT(s32 a, s32 count);
extern void psCreateGeneratorIDJObjS(void);
extern void psCreateGeneratorIDJObjD(void);
extern void psSetPointJObjNodup(void);
extern void HSD_JObjSetSPtclCallback(void (*callback)(void));
extern void HSD_JObjSetDPtclCallback(void (*callback)(void));
extern void HSD_JObjSetPtclTargetCallback(void (*callback)(void));

void initPtcl(void)
{
    psInitParticle(3000);
    psInitGenerator(300);
    psInitAppSRT(0, 164);
    HSD_JObjSetSPtclCallback(psCreateGeneratorIDJObjS);
    HSD_JObjSetDPtclCallback(psCreateGeneratorIDJObjD);
    HSD_JObjSetPtclTargetCallback(psSetPointJObjNodup);
}

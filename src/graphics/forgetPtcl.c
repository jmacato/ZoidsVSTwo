#include "types.h"

extern void *pTref;
extern void *pTtexGroup;
extern void *pTcmdList;
extern void psRemoveParticle(void);
extern void psRemoveGenerator(void);
extern void psRemoveAppSRT(void);
extern void waitV(s32 count);
extern void free(void *memory);

void forgetPtcl(void)
{
    psRemoveParticle();
    psRemoveGenerator();
    psRemoveAppSRT();
    waitV(3);
    free(pTref);
    free(pTtexGroup);
    free(pTcmdList);
}

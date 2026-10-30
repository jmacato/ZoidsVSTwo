#include "types.h"

extern s32 MemCardOn[2];
extern s32 MemCardStatus[2];
extern s32 MemCardError;
extern void CARDInit(void);
extern void OSReport(const char *format, ...);

void TEA_mcInit(void)
{
    CARDInit();
    MemCardOn[0] = 0;
    MemCardOn[1] = 0;
    MemCardError = 0;
    MemCardStatus[0] = 0;
    MemCardStatus[1] = 0;
    OSReport("MemCardInit\n");
}

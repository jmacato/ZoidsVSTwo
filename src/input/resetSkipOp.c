#include "types.h"

extern void gClear(void);
extern void gRefresh(void);
extern s32 sReadPre(void);
extern void fixPut(void);
extern void *fixSt;
extern void *fixEd;
extern void OSSetSaveRegion(void *start, void *end);
extern void OSReport(const char *format, ...);
extern void OSResetSystem(int reset, unsigned long reset_code, int force_menu);
extern u8 GameMode[];
extern void resetSkipOp(void);

void resetSkipOp(void)
{
    gClear();
    gRefresh();
    gClear();
    sReadPre();
    gClear();
    fixPut();
    OSSetSaveRegion(fixSt, fixEd);
    OSReport("addr = %X - %X prg%X \n", GameMode, GameMode + 0x44, resetSkipOp);
    OSResetSystem(0, 0x8800, 0);
}

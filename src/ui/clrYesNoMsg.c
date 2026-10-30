#include "types.h"

extern s32 menudispyesnoflag;
extern void menuYesNoSetStr(const char *first, const char *second);

void clrYesNoMsg(void)
{
    menudispyesnoflag = 0;
    menuYesNoSetStr("", "");
}

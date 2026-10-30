#include "types.h"

extern s32 TEA_mcFileOldFileChk(s32 slot, s32 index);
extern s32 printf(const char *format, ...);

s32 TEA_mcFileOldFileExist(s32 slot)
{
    s16 index = 0;
    for (index = 0; index < 0x100; index++) {
        if (TEA_mcFileOldFileChk(slot, index) == 1)
            return 1;
    }
    printf("\213\214\203]\203C\203h\203t\203@\203C\203\213\221\266\215\335\202\271\202\270\n");
    return 0;
}

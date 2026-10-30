#include "types.h"

extern s32 DVDCheckDisk(void);
extern s32 dOpen;

void dInt(void)
{
    if (DVDCheckDisk() == 0) {
        dOpen = 1;
    }
}

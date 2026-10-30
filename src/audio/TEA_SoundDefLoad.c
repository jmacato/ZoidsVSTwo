#include "types.h"

extern u8 TeaSoundSfxLoadNum;
extern void sReadPre(void);
extern void HSD_AudioSFXPop(void);
extern void TEA_SoundSfxLoad(char *path, s32 group);

void TEA_SoundDefLoad(void)
{
    sReadPre();
    for (;;) {
        if (TeaSoundSfxLoadNum == 0)
            break;
        HSD_AudioSFXPop();
        TeaSoundSfxLoadNum--;
    }
    TEA_SoundSfxLoad("audio/se/se", 8);
    TeaSoundSfxLoadNum = 1;
}

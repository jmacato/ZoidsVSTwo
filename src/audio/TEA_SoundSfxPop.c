#include "types.h"

typedef struct SfxName {
    char name[0x50];
    s16 group;
    u8 reserved_52[2];
    s32 loaded;
} SfxName;

extern u8 TeaSoundSfxLoadNum;
extern SfxName TeaSfxNamePush[10];
extern void OSReport(const char *format, ...);
extern void HSD_AudioSFXPop(void);

void TEA_SoundSfxPop(void)
{
    if (TeaSoundSfxLoadNum != 0) {
        TeaSoundSfxLoadNum--;
        TeaSfxNamePush[TeaSoundSfxLoadNum].loaded = 0;
        OSReport("SE UnLoaded POP POP POP now=%d ", TeaSoundSfxLoadNum);
        HSD_AudioSFXPop();
        OSReport(" pop OFF\n");
    }
}

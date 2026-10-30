#include "types.h"

typedef struct SfxName {
    char name[0x50];
    s16 group;
    u8 reserved_52[2];
    s32 loaded;
} SfxName;

static u32 aram_block[16];
extern SfxName TeaSfxNamePush[10];
extern u8 TeaSoundSfxLoadNum;
extern void ARInit(u32 *stack, u32 count);
extern void ARQInit(void);
extern void AIInit(u8 *stack);
extern void *malloc2(u32 size);
extern void free(void *ptr);
extern void HSD_AudioSetHooks(void *(*alloc)(u32), void (*release)(void *));
extern void HSD_AudioInit(s32 voices, s32 music, s32 sfx, u32 aram);

void TEA_SoundInit(void)
{
    ARInit(aram_block, 16);
    ARQInit();
    AIInit(0);
    HSD_AudioSetHooks(malloc2, free);
    HSD_AudioInit(62, 48, 14, 0xC00000);
    TeaSoundSfxLoadNum = 0;
    TeaSfxNamePush[TeaSoundSfxLoadNum].loaded = 0;
}

#include "joint.h"

typedef struct Lsr1_3 {
    u8 reserved_00[0x60];
    HSD_JObj *joint;
    u8 reserved_64[0x6C - 0x64];
} Lsr1_3;

extern Lsr1_3 sLsr1[400];
extern void HSD_JObjDispAll(HSD_JObj *joint, void *mtx, u32 flags, u32 rendermode);

void sLsrDisp(void)
{
    s32 i;
    Lsr1_3 *laser;

    for (i = 0; i < 400; i++) {
        laser = &sLsr1[i];
        if (laser->joint != (void *)0) {
            HSD_JObjDispAll(laser->joint, 0, 7, 0);
        }
    }
}

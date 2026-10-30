#include "joint.h"

typedef struct Lsr1_3 {
    u8 reserved_00[0x60];
    HSD_JObj *joint;
    u8 reserved_64[0x6C - 0x64];
} Lsr1_3;

extern void HSD_JObjRemoveAnimAll(HSD_JObj *joint);
extern void psKillJObjAllGenerator(HSD_JObj *joint);
extern void HSD_JObjUnref(HSD_JObj *joint);

void sLsr1Purge(Lsr1_3 *laser)
{
    if (laser != (void *)0 && laser->joint != (void *)0) {
        HSD_JObjRemoveAnimAll(laser->joint);
        psKillJObjAllGenerator(laser->joint);
        HSD_JObjUnref(laser->joint);
        laser->joint = (void *)0;
    }
}

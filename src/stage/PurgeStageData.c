#include "model_archive.h"
#include "fog.h"

typedef struct CameraState3 {
    s32 mode;
    s32 half;
    void *lights;
    u8 reserved_00C[0x590 - 0x0C];
} CameraState3;

extern ModelArchive Ground;
extern ModelArchive *Sky;
extern struct HSD_Fog *fogG;
extern CameraState3 sCamera[4];
extern s32 nowStage;
extern void GroundObjPurge(void);
extern void PurgeArc(ModelArchive *);
extern void PurgeArc2(ModelArchive *);
extern void sColEpilogue(void);

void PurgeStageData(void)
{
    GroundObjPurge();
    PurgeArc(&Ground);
    sCamera[0].lights = 0;
    sCamera[1].lights = 0;
    PurgeArc2(Sky);
    Sky = 0;
    if (fogG != 0)
        fogG = 0;
    sColEpilogue();
    nowStage = -1;
}

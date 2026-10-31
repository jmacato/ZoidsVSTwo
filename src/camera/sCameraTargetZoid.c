#include "zoid3.h"

typedef struct CameraState3 {
    u8 pad_000[0x1C0];
    HSD_JObj *field_1C0;
    ZoidState *zoid;
    s32 target_index;
} CameraState3;

void sCameraTargetZoid(CameraState3 *camera, ZoidState *zoid)
{
    HSD_JObj *joint;
    if (camera != (void *)0 && zoid != (void *)0) {
        camera->zoid = zoid;
        joint = 0;
        camera->target_index = *((u8 *)zoid + 7560);
        zoid->camera = camera;
        if (*(HSD_JObj **)((u8 *)zoid + 1116) != 0)
            joint = *(HSD_JObj **)((u8 *)zoid + 1116);
        else if (*(HSD_JObj **)((u8 *)zoid + 1272) != 0)
            joint = *(HSD_JObj **)((u8 *)zoid + 1272);
        else if (*(HSD_JObj **)((u8 *)zoid + 1428) != 0)
            joint = *(HSD_JObj **)((u8 *)zoid + 1428);
        else if (*(HSD_JObj **)((u8 *)zoid + 1740) != 0)
            joint = *(HSD_JObj **)((u8 *)zoid + 1740);
        else if (*(HSD_JObj **)((u8 *)zoid + 1896) != 0)
            joint = *(HSD_JObj **)((u8 *)zoid + 1896);
        camera->field_1C0 = joint;
    }
}

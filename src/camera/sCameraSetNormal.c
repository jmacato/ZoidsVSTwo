#include "types.h"

typedef struct CameraState3 {
    s32 mode;
    s32 half;
    u8 pad_008[0x1C0 - 0x08];
    void *field_1C0;
    void *zoid;
    u8 pad_1C8[0x244 - 0x1C8];
    void *cobj;
} CameraState3;

extern u8 cameraNormal[];
extern s32 sCamNN;
extern s32 sCamNS;
extern void sCameraReset(void *camera);
extern void *HSD_CObjLoadDesc(void *desc);

void sCameraSetNormal(CameraState3 *camera)
{
    if (camera != (void *)0) {
        sCameraReset(camera);
        camera->cobj = HSD_CObjLoadDesc(cameraNormal);
        camera->field_1C0 = 0;
        camera->zoid = 0;
        camera->mode = 1;
        sCamNN = 1;
        sCamNS = 0;
        camera->half = 0;
    }
}

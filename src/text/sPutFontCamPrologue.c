#include "cobj.h"
#include "lobj.h"

extern HSD_CObj *camFont;

s32 sPutFontCamPrologue(void)
{
    if (camFont == 0) {
        return 0;
    } else {
        Vec3 eye = {0.0f, 0.0f, 0.0f};
        Vec3 interest = {0.0f, 0.0f, -6000.0f};
        HSD_CObjSetEyePosition(camFont, &eye);
        HSD_CObjSetInterest(camFont, &interest);
        if (HSD_CObjSetCurrent(camFont) == 0)
            return 0;
        HSD_LObjSetupInit(camFont);
        return 1;
    }
}

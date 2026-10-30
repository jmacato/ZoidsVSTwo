#include "cobj.h"

extern s32 sStringNow;
extern HSD_CObj *camFont;
extern void sPutFontAllPrologue(void);
extern void sPutFontSt(s32 index);
extern void sPutFontAllEpilogue(void);

void sPutFontAllN(void)
{
    s32 i;
    if (sStringNow != 0) {
        HSD_CObjSetCurrent(camFont);
        sPutFontAllPrologue();
        for (i = 0; i < sStringNow; i++) {
            sPutFontSt(i);
        }
        sPutFontAllEpilogue();
        HSD_CObjEndCurrent();
    }
}

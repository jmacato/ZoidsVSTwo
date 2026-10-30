#include "os_font.h"
#include "os_alloc.h"
#include "cobj.h"

extern OSFontHeader *FontData;
extern s16 FontSize, FontSpace;
extern HSD_CObj *camFont;
extern HSD_CObjDesc cameraFont;

static inline s32 decrement_reference(void *object)
{
    s32 result;
    u16 *count = &((HSD_Obj *)object)->reference_count;
    if ((result = (*count == (u16)-1)))
        return result;
    return (*count)-- == 0;
}

static inline void HSD_CObjUnref(HSD_CObj *object)
{
    if (object != 0 && decrement_reference(object)) {
        if (object != 0) {
            object->parent.parent.class_info->release((HSD_Class *)object);
            object->parent.parent.class_info->destroy((HSD_Class *)object);
        }
    }
}

s32 sFontInit(void)
{
    if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS)
        FontData = OSAllocFromHeap(__OSCurrHeap, OS_FONT_SIZE_SJIS);
    else
        FontData = OSAllocFromHeap(__OSCurrHeap, OS_FONT_SIZE_ANSI);
    if (!OSInitFont(FontData))
        return 0;
    FontSize = (s16)(FontData->cellWidth * 16);
    FontSpace = -16;
    camFont = HSD_CObjLoadDesc(&cameraFont);
    return 1;
}



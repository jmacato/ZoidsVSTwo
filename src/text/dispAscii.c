#include "zoid3.h"
#include "model_archive.h"
#include "gx_tev.h"

#pragma auto_inline off
#pragma readonly_strings on

typedef struct AsciiEntry {
    char text[80];
    f32 scroll_y;
} AsciiEntry;

#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40

extern AsciiEntry *sStr;
extern ModelArchive *textPos;
extern HSD_JObj *j0, *j1;
extern f32 strConfUp;
extern HSD_JObj *GetChild(HSD_JObj *joint, s32 first, s32 second, s32 third, s32 fourth, s32 fifth);
extern void jobjCopyPos(HSD_JObj *source, HSD_JObj *target);
extern void jobjDispAll(HSD_JObj *joint);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *jobj);
extern void sPutFontRect(char *text, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow, s32 alpha, s32 flags);
extern void __assert(const char *file, u32 line, const char *condition);

static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY);
}

static inline void HSD_JObjSetTranslateY(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 966, "jobj");
    jobj->translation.y = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
            HSD_JObjSetMtxDirtySub(jobj);
    }
}

void dispAscii(void)
{
    HSD_JObj *jobj;
    s32 i;
    AsciiEntry *entry;
    s32 child;

    for (i = 0; i < 40; i++) {
        entry = &sStr[i];
        if (entry->text[0] == 0)
            continue;
        if (entry->text[0] == '!') {
            child = entry->text[1] & 0xF;
            jobj = GetChild(textPos->models[3], 0, child, -1, -1, -1);
            HSD_JObjSetTranslateY(jobj, entry->scroll_y);
            jobjDispAll(jobj);
        } else {
            jobjCopyPos(GetChild(textPos->models[0], 0, -1, -1, -1, -1), j0);
            jobjCopyPos(GetChild(textPos->models[0], 0, 0, -1, -1, -1), j1);
            j0->matrix[1][3] += entry->scroll_y;
            j1->matrix[1][3] += entry->scroll_y;
            sPutFontRect(entry->text, j0, j1, (GXColor){255, 255, 255, 255}, (GXColor){0, 0, 0, 0}, 0, 2);
        }
        entry->scroll_y += strConfUp;
        if (entry->scroll_y > 100.0f) {
            entry->text[0] = 0;
            entry->scroll_y = 0.0f;
        }
    }
}

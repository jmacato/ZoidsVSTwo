#include "model_archive.h"

extern ModelArchive *textPos;
extern s32 dOn;
extern void jobjDispAll(HSD_JObj *joint);

void closeDispC(void)
{
    if (dOn != 0)
        jobjDispAll(textPos->models[4]);
}

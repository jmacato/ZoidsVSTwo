#include "game_text.h"

void ascEpilogue(void)
{
    s32 i;
    for (i = 0; i < ASC_FONT_COUNT; i++) {
        PurgeArc2(ascFile[i]);
        ascFile[i] = 0;
    }
}

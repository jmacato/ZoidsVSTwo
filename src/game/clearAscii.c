#include "types.h"
typedef struct AsciiEntry {
    char text[80];
    f32 scroll_y;
} AsciiEntry;

extern AsciiEntry *sStr;

void clearAscii(void)
{
    s32 i;

    for (i = 0; i < 40; i++)
        sStr[i].text[0] = 0;
}

#include "types.h"
#pragma readonly_strings on

typedef struct AsciiEntry {
    char text[80];
    f32 scroll_y;
} AsciiEntry;

extern AsciiEntry *sStr;
extern char *strcpy(char *destination, const char *source);

void setAscii(char *text)
{
    AsciiEntry *entry;
    s32 i;

    for (i = 0; i < 40; i++) {
        entry = &sStr[i];
        if (entry->text[0] == 0) {
            entry->scroll_y = 0.0f;
            strcpy(entry->text, text);
            if (entry->text[0] == 0)
                strcpy(entry->text, "  ");
            return;
        }
    }
}

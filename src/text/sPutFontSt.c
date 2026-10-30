#include "gx_tev.h"

typedef struct FontString {
    char *text;
    s32 x;
    s32 y;
    s32 z;
    GXColor color;
    GXColor shadow;
    s32 alpha;
    s32 flags;
} FontString;

extern s32 sStringNow;
extern FontString sString[1000];
/* by value to match (the definition takes GXColor *; pointers drop the stack copies) */
extern void sPutFont1(char *text, s32 x, s32 y, s32 z, GXColor color, GXColor shadow,
                      s32 alpha, s32 flags);

void sPutFontSt(s32 index)
{
    FontString *string;
    if (index < sStringNow) {
        string = &sString[index];
        sPutFont1(string->text, string->x, string->y, string->z, string->color,
                  string->shadow, string->alpha, string->flags);
    }
}

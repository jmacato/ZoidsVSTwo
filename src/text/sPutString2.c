#include "gx_tev.h"

extern void sPutFontAllPrologue(void);
extern void sPutFontAllEpilogue(void);
/* by value to match (the definition takes GXColor *; pointers drop the stack copies) */
extern void sPutFont1(char *text, s32 x, s32 y, s32 z, GXColor color, GXColor shadow,
                      s32 alpha, s32 flags);

void sPutString2(char *text, s32 x, s32 y, s32 z, GXColor color, GXColor shadow, s32 alpha,
                 s32 flags)
{
    sPutFontAllPrologue();
    sPutFont1(text, x, y, z, color, shadow, alpha, flags);
    sPutFontAllEpilogue();
}

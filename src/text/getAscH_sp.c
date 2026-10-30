#include "game_text.h"

#pragma auto_inline off
extern s32 getAscNum(char *text);

f32 getAscH_sp(s32 font, char *text, f32 spacing)
{
    f32 width;
    s32 index;

    if (font >= ASC_FONT_COUNT)
        return 0.0F;
    width = 0.0F;
    while (*text) {
        index = getAscNum(text);
        if (index >= ASC_GLYPH_MAX)
            index = -1;
        if (index != -1) {
            width += ascH[font][index];
            if (isKanji(*text))
                text += 2;
            else
                text++;
        } else {
            if (isKanji(*text))
                text += 2;
            else
                text++;
            continue;
        }
        width += spacing;
    }
    return width;
}

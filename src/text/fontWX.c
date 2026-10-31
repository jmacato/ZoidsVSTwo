#include "os_font.h"

f32 fontWX(char *text)
{
    s32 width;
    void *image;
    s32 s;
    s32 t;
    f32 total;

    total = 0.0f;
    if (text == 0)
        return 0.0f;
    if (*text == 0)
        return 0.0f;
    while (*text != 0) {
        if (*text == '\n') {
            text++;
        } else if (*text == '\r') {
            break;
        } else {
            text = OSGetFontTexture(text, &image, &s, &t, &width);
            total += width;
        }
    }
    return total;
}

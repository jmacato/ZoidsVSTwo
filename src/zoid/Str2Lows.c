#include "game_text.h"
#include "runtime_string.h"

extern s8 toLow(s8 character);

void Str2Lows(char *str)
{
    s32 len;
    char c;
    len = strlen(str);
    if (len != 0) {
        for (; len != 0; len--) {
            c = *str;
            if (c == 0)
                return;
            if (isKanji(c)) {
                str += 2;
            } else {
                c = toLow(c);
                *str = c;
                str++;
            }
        }
    }
}

#include "game_text.h"

s32 getAscNum1(s8 character)
{
    s8 *characters = asc01;
    s32 index = 0;

    while (*characters) {
        if (character == *characters) {
            return index;
        }
        characters++;
        index++;
    }
    return -1;
}
